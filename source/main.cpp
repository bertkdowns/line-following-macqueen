#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

#define FOLLOW_LEFT -1
#define FOLLOW_FORWARD 0
#define FOLLOW_RIGHT 1

int follow_direction = FOLLOW_LEFT;

void followLeft(int offLeft, int offRight){
    // white on left side, black on right side
    if(offLeft && !offRight){
        //right motor slightly faster than left motor
        motorRun(Motors::Left, Dir::CW,0x30);
        motorRun(Motors::Right, Dir::CW,0x80);
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        // We have hit an intersection, turn left
        motorRun(Motors::Left, Dir::CW,0x0);
        motorRun(Motors::Right, Dir::CW,0x80);
        uBit.sleep(100);
        //follow_direction = FOLLOW_RIGHT;
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly left??
        motorRun(Motors::Right, Dir::CW,0x80);
        motorRun(Motors::Left, Dir::CW,0x60);
    }
    // both white
    else{
        // turn right
        motorRun(Motors::Right, Dir::CW,0x10);
        motorRun(Motors::Left, Dir::CW,0x80);
    }
}
void followRight(int offLeft, int offRight){
    // white on left side, black on right side
    if(offLeft && !offRight){
        // We have hit an intersection, turn right
        motorRun(Motors::Right, Dir::CW,0x0);
        motorRun(Motors::Left, Dir::CW,0x80);
        uBit.sleep(100);
        follow_direction = FOLLOW_LEFT;
        
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        //left motor slightly faster than right motor
        motorRun(Motors::Right, Dir::CW,0x30);
        motorRun(Motors::Left, Dir::CW,0x80);
        
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly right
        motorRun(Motors::Left, Dir::CW,0x80);
        motorRun(Motors::Right, Dir::CW,0x60);
    }
    // both white
    else{
        // turn left
        motorRun(Motors::Left, Dir::CW,0x10);
        motorRun(Motors::Right, Dir::CW,0x80);
    }
}
void followForward(int offLeft, int offRight){

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
        int distance = readUlt();
        while(distance < 5 && distance >= 0) //3 centimetres away
        {
            motorRun(Motors::All, Dir::CW, 0x0);
            uBit.sleep(1000);
            distance = readUlt();
        }
    }
    //Make LEDS flash

}

