#ifndef GAME_MANAGER_H

#include <Arduino.h>
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>

#define GAME_MANAGER_H

#ifdef __AVR__
  #include <avr/power.h>
#endif

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif

#define LED_PIN         6
#define INTERRUPT_PIN   10
#define ALL_PIXELS      448
#define NUM_PIXELS      16
#define NUM_STRIP       28
#define LED_BRIGHTNESS  100

#define HUE_CIRCLE 65536L

#define BASE_COLOR      0, 125, 127 //#007D7F
#define ERROR_COLOR   250, 219,  98 //#FFDDDD
#define SUCESS_COLOR  244, 246,   0 //#F4E200
#define FAILURE_COLOR 255,   0,   0 //#ff3300

class gameMode
{
    private:
    Adafruit_NeoPixel *stripGame;
    MPU6050 accelerator;
    int16_t base, receive;
    bool judgeDaigo = false;

    void startup();
    void showReset(int);
    void judge(int, bool, int);
    void sucess();
    void failure(int);
    int  randomSpeed();
    void delayTime(int time);

  public:
    int val;

    void setStrip(Adafruit_NeoPixel *strip);
    void previw(int);
    void constantVelocity(int);
    void constantVelocity(int, bool);
    void acceleration();
    void rainbowStrip();
    void rainbowStrip(int time);
    void rainbowStrip(int time, int multiple);
};

#endif