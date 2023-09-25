#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

int main()
{
    uBit.init();

    //Motor Run is driving
    //CW means forward
    //0x80 means speed at 128 digital value
    motorRun(Motors::All, Dir::CW,0x80);
    //Sleep for 2 seconds
    uBit.sleep(2000);
    //0x0 means stop
    motorRun(Motors::All, Dir::CW,0x0);
    //Make LEDS flash
    /*writeLED(LED::LEDLeft, LEDswitch::turnOn);
    writeLED(LED::LEDRight, LEDswitch:: turnOn);
    uBit.sleep(2000);
    writeLED(LED::LEDLeft, LEDswitch::turnOff);
    writeLED(LED::LEDRight, LEDswitch:: turnOff);*/
}

