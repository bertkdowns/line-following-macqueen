#include "MicroBit.h"
#include <cmath>
//#include "<string>"
extern MicroBit uBit; // extern, because it is defined in main.cpp
//MicroBitI2C i2c(I2C_SDA0, I2C_SCL0);

#define MOTOR_ADDRESS 0x20 // device address is 0x10 but must be left shifted for Micro:bit libraries.
typedef struct CompassCooordinate {
    int x;
    int y;
} COORD;
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
    LEDLeft = 8,//P8
    LEDRight = 12//P12
};

enum class LEDswitch {
    turnOn = 0x01, //On
    turnOff = 0x00 //Off
};

void calibrateCompass(){
    // load in calibration values from flash, or freshly calibrate it.
    CompassCalibration calibration;
    KeyValuePair* calibration_location = uBit.storage.get("calib");
    if(calibration_location == NULL)
    {
        //this is the first boot after flash, or no calibration is stored yet
        uBit.compass.calibrate(); //Calibrate the compass
        calibration = uBit.compass.getCalibration();
        // store the calibration in flash
        uBit.storage.put("calib", (uint8_t*) &calibration, sizeof(CompassCalibration));
    }
    else
    {
        //this is not the first boot, copy the stored value
        memcpy(&calibration, calibration_location->value, sizeof(CompassCalibration));
        // free the memory from the heap
        delete calibration_location;
        // load the calibration
        uBit.compass.setCalibration(calibration);
    }
}

COORD readCompass(){
    COORD coords; // declare coordinates on stack
    coords.x = uBit.compass.getX(); 
    coords.y = uBit.compass.getZ(); // because microbit is upright
    return coords;
}

COORD rotateCoordinate(COORD input, int degrees){
    float radians = degrees/57.29;
    // rotates a coordinate by a number of degrees.
    COORD coords;
    coords.x = cos(radians * input.x) - sin(radians * input.y);
    coords.y = sin(radians * input.x) + cos(radians * input.y);
    return coords;
}
COORD normaliseCoordinate(COORD input){
    // normalise the coordinate to be a value in the circle of radius 1024
    
    // calculate the magnitude of the vector
    float magnitude = sqrt(input.x*input.x + input.y*input.y);
    // calculate the scale factor (how much we have to scale it for the magnitude to be 1024)
    float scaleFactor = 1024/magnitude;
    // scale x * y by that amount
    input.x = input.x * scaleFactor;
    input.y = input.y * scaleFactor;
    return input;
    /*
    Two normalised coordinates will be a maximum of 2048 units apart. (i.e on the opposite sides of the circle) 
    */
}
int closeness(COORD a, COORD b){ // returns how close two coordinates are as a percentage.
    a = normaliseCoordinate(a);
    b = normaliseCoordinate(b);
    // calculate the distance between the normalised coordinates
    int distance = sqrt(((a.x-b.x)^2) + ((a.y-b.y)^2)); 
    // this will be a value between 0 and 2048. scale it to 100 so that it's easier
    return distance/(2048/100);
}

void motorRun(Motors index, Dir direction, uint8_t speed) {
    uint8_t buf[3]; //3 bytes
    buf[1] =  (direction == Dir::CW) ? 0 : 1; // 0 forward,  1 backward
    buf[2] = speed; //Write speed
    if (index == Motors::Left || index == Motors::All) {
        buf[0] = 0x00; //Left
        uBit.i2c.write(MOTOR_ADDRESS, buf, 3); //Write to Motor Address
    }
    if (index == Motors::Right || index == Motors::All) {
        buf[0] = 0x02; //Right
        uBit.i2c.write(MOTOR_ADDRESS, buf, 3); //Write to Motor Address
    }
}

//Checks whether the sensor is reading black or right
int readPatrol(Patrol patrol) {
    if (patrol == Patrol::PatrolLeft) {
        return uBit.io.P13.getDigitalValue(); //P13
    }
    else if (patrol == Patrol::PatrolRight) {
        return uBit.io.P14.getDigitalValue(); //P14
    }
    else {
        return -1; //No patrols being read
    }
}

//Write Leds
void writeLED(LED led, LEDswitch ledswitch) {
    if (led == LED::LEDLeft) {
        uBit.io.P8.setDigitalValue( (int) ledswitch); //P8
    }
    else if (led == LED::LEDRight) {
        uBit.io.P12.setDigitalValue( (int) ledswitch); //P12
    }
}

//Read Ultrasonic
int readUlt(){
        int d = 0; //Initalise d
        uBit.io.P1.setDigitalValue(1); //P1 to 1
    
        uBit.sleep(1);
        uBit.io.P1.setDigitalValue(0); 
        if (uBit.io.P2.getDigitalValue() == 0) { //Read P2
            uBit.io.P1.setDigitalValue(0);
            uBit.io.P1.setDigitalValue(1);
            uBit.sleep(20); //Sleep for 20 milliseconds
            uBit.io.P1.setDigitalValue(0);
            d = uBit.io.P2.getPulseUs(500*58); //Get pulse times by 58 from P2
        } else {
            uBit.io.P1.setDigitalValue(1);
            uBit.io.P1.setDigitalValue(0);
            uBit.sleep(20);
            uBit.io.P1.setDigitalValue(0);
            d = uBit.io.P2.getPulseUs(500*58);
        }
        int x = d / 59; //Divide by 59
        return x;//In centimeters
    }