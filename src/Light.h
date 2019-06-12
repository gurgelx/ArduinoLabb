#ifndef Light_h
#define Light_h
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

enum class Program {none, nightrider, psilon, police};

class Light
{
    public:
        Light(int led_pin, int led_count);
        void runProgram(Program program);
        void update(long tick);


    private:
        static int getCirculardistance(int my_pos, int target_pos, int count);
        static float getTailFactor(int distance, int tail_length, bool clockwise = true);
        void nightRider(int speed, int tail_length, long tick);
        int m_led_pin;
        int m_led_count;
        Program m_program = Program::none;
        long m_startTick;
        Adafruit_NeoPixel strip;
};

#endif