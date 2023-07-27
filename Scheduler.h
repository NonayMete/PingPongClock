struct Scheduler {
    unsigned long lastTime = micros();
    unsigned long interval = 0; //in uS

    static Scheduler usec (unsigned long interval) {
    Scheduler s;
    s.interval = interval;
    return s;
    }

    static Scheduler msec (unsigned long interval) {
    Scheduler s;
    s.interval = interval * 1000;
    return s;
    }

    static Scheduler sec (unsigned long interval) {
    Scheduler s;
    s.interval = interval * 1000000;
    return s;
    }

    static Scheduler min (unsigned long interval) {
    Scheduler s;
    s.interval = interval * 60000000;
    return s;
    }

    static Scheduler frequency (unsigned long hertz) {
    Scheduler s;
    s.interval = 1000000 / hertz;
    return s;
    }

    bool shouldRun() {
    unsigned long now = micros();
    if(now - lastTime > interval) {
        lastTime = now;
        return true;
    }
    return false;
    }

    unsigned long timeLeft() {
    unsigned long now = micros();
    if(now - lastTime > interval) {
        return 0;
    }
    return interval - (now - lastTime);
    }

    // delay until the next time this should run
    // if warn is true, print a warning that no time is left
    void wait(bool warn = false) {
        unsigned long now = micros();
        if(now - lastTime > interval) {  
            lastTime = now;
            if(warn) {
                Serial.println("Scheduler: did not run in time");
            }  
            return;
        }
        delayMicroseconds(interval - (now - lastTime));
        lastTime = micros();
        return;
    }
};