#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

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
    int turnLeft = 1;
    // if left patrol sensor is on, turn the left led on
    // if right senspor is on turn the right led on
    while(1)
    {
        // white on left side, black on right side
        if(readPatrol(Patrol::PatrolLeft) && !readPatrol(Patrol::PatrolRight))
        {
            //right motor slightly faster than left motor
            writeLED(LED::LEDLeft, LEDswitch::turnOn);
            motorRun(Motors::Left, Dir::CW,0x30);
            writeLED(LED::LEDRight, LEDswitch::turnOff);
            motorRun(Motors::Right, Dir::CW,0x80);
        }
        // black on left side, white on right side
        else if(!readPatrol(Patrol::PatrolLeft) && readPatrol(Patrol::PatrolRight))
        {
            // We have hit an intersection, turn left
            writeLED(LED::LEDLeft, LEDswitch::turnOff);
            motorRun(Motors::Left, Dir::CW,0x0);
            writeLED(LED::LEDRight, LEDswitch::turnOn);
            motorRun(Motors::Right, Dir::CW,0x80);
        }
        // both black
        else if(!readPatrol(Patrol::PatrolLeft) && !readPatrol(Patrol::PatrolRight))
        {
            // turn slightly left??
            motorRun(Motors::Right, Dir::CW,0x80);
            motorRun(Motors::Left, Dir::CW,0x60);
            writeLED(LED::LEDLeft, LEDswitch::turnOff);
            writeLED(LED::LEDRight, LEDswitch::turnOff);
        }
        // both white
        else
        {
            // turn right
            motorRun(Motors::Right, Dir::CW,0x10);
            motorRun(Motors::Left, Dir::CW,0x80);
            writeLED(LED::LEDLeft, LEDswitch::turnOn);
            writeLED(LED::LEDRight, LEDswitch::turnOn);

            /*
            motorRun(Motors::All, Dir::CW,0x0);
            if(turnLeft)
            {
                motorRun(Motors::Right, Dir::CW,0x80);
                uBit.sleep(100);
                turnLeft = 0;
            }
            else
            {
                motorRun(Motors::Left, Dir::CW,0x80);
                uBit.sleep(100);
                turnLeft = 1;
            }
            
            */
        }
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

