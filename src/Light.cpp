#include "Light.h"
#include <Adafruit_NeoPixel.h>

Light::Light(int led_pin, int led_count) : strip(led_count, led_pin, NEO_GRB + NEO_KHZ800)
{
    m_led_count = led_count;
    m_led_pin = led_pin;
    m_startTick = millis();

    strip.begin();
    strip.show();
    strip.setBrightness(50);
}

int Light::getCirculardistance(int my_pos, int target_pos, int count)
{
    int distance = target_pos - my_pos;
    if (std::abs(distance) > count / 2)
    {
        if (distance > 0)
        {
            target_pos = target_pos - count;
        }
        else
        {
            target_pos = target_pos + count;
        }
    }

    int res = target_pos - my_pos;
    return res;
}

float Light::getTailFactor(int distance, int tail_length, bool clockwise)
{
    if (distance == 0)
        return 1;

    if ((clockwise && distance < 0) || (!clockwise && distance > 0))
        return 0;

    float step = 1.0 / (float)tail_length;
    float result = step * ((float)tail_length - (float)std::abs(distance));
    if (result < 0)
        return 0;

    return result * (result / 2);
}

void Light::update(long tick)
{
    if (m_program == Program::none)
        return;

    switch (m_program)
    {
    case Program::nightrider:
        nightRider(750, 4, tick);
        break;
    case Program::police:
        police(750, 3, tick);
        break;
    default:
        break;
    }
}

void Light::runProgram(Program program)
{
    m_program = program;
}

void Light::nightRider(int speed, int tail_length, long tick)
{
    uint32_t color = strip.Color(255, 0, 0);
    drawTail(tail_length, getTimePos(speed, tick), color);
    strip.show();
}

void Light::police(int speed, int tail_length, long tick)
{
    uint32_t color = strip.Color(0, 0, 255);
    int pos = getTimePos(speed, tick);
    int offSet = pos + (m_led_count / 2);
    drawTail(tail_length, pos, color);
    drawTail(tail_length, offSet, color);
    strip.show();
}

int Light::getTimePos(int speed, long tick)
{
    int now = millis();
    int offset = now % speed;
    return ((float)offset / (float)speed) * (float)m_led_count;
}

void Light::drawTail(int tail_length, int pos, uint32_t color)
{
    for (int i = 0; i < m_led_count; i++)
    {
        int distance = getCirculardistance(i, pos, m_led_count);
        if (std::abs(distance) <= tail_length)
        {
            float factor = getTailFactor(distance, tail_length, true);            
            setPixelColorDimmed(i, color, factor);
        }
    }
}

void Light::setPixelColorDimmed(int pos, uint32_t color, float brightness)
{
    uint8_t
        r = (uint8_t)(color >> 16),
        g = (uint8_t)(color >> 8),
        b = (uint8_t)color;

    strip.setPixelColor(pos, (float)r * brightness, (float)g * brightness, (float)b * brightness);
}