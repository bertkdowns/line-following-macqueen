#ifndef MB_NEXT_TESTS_H
#define MB_NEXT_TESTS_H
#include "MicroBit.h"

enum class Motors {
    M1,
    M2,
    All
};

enum class Dir {
    CW,
    CCW
};

enum class Patrol {
    PatrolLeft,
    PatrolRight
};

enum class LED {
    LEDLeft = 8,
    LEDRight = 12
};

enum class LEDswitch {
    turnOn = 0x01,
    turnOff = 0x00
};

void motorRun(Motors index, Dir direction, uint8_t speed);

int readPatrol(Patrol patrol) ;

void writeLED(LED led, LEDswitch ledswitch);

#endif