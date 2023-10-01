#include "MicroBit.h"
//#include "<string>"
extern MicroBit uBit; // extern, because it is defined in main.cpp
//MicroBitI2C i2c(I2C_SDA0, I2C_SCL0);

#define MOTOR_ADDRESS 0x20 // device address is 0x10 but must be left shifted for Micro:bit libraries.

enum class Motors {
    Left,
    Right,
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

void motorRun(Motors index, Dir direction, uint8_t speed) {
    uint8_t buf[3];
    buf[1] =  (direction == Dir::CW) ? 0 : 1; // 0 forward,  1 backward
    buf[2] = speed;
    if (index == Motors::Left || index == Motors::All) {
        buf[0] = 0x00;
        uBit.i2c.write(MOTOR_ADDRESS, buf, 3);
    }
    if (index == Motors::Right || index == Motors::All) {
        buf[0] = 0x02;
        uBit.i2c.write(MOTOR_ADDRESS, buf, 3);
    }
}

int readPatrol(Patrol patrol) {
    if (patrol == Patrol::PatrolLeft) {
        return uBit.io.P13.getDigitalValue();
    }
    else if (patrol == Patrol::PatrolRight) {
        return uBit.io.P14.getDigitalValue();
    }
    else {
        return -1;
    }
}

void writeLED(LED led, LEDswitch ledswitch) {
    if (led == LED::LEDLeft) {
        uBit.io.P8.setDigitalValue( (int) ledswitch);
    }
    else if (led == LED::LEDRight) {
        uBit.io.P12.setDigitalValue( (int) ledswitch);
    }
}

int readUlt(){
        int d = 0;
        uBit.io.P1.setDigitalValue(1);
    
        uBit.sleep(1);
        uBit.io.P1.setDigitalValue(0);
        if (uBit.io.P2.getDigitalValue() == 0) {
            uBit.io.P1.setDigitalValue(0);
            uBit.io.P1.setDigitalValue(1);
            uBit.sleep(20);
            uBit.io.P1.setDigitalValue(0);
            d = uBit.io.P2.getPulseUs(500*58);//pins.pulseIn(DigitalPin.P2, PulseValue.High, 500 * 58);//readPulseIn(1);
        } else {
            uBit.io.P1.setDigitalValue(1);
            uBit.io.P1.setDigitalValue(0);
            uBit.sleep(20);
            uBit.io.P1.setDigitalValue(0);
            d = uBit.io.P2.getPulseUs(500*58);//readPulseIn(0);
        }
        int x = d / 59;
        return x;//In centimeters
    }