#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

#define FOLLOW_LEFT -1
#define FOLLOW_FORWARD 0
#define FOLLOW_RIGHT 1
#define MAX_SPEED 0x60

int follow_direction = FOLLOW_FORWARD;

void followLeft(int offLeft, int offRight){
    uBit.display.image.clear();
    uBit.display.image.setPixelValue(0,0,255);
    uBit.display.image.setPixelValue(0,1,255);
    uBit.display.image.setPixelValue(0,2,255);
    uBit.display.image.setPixelValue(0,3,255);
    uBit.display.image.setPixelValue(0,4,255);
    uBit.display.image.setPixelValue(1,4,255);
    uBit.display.image.setPixelValue(2,4,255);
    uBit.display.image.setPixelValue(3,4,255);
    uBit.display.image.setPixelValue(4,4,255);
    // white on left side, black on right side
    if(offLeft && !offRight){
        motorRun(Motors::Right, Dir::CW,0x0);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        // We have hit an intersection, turn left
        motorRun(Motors::Left, Dir::CW,0x0);
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        uBit.sleep(600);
        follow_direction = FOLLOW_RIGHT;
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly left??
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        motorRun(Motors::Left, Dir::CCW,0);
    }
    // both white
    else{
        // turn right
        motorRun(Motors::Right, Dir::CCW,MAX_SPEED);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }
}
void followRight(int offLeft, int offRight){
    uBit.display.image.clear();
    // white on left side, black on right side
    if(offLeft && !offRight){
        // We have hit an intersection, turn right
        motorRun(Motors::Right, Dir::CW,0x0);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
        uBit.sleep(600);
        follow_direction = FOLLOW_LEFT;
        
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        motorRun(Motors::Left, Dir::CW,0x0);
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly right
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
        motorRun(Motors::Right, Dir::CCW,0);
    }
    // both white
    else{
        // turn left
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        motorRun(Motors::Left, Dir::CCW,MAX_SPEED);

    }
}
void followForward(int offLeft, int offRight){
    // white on left side, black on right side
    if(offLeft && !offRight){
        motorRun(Motors::Right, Dir::CW,MAX_SPEED/2);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        // We have hit an intersection, Go straight
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        uBit.sleep(600);
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly left??
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        motorRun(Motors::Left, Dir::CCW,MAX_SPEED/2);
    }
    // both white
    else{
        // turn right
        motorRun(Motors::Right, Dir::CCW,MAX_SPEED/2);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }
    /*if(offLeft && !offRight)
    {
        motorRun(Motors::Left, Dir::CW,0x20);
        motorRun(Motors::Right, Dir::CW,0x0);
    }
    else if(!offLeft && offRight)
    {
        motorRun(Motors::Left, Dir::CW,0x0);
        motorRun(Motors::Right, Dir::CW,0x20);
    }
    else if(!offLeft && !offRight)
    {
        motorRun(Motors::All, Dir::CW,0x20);
    }
    else
    {
        motorRun(Motors::All, Dir::CCW,0x10);
    }*/
    /*if(readPatrol(Patrol::PatrolLeft))
        {
            writeLED(LED::LEDLeft, LEDswitch::turnOn);
            motorRun(Motors::Left, Dir::CW,0x80);
        }
        else
        {
            writeLED(LED::LEDLeft, LEDswitch::turnOff);
            motorRun(Motors::Left, Dir::CW,0x0);
        }
        if(readPatrol(Patrol::PatrolRight))
        {
            writeLED(LED::LEDRight, LEDswitch::turnOn);
            motorRun(Motors::Right, Dir::CW,0x80);
        }
        else
        {
            writeLED(LED::LEDRight, LEDswitch::turnOff);
            motorRun(Motors::Right, Dir::CW,0x0);
        }

        if(!readPatrol(Patrol::PatrolLeft) && !readPatrol(Patrol::PatrolRight))
        {
            motorRun(Motors::All, Dir::CW,0x80);
        }*/
        //uBit.sleep(20);
}


int main()
{
    uBit.init();

    //Motor Run is driving
    //CW means forward
    //0x80 means speed at 128 digital value
    
    //Sleep for 2 seconds
    uBit.sleep(2000);
    //0x0 means stop
    

    /// loop forever
    // if left patrol sensor is on, turn the left led on
    // if right senspor is on turn the right led on
    while(1)
    {
        // Read sesor Values (1 if on white, 0 if on black)
        bool offLeft =  readPatrol(Patrol::PatrolLeft) ;
        bool offRight = readPatrol(Patrol::PatrolRight);
        // Display sensor values on LEDs
        writeLED(LED::LEDLeft, offLeft? LEDswitch::turnOn : LEDswitch::turnOff);
        writeLED(LED::LEDRight, offRight? LEDswitch::turnOn : LEDswitch::turnOff);

        if(follow_direction == FOLLOW_LEFT) followLeft(offLeft,offRight);
        else if (follow_direction == FOLLOW_RIGHT) followRight(offLeft,offRight);
        else followForward(offLeft,offRight);
        
        

        uBit.sleep(20);
        /*int distance = readUlt();
        while(distance < 5 && distance >= 0) //3 centimetres away
        {
            motorRun(Motors::All, Dir::CW, 0x0);
            uBit.sleep(1000);
            distance = readUlt();
        }*/
    }
    //Make LEDS flash

}

