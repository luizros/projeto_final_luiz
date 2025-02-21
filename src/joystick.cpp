#include "../include/joystick.h"

Joystick::Joystick(int x, int y, int button, int channelX, int channelY)
    : pinX(x), pinY(y), pinButton(button), adcChannelX(channelX), adcChannelY(channelY) {}

void Joystick::begin()
{
    adc_init();
    adc_gpio_init(pinX);
    adc_gpio_init(pinY);
    gpio_init(pinButton);
    gpio_set_dir(pinButton, GPIO_IN);
    gpio_pull_up(pinButton);
}

void Joystick::read(int &x, int &y)
{
    adc_select_input(adcChannelX);
    sleep_us(2);
    x = (adc_read() - 2048) * 100 / 2048;

    adc_select_input(adcChannelY);
    sleep_us(2);
    y = (adc_read() - 2048) * 100 / 2048;
}

bool Joystick::isPressed()
{
    return gpio_get(pinButton) == 0;
}


