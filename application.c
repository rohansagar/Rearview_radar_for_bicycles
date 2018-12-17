#include "application.h"


/*
@info: this function sets up all the necessary pins
*/
void setup()
{
    // Setup the system clock to run at 80 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    setupButtons();
    setupLights();
    setupSleep();
    //setupHibernation();


    set_interrupt_priorities();

    // setting up blindspot indicator leds
    setup_blind_spot_leds();
    // setup ultrasonic sensor pins. These pins are defined in ultrasonic_sensor.h 
    uss_setup_pins_1();
    uss_setup_pins_2();
    enable_uss_trigger_timer();

    // Optional for Debugging 
    ConfigureUART();

}

int main(void) 
{
    /*
    if(HibernateIsActive()){
        HibernateDisable();
    }
    else {
        setup();
    }*/

    setup();
    while(1)
    {
        //timer4ISR();
        //SysCtlSleep();

    }
}
