#include "MicroBit.h"
#include "macqueen.h"

MicroBit uBit;

int main()
{
    uBit.init();

    motorRun(Motors::All, Dir::CW,0x80);

    uBit.sleep(2000);

    motorRun(Motors::All, Dir::CW,0x0);

}

