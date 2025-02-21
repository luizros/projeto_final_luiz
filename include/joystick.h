#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hardware/adc.h"
#include "pico/stdlib.h"
#include <stdio.h>

class Joystick
{
private:
    int pinX, pinY, pinButton;
    int adcChannelX, adcChannelY;

public:
    Joystick(int x, int y, int button, int channelX, int channelY);
    void begin();
    void read(int &x, int &y);
    bool isPressed();
};

#endif
