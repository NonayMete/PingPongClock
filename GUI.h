#pragma once
#include "leds.h"
#include <ESPAsyncWebServer.h>
#include <FastLED.h>


AsyncWebSocket ws("/ws");

void setupGUI(AsyncWebServer *server) {
  ws.onEvent([](AsyncWebSocket *server_, AsyncWebSocketClient *client_, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    // Handle WebSocket event
  });

  server->on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String sliderValue;
      if (request->hasParam("value")) {
        sliderValue = request->getParam("value")->value();
        Serial.println(sliderValue);
        settings.setBrightness(sliderValue.toInt());
      }
      request->send(200, "text/plain", "OK");
    });

  server->on("/toggle24HrTime", HTTP_GET, [](AsyncWebServerRequest *request){
    String value;
    if (request->hasParam("value")) {
      value = request->getParam("value")->value();
      Serial.println("Setting 24hr time to:" + String(value.toInt()));
      settings.set24HrTime(value.toInt());
    }
    request->send(200, "text/plain", "OK");
  });

  server->on("/bgSelect", HTTP_GET, [](AsyncWebServerRequest *request){
    String value;
    if (request->hasParam("value")) {
      value = request->getParam("value")->value();
      Serial.println("Setting background to:" + String(value));
      settings.setBackground(value.toInt());
    }
    request->send(200, "text/plain", "OK");
  });

  server->on("/colSelect", HTTP_GET, [](AsyncWebServerRequest *request){
    String value;
    if (request->hasParam("value")) {
      value = request->getParam("value")->value();
      Serial.println("Setting background color to:" + String(value));
      settings.setBackgroundColor(value.toInt());
    }
    request->send(200, "text/plain", "OK");
  });


}

//ball params
#define BALL_RADIUS 20
#define STROKE_WIDTH 4

String dynamic_svg = R"(
  <div id="svgContainer"></div>
  <script>
  var radius = )" + String(BALL_RADIUS) + R"(;
  var stroke_width = )" + String(STROKE_WIDTH) + R"(;
  var cx = -(radius / 2);
  var cy = -(radius / 2);
  var id = 0;
  var CLOCK_ROWS = )" + String(CLOCK_ROWS) + R"(;
  var CLOCK_COLS = )" + String(CLOCK_COLS) + R"(;
  var EMPTY = )" + String(EMPTY) + R"(;
  let balls=[[EMPTY,EMPTY,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,EMPTY],
  [EMPTY,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,EMPTY],
  [EMPTY,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53],
  [54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73],
  [EMPTY,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92],
  [EMPTY,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,EMPTY],
  [EMPTY,EMPTY,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,EMPTY]];
  var height = (radius + stroke_width) * 2 * CLOCK_ROWS;
  var width = (radius + stroke_width) * 2 * CLOCK_COLS;
  // Create SVG element
  var svgContainer = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
  svgContainer.setAttribute('height', height);
  svgContainer.setAttribute('width', width);
  // Loop through all balls
  for (var i = 0; i < CLOCK_ROWS; i++) {
    if (i % 2) cx = -(radius / 2) + radius + 1;
    else cx = -(radius / 2);
    cy += 2 * radius - (radius / 5);
    for (var j = 0; j < CLOCK_COLS; j++) {
      id = balls[i][j];
      cx += 2 * radius + 2;
      if (id === EMPTY) continue;
      // Create a new circle element
      var newCircle = document.createElementNS('http://www.w3.org/2000/svg', 'circle');
      newCircle.setAttribute('id', 'circle' + id);
      newCircle.setAttribute('cx', cx);
      newCircle.setAttribute('cy', cy);
      newCircle.setAttribute('r', radius);
      newCircle.setAttribute('stroke', 'black');
      newCircle.setAttribute('stroke-width', stroke_width);
      newCircle.setAttribute('fill', 'rgb(0,0,0)');
      // Add the new circle to the SVG container
      svgContainer.appendChild(newCircle);
    }
  }
  // Add the SVG to the container div
  document.getElementById('svgContainer').appendChild(svgContainer);
  </script>)" + R"(
  <script>
  var ws = new WebSocket('ws://' + location.hostname + ':80/ws');
  ws.onmessage = function(event) {
    event.data.arrayBuffer().then(function(buffer) {
      var colors = new Uint8Array(buffer);
      // Assuming each circle is updated with three consecutive colors
      for (var i = 0; i <)" + String(NUM_LEDS) + R"(; i++) {
        document.getElementById("circle" + i).setAttribute("fill", "rgb(" + colors[i * 3] + "," + colors[i * 3 + 1] + "," + colors[i * 3 + 2] + ") ");
      }
    });
  };
  </script>
  )";

String createClockUI() {
  return dynamic_svg;
}


