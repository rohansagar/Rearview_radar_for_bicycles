#include "blindSpot.h"

void setup(){
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

    setupButtons();
    setupLights();
}

void loop(){

}

int main(void) {

    setup();
    while(1) loop();
}
