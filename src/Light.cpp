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
    if(std::abs(distance) > count / 2){
        if(distance > 0){
            target_pos = target_pos - count;
        }else{
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

void Light::update(long tick){
    if (m_program == Program::none)
        return;    

    switch (m_program)
    {
    case Program::nightrider :
        nightRider(750, 4, tick);        
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
    int now = millis();

    int offset = now % speed;
    int offsetPos = ((float)offset / (float)speed) * (float)m_led_count;

    for (int i = 0; i < m_led_count; i++)
    {
        int distance = getCirculardistance(i, offsetPos, m_led_count);
        float factor = getTailFactor(distance, tail_length, true);
        int red = 255.0 * factor;

        strip.setPixelColor(i, red, 0, 0);
    }
    strip.show();
}