#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

#define FOLLOW_LEFT -1
#define FOLLOW_FORWARD 0
#define FOLLOW_RIGHT 1
#define MAX_SPEED 0x60

int follow_direction = FOLLOW_LEFT;
Image smiley("0,255,0,255, 0\n0,255,0,255,0\n0,0,0,0,0\n255,0,0,0,255\n0,255,255,255,0\n");
/*Compass methods*/
int distanceBetweenHeadings(int locationA, int locationB){
    // Calculates the distance between two headings, up to a maximum of 180 degrees
    int distance1 = abs(locationA-locationB);
    // This distance could be ridiculiously large (e.g if location A is 0* and locationB is 360*), so add 180 degrees and mod 360
    int distance2 = abs(
        ((locationA + 180) % 360) 
        -((locationB + 180) % 360)
        );
    // return the smallest of the two
    return distance1 < distance2 ? distance1 : distance2;
}
bool nearby(int locationA,int locationB, int maxDegrees){
    return (distanceBetweenHeadings(locationA,locationB) < maxDegrees);
}

void delayUntil(int targetHeading, int precision, int minDelay, int maxDelay){
    // delays until you reach a specific heading, or times out at max delay.
    // This allows us to compare the compass value with the expected changes from turning the wheels
    // because we know roughly how long we need to delay for the robot to turn.
    uBit.sleep(minDelay);
    int totalDelay = minDelay;
    bool isNearby = false;
    uBit.display.image.clear();
    uBit.display.print(smiley);
    while(!isNearby && totalDelay < maxDelay){
        // Read the compass value 5 times
        /*int sum = 0;
        sum += distanceBetweenHeadings(targetHeading,uBit.compass.heading());
        uBit.sleep(5);
        sum += distanceBetweenHeadings(targetHeading,uBit.compass.heading());
        uBit.sleep(5);
        sum += distanceBetweenHeadings(targetHeading,uBit.compass.heading());
        uBit.sleep(5);
        sum += distanceBetweenHeadings(targetHeading,uBit.compass.heading());
        uBit.sleep(5);
        sum += distanceBetweenHeadings(targetHeading,uBit.compass.heading());
        uBit.sleep(5);
        // average distance away must be less than the target prescision.
        isNearby = sum < (precision * 5);*/
        isNearby = nearby(targetHeading,uBit.compass.heading(),precision);
        uBit.sleep(5);
        totalDelay += 5;
    }
    uBit.display.image.clear();
}



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
        // keep turning till you've turned 90 degrees
        int val = uBit.compass.heading();
        // subtract 130 degrees to get target value for turning left (this is more than 90 degrees because the compass has low prescision.)
        int target = (val + 230) % 360;
        delayUntil(target,30,400,1000);
        //uBit.sleep(575);
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
        // keep turning till you've turned 90 degrees
        int val = uBit.compass.heading();
        // add 130 degrees to get target value for turning right
        int target = (val + 130) % 360;
        delayUntil(target,30,400,1000);
        //uBit.sleep(575);
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
        motorRun(Motors::Right, Dir::CW,MAX_SPEED/4);
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
        motorRun(Motors::Right, Dir::CCW,MAX_SPEED/4);
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }

}


int main()
{
    uBit.init();
    uBit.compass.calibrate();
    //Motor Run is driving
    //CW means forward
    //0x80 means speed at 128 digital value
    
    //Sleep for 1 seconds
    //uBit.sleep(1000);
    //0x0 means stop
    

    /// loop forever
    // if left patrol sensor is on, turn the left led on
    // if right senspor is on turn the right led on
    int readUltrasonic = 20; 
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
        // only read ultrasonic sensor once every 20 iterations
        readUltrasonic--;
        if(readUltrasonic == 0){
            readUltrasonic = 20;
            int distance = readUlt();
            while(distance < 8 && distance >= 0) //3 centimetres away
            {
                motorRun(Motors::All, Dir::CW, 0x0);
                uBit.sleep(500);
                distance = readUlt();
            }
        }
    }
    //Make LEDS flash

}

