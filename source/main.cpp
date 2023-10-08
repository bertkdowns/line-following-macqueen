#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit; //Define Microbit
//Define constants
#define FOLLOW_LEFT -1 //Left
#define FOLLOW_FORWARD 0 //Center
#define FOLLOW_RIGHT 1 //Right
#define MAX_SPEED 0x60 //96 for max speed
#define FOLLOW_MAZE 1 //Follow Maze

int follow_direction = FOLLOW_LEFT; //Set to follow left
Image smiley("0,255,0,255, 0\n0,255,0,255,0\n0,0,0,0,0\n255,0,0,0,255\n0,255,255,255,0\n");  //Pixels for smiley face
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

//If the location between A and B are between precision defined
bool nearby(int locationA,int locationB, int maxDegrees){
    return (distanceBetweenHeadings(locationA,locationB) < maxDegrees);
}

void delayUntil(int targetHeading, int precision, int minDelay, int maxDelay){
    // delays until you reach a specific heading, or times out at max delay.
    // This allows us to compare the compass value with the expected changes from turning the wheels
    // because we know roughly how long we need to delay for the robot to turn.
    uBit.sleep(minDelay); //Sleep for minimum delay
    int totalDelay = minDelay; //Set totalDelay counter
    bool isNearby = false; //Set is nearby to flase
    uBit.display.image.clear();
    uBit.display.print(smiley);
    while(!isNearby && totalDelay < maxDelay){ //Nearby or too long stops rotating
        //Read the compass heading and precision
        isNearby = nearby(targetHeading,uBit.compass.heading(),precision); //Precision between two locations
        uBit.sleep(5); //Sleep for 5 millisceonds
        totalDelay += 5; //Add 5 to delay for pausing for 5 millisceonds
    }
    uBit.display.image.clear(); //Clear the smiley face
}

//Follow left
void followLeft(int offLeft, int offRight){
    uBit.display.image.clear(); //Clear before drawing
    //Draw the letter L on the Microbit
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
        motorRun(Motors::Right, Dir::CW,MAX_SPEED/4); //Turn right since left motor is faster
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        // We have hit an intersection, turn left
        motorRun(Motors::Left, Dir::CW,0x0);
        motorRun(Motors::Right, Dir::CW,MAX_SPEED);
        // keep turning till you've turned 90 degrees
        int val = uBit.compass.heading();
        // subtract 90 degrees to get target value for turning left (this is more than 90 degrees because the compass has low prescision.)
        int target = (val + 270) % 360;
        delayUntil(target,30,600,1000); //Delay for rotation
        //uBit.sleep(575);
        follow_direction = FOLLOW_RIGHT; //Follow right
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
        // add 90 degrees to get target value for turning right
        int target = (val + 90) % 360;
        delayUntil(target,30,600,1000); //Delay until rotation is complete
        //uBit.sleep(575);
        follow_direction = FOLLOW_LEFT; //Follow left
        
    }
    // black on left side, white on right side
    else if(!offLeft && offRight){
        motorRun(Motors::Left, Dir::CW,MAX_SPEED/4);
        motorRun(Motors::Right, Dir::CW,MAX_SPEED); //Turn left
        
    }
    // both black
    else if(!offLeft && !offRight){
        // turn slightly right
        motorRun(Motors::Left, Dir::CW,MAX_SPEED);
        motorRun(Motors::Right, Dir::CCW,MAX_SPEED/2);
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
    uBit.init(); //Initalise the microbit
    uBit.compass.calibrate(); //Calibrate the compass
    //Motor Run is driving
    //CW means forward
    //0x80 means speed at 128 digital value
    
    //Sleep for 1 seconds
    //uBit.sleep(1000);
    //0x0 means stop
    

    /// loop forever
    // if left patrol sensor is on, turn the left led on
    // if right senspor is on turn the right led on
    int readUltrasonic = 40;  //Set ultrasonic counter to 40
    while(1)
    {
        // Read sesor Values (1 if on white, 0 if on black)
        bool offLeft =  readPatrol(Patrol::PatrolLeft) ;
        bool offRight = readPatrol(Patrol::PatrolRight);
        // Display sensor values on LEDs
        writeLED(LED::LEDLeft, offLeft? LEDswitch::turnOn : LEDswitch::turnOff);
        writeLED(LED::LEDRight, offRight? LEDswitch::turnOn : LEDswitch::turnOff);

        if(FOLLOW_MAZE) followLeft(offLeft,offRight); //Follow maze designed
        else if(follow_direction == FOLLOW_LEFT) followLeft(offLeft,offRight);
        else if (follow_direction == FOLLOW_RIGHT) followRight(offLeft,offRight);
        else followForward(offLeft,offRight);
        
        

        uBit.sleep(10); //Sleep 10 milliseconds
        // only read ultrasonic sensor once every 40 iterations
        readUltrasonic--;
        if(readUltrasonic == 0){
            readUltrasonic = 40; //Reset counter
            int distance = readUlt(); //If close to a wall
            while(distance < 8 && distance >= 0) //8 centimetres away
            {
                motorRun(Motors::All, Dir::CW, 0x0); //Stop
                uBit.sleep(500); //Wait 500 milliseconds
                distance = readUlt(); //Read again
            }
        }
    }
    //Make LEDS flash

}

