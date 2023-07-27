#pragma once
#include <FastLED.h>

#define NUM_LEDS 128  // The number of LEDs in your strip
#define CLOCK_ROWS 7
#define CLOCK_COLS 20
#define EMPTY 255
#define EMP EMPTY

CRGB leds[NUM_LEDS];  // Define the array of leds, stores RGB values for each led

uint8_t balls[CLOCK_ROWS][CLOCK_COLS] = {
  // Define the array of balls, stores the array index of the LED values for that ball
  { EMPTY, EMPTY, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, EMPTY },                            //17
  { EMPTY, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, EMPTY },                     //18
  { EMPTY, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53 },                        //19
  { 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73 },                           //20
  { EMPTY, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92 },                        //19
  { EMPTY, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, EMPTY },          //18
  { EMPTY, EMPTY, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, EMPTY }  //17
};

// uint8_t balls[CLOCK_ROWS][CLOCK_COLS] = {
//   // Define the array of balls, stores the array index of the LED values for that ball
//       {EMP,EMP, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,EMP},                            //17
//     {EMP, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,EMP},                     //18
//   {EMP,  0, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53},                         //19
//     {1  , 2, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73},                             //20
//   {EMP, 2, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92},                         //19
//     {EMP, 93, 94, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,110,EMP},          //18
//       {EMP,EMP,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,EMP}  //17
// };


struct ball {
  uint8_t row;
  uint8_t col;
  uint8_t index; //The LED array index
  bool on;

  ball(uint8_t row, uint8_t col) : row(row), col(col), index(balls[row][col]), on(false) {
  }
};

ball digit1[10] = {
  ball(1,2), ball(1,3),
  ball(2,2), ball(2,4),
  ball(3,2), ball(3,3),
  ball(4,2), ball(4,4),
  ball(5,2), ball(5,3)
};

ball digit2[10] = {
  ball(1,6), ball(1,7),
  ball(2,6), ball(2,8),
  ball(3,6), ball(3,7),
  ball(4,6), ball(4,8),
  ball(5,6), ball(5,7)
};

ball digit3[10] = {
  ball(1,12), ball(1,13),
  ball(2,12), ball(2,14),
  ball(3,12), ball(3,13),
  ball(4,12), ball(4,14),
  ball(5,12), ball(5,13)
};

ball digit4[10] = {
  ball(1,16), ball(1,17),
  ball(2,16), ball(2,18),
  ball(3,16), ball(3,17),
  ball(4,16), ball(4,18),
  ball(5,16), ball(5,17)
};

ball dots[2] = {
  ball(2,10), ball(4,10)
};

void setDigitTo(ball *digit, int num) {
  switch (num) {
    case 0:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[4].on = false;
      digit[5].on = false;
      break;
    case 1:
      for(int i=0; i<10; i++) {
        digit[i].on = i%2;
      }
      break;
    case 2:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[2].on = false;
      digit[7].on = false;
      break;
    case 3:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[2].on = false;
      digit[6].on = false;
      break;
    case 4:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[1].on = false;
      digit[6].on = false;
      digit[8].on = false;
      break;
    case 5:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[3].on = false;
      digit[6].on = false;
      break;
    case 6:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[1].on = false;
      digit[3].on = false;
      break;
    case 7:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[4].on = false;
      digit[6].on = false;
      digit[8].on = false;
      break;
    case 8:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      break;
    case 9:
      for(int i=0; i<10; i++) {
        digit[i].on = true;
      }
      digit[6].on = false;
      digit[8].on = false;
      break;
  }
}

//converts hsv to rgb
CRGB hsvToRgb(uint16_t h, uint8_t s, uint8_t v) {
  uint8_t f = (h % 60) * 255 / 60;
  uint8_t p = (255 - s) * (uint16_t)v / 255;
  uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
  uint8_t r = 0, g = 0, b = 0;
  switch ((h / 60) % 6) {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    case 5:
      r = v;
      g = p;
      b = q;
      break;
  }
  return CRGB(r, g, b);
}

//sets all leds to a time variant rainbow pattern
//takes in speed multiplier as a percentage
void rainbow(uint speed=100) {
  uint16_t time = int(millis() * float(speed/100.0)) >> 2;
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    byte x = (time >> 2) - (i << 3);
    leds[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, 255);
    //leds[i] = CHSV(x, 255, 255);
  }
}

CRGB fire_pallette[] = {
  CRGB::Red,
  CRGB::Orange,
  CRGB::Yellow,
  CRGB::White
};

//sets leds in a time variant fire animation
//takes in speed multiplier as a percentage
void fire(uint speed=100) {
  for(int row=0; row<CLOCK_ROWS; row+=1) {
    for(int col=0; col<CLOCK_COLS; col+=1) {
      leds[balls[row][col]] = fire_pallette[random(4)];
    }
  }
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120

uint8_t heat[CLOCK_ROWS][CLOCK_COLS];

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell

  for (int col=0; col<CLOCK_COLS; col+=1) {
  // Step 1.  Cool down every cell a little
    for( int u = 0; u < CLOCK_ROWS; u+=1) {
      heat[u][col] = qsub8( heat[u][col],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int i = CLOCK_ROWS - 1; i >= 2; i--) {
      heat[i][col] = (heat[i - 1][col] + heat[i - 2][col] + heat[i - 2][col] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    //if( random8() < SPARKING ) {
      //int j = 0;
      //int row = random8(1);
      // int t = heat[row][col] + random8(160,254);
      // if( t > 255) t = 255;
      //heat[row][col] = random8(160,254);
      //heat[j][col] = 230;
    //}
    // Step 4.  Map from heat cells to LED colors
    for( int k = 0; k < NUM_LEDS; k+=1) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      uint8_t colorindex = scale8( heat[k][col], 240);
      //CRGB color = ColorFromPalette( gPal, colorindex);
      CRGB color = HeatColor(colorindex);
      int pixelnumber = balls[k][col];
      leds[pixelnumber] = color;
    }
  }
}
