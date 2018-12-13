#include "main.h"


void setup(){
    // Setup the system clock to run at 80 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    setupButtons();
    setupLights();
    setupSleep();
    setupHibernation();
}

void loop(){

    checkBlindSpot();
    // If none of the turn signals are on, then go into deep sleep
    // We use normal sleep because the turn signals rely on a timer that
    //    gets messed up in deep sleep mode
    // SSI function is currently blocking so we don't have to worry about that
    if(signalOn) {
        SysCtlSleep();
    }
    else {
        // Set the count time to 10 seconds
        // 30000 is the Khz of the int oscillator being used by the deep sleep module
        //TimerLoadSet(TIMER1_BASE, TIMER_A, 10*30000);
        //TimerEnable(TIMER1_BASE, TIMER_A);

        //SysCtlDeepSleep();
    }
}

int main(void) {
    if(HibernateIsActive()){
        HibernateDisable();
    }
    else {
        setup();
    }

    while(1) loop();
}