String getUI() {
  String userInterface = R"(
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js"></script>
    <p></p>)"
    + String(createClockUI()) + R"(
    <div style="display: table;">
      <div style="display: table-row">
        <div style="display: table-cell;">
          <fieldset class="block">
            <legend style="font-size:0.8em;"><b>Background Style:</b></legend>
              <div class="block_inner">
              <form id="radioForm">
                <input type="radio" name="bgSelect" value=)"+String(Backgrounds::ColourSolid)+R"(>Single Colour<br>
                <input type="radio" name="bgSelect" value=)"+String(Backgrounds::ColourGradient)+R"(>Gradient Colour<br>
                <input type="radio" name="bgSelect" value=)"+String(Backgrounds::Rainbow)+R"(>Rainbow<br>
                <input type="radio" name="bgSelect" value=)"+String(Backgrounds::Fire)+R"(>Fire<br>
                <input type="radio" name="bgSelect" value=)"+String(Backgrounds::Black)+R"(>Off<br>
              </form>
            </div>
          </fieldset>
        </div>
        <div style="display: table-cell;">
          <fieldset class="block">
            <legend style="font-size:0.8em;"><b>Colour:</b></legend>
              <div class="block_inner">
              <form id="radioForm">
              <div style="display: table;">
              <div style="display: table-row">
              <div style="display: table-cell;">
                <!-- Red -->
                <input type="radio" name="colSelect" value=16711680><span style="color:red;">Red</span><br>
                <input type="radio" name="colSelect" value=13458524><span style="color:IndianRed;">Indian Red</span><br>
                <input type="radio" name="colSelect" value=16416882><span style="color:salmon;">Salmon</span><br>
                <input type="radio" name="colSelect" value=11674146><span style="color:firebrick;">FireBrick</span><br>
                <input type="radio" name="colSelect" value=14423100><span style="color:Crimson;">Crimson</span><br>
                <input type="radio" name="colSelect" value=9109504><span style="color:DarkRed;">Dark Red</span><br>

                <!-- Pink -->
                <input type="radio" name="colSelect" value=16761035><span style="color:pink;">Pink</span><br>
                <input type="radio" name="colSelect" value=16716947><span style="color:deeppink;">Deep Pink</span><br>
                <input type="radio" name="colSelect" value=16738740><span style="color:hotpink;">Hot Pink</span><br>
                <input type="radio" name="colSelect" value=13047173><span style="color:mediumvioletred;">Medium Violet Red</span><br>
                <input type="radio" name="colSelect" value=14381203><span style="color:palevioletred;">Pale Violet Red</span><br>
                <input type="radio" name="colSelect" value=16753920><span style="color:Orange;">Orange</span><br>
                <input type="radio" name="colSelect" value=16752762><span style="color:LightSalmon;">LightSalmon</span><br>
              </div>
              <div style="display: table-cell;">
                <!-- Orange -->
                
                <input type="radio" name="colSelect" value=16744272><span style="color:Coral;">Coral</span><br>
                <input type="radio" name="colSelect" value=16737095><span style="color:Tomato;">Tomato</span><br>
                <input type="radio" name="colSelect" value=16729344><span style="color:OrangeRed;">Orange Red</span><br>
                <input type="radio" name="colSelect" value=16747520><span style="color:DarkOrange;">Dark Orange</span><br>

                <!-- Purple -->
                <input type="radio" name="colSelect" value=8388736><span style="color:purple;">Purple</span><br>
                <input type="radio" name="colSelect" value=15631086><span style="color:violet;">Violet</span><br>
                <input type="radio" name="colSelect" value=4915330><span style="color:indigo;">Indigo</span><br>
                <input type="radio" name="colSelect" value=15132410><span style="color:Lavender;">Lavender</span><br>
                <input type="radio" name="colSelect" value=16711935><span style="color:Fuchsia;">Fuchsia</span><br>
                <input type="radio" name="colSelect" value=9055202><span style="color:blueviolet;">Blue Violet</span><br>
                <input type="radio" name="colSelect" value=9109643><span style="color:darkmagenta;">Dark Magenta</span><br>
                <input type="radio" name="colSelect" value=14524637><span style="color:plum;">Plum</span><br>
                <input type="radio" name="colSelect" value=14315734><span style="color:orchid;">Orchid</span><br>

              </div>
              <div style="display: table-cell;">
                <!-- Yellow -->
                <input type="radio" name="colSelect" value=16776960><span style="color:Yellow;">Yellow</span><br>
                <input type="radio" name="colSelect" value=16766720><span style="color:Gold;">Gold</span><br>
                <input type="radio" name="colSelect" value=16773077><span style="color:PapayaWhip;">PapayaWhip</span><br>
                <input type="radio" name="colSelect" value=15787660><span style="color:Khaki;">Khaki</span><br>
                <input type="radio" name="colSelect" value=12433259><span style="color:DarkKhaki;">Dark Khaki</span><br>

                <!-- Green -->
                <input type="radio" name="colSelect" value=32768><span style="color:green;">Green</span><br>
                <input type="radio" name="colSelect" value=3050327><span style="color:seagreen;">Sea Green</span><br>
                <input type="radio" name="colSelect" value=2142890><span style="color:lightseagreen;">Light Sea Green</span><br>
                <input type="radio" name="colSelect" value=35723><span style="color:darkcyan;">Dark Cyan</span><br>
                <input type="radio" name="colSelect" value=32896><span style="color:teal;">Teal</span><br>
                <input type="radio" name="colSelect" value=65280><span style="color:lime;">Lime</span><br>
                <input type="radio" name="colSelect" value=3329330><span style="color:limegreen;">Lime Green</span><br>
                <input type="radio" name="colSelect" value=8388352><span style="color:chartreuse;">Chartreuse</span><br>
                
              </div>
              <div style="display: table-cell;">
                <input type="radio" name="colSelect" value=65407><span style="color:springgreen;">Spring Green</span><br>
                <input type="radio" name="colSelect" value=8421376><span style="color:Olive;">Olive</span><br>
                <!-- Blue -->
                <input type="radio" name="colSelect" value=255><span style="color:blue;">Blue</span><br>
                <input type="radio" name="colSelect" value=11393254><span style="color:lightblue;">Light Blue</span><br>
                <input type="radio" name="colSelect" value=8900331><span style="color:skyblue;">Sky Blue</span><br>
                <input type="radio" name="colSelect" value=49151><span style="color:deepskyblue;">Deep Sky Blue</span><br>
                <input type="radio" name="colSelect" value=8900346><span style="color:lightskyblue;">Light Sky Blue</span><br>
                <input type="radio" name="colSelect" value=2003199><span style="color:dodgerblue;">Dodger Blue</span><br>
                <input type="radio" name="colSelect" value=6591981><span style="color:cornflowerblue;">Cornflower Blue</span><br>
                <input type="radio" name="colSelect" value=65535><span style="color:Cyan;">Cyan</span><br>
                <input type="radio" name="colSelect" value=6266528><span style="color:cadetblue;">Cadet Blue</span><br>
                <input type="radio" name="colSelect" value=4620980><span style="color:steelblue;">Steel Blue</span><br>
                <input type="radio" name="colSelect" value=4286945><span style="color:royalblue;">Royal Blue</span><br>
                <input type="radio" name="colSelect" value=128><span style="color:Navy;">Navy</span><br>
              
              </div>
              </div>
              </div>

              </form>
              </div>
          </fieldset>
        </div>
      </div>
    </div>

    <script>
      //background type
      var radios = document.getElementsByName("bgSelect");
      for(var i=0; i<radios.length; i++) {
          radios[i].onclick = function() {
              // send value to the server
              if(this.checked) {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", "/bgSelect?value="+this.value, true);
                xhttp.send();
              }
          }
      }

      //colour
      var radios = document.getElementsByName("colSelect");
      for(var i=0; i<radios.length; i++) {
          radios[i].onclick = function() {
              // send value to the server
              if(this.checked) {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", "/colSelect?value="+this.value, true);
                xhttp.send();
              }
          }
      }


    </script>


    <div style="display: flex; align-items: center;">
    <p style="margin-right: 10px;">24 Hour Time:</p>
    <label class="switch">
        <input type="checkbox" id="toggleBox0" )" + (settings.get24HrTime() ? "checked" : "") + R"(>
        <span class="slider round"></span>
    </label>
    </div>

    <script>
      document.getElementById('toggleBox0').addEventListener('change', function() {
        var isChecked = this.checked ? '1' : '0';
        fetch('/toggle24HrTime?value=' + isChecked);
      });
    </script>

    <p>Brightness: <span id="sliderValue">)" + String(settings.getBrightness()) + R"(</span></p>
    <input type="range" min="0" max="100" value=")" + String(settings.getBrightness()) + R"(" class="slider" id="myRange">
    <script>
    var slider = document.getElementById("myRange");
    var output = document.getElementById("sliderValue");
    output.innerHTML = slider.value;

    slider.oninput = function() {
      output.innerHTML = this.value;
      var xhttp = new XMLHttpRequest();
      xhttp.open("GET", "/slider?value="+this.value, true);
      xhttp.send();
    }
    </script>
    
    <p>R Value: <input type="range" min="0" max="255" value="0" class="slider" id="R"></p>
    <p>G Value: <input type="range" min="0" max="255" value="0" class="slider" id="G"></p>
    <p>B Value: <input type="range" min="0" max="255" value="0" class="slider" id="B"></p>


  )";
  return userInterface;
}

void updateWebPage(void * parameter) {
  //Update LED's on web page
  for(;;) {
    //Update WebPage every approx 500ms
    delay(500);
    if(ws.count() > 0) { //only bother sending if a client is connected
      ws.binaryAll((uint8_t *)leds, NUM_LEDS * sizeof(CRGB));
    }
  }
}

