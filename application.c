#include "application.h"

void setup()
{
    // Setup the system clock to run at 80 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

    setupButtons();
    setupLights();
    setupSleep();
    setupHibernation();

    // setting up blindspot indicator leds
    setup_blind_spot_leds();
    // setup ultrasonic sensor pins. These pins are defined in ultrasonic_sensor.h 
    uss_setup_pins_1();
    uss_setup_pins_2();
    enable_uss_trigger_timer();

    // Optional for Debugging 
    ConfigureUART();
    set_interrupt_priorities();

}

int main(void) 
{
    // If the hibernation clock is running, then we woke up from hibernation
    // No need to set everything up again in that case
    if(HibernateIsActive()){
        HibernateDisable();
        TimerEnable(TIMER4_BASE,TIMER_A);
    }
    else {
        setup();
    }

    while(1)
    {
        SysCtlSleep();
    }
}

void set_interrupt_priorities(){
//    Priority table: (from highest to lowest)
//    0, 1: USS Echo
//    2, 3: N/A
//    4, 5: Brake Signals
//    6, 7: Turn signal on / off
//    8, 9: USS check
//    A, B: Wheel sensor
//    C, D: Turn signal flash
//    E, F: Hibernation

    // USS1 USS2 echo
    IntPrioritySet(INT_GPIOB, 0X00);
    IntPrioritySet(INT_GPIOD, 0X00);

    // Brake
    IntPrioritySet(INT_GPIOE, 0x40);

    // Turn Signal buttons
    IntPrioritySet(INT_GPIOF, 0x60);

    // USS Check
    IntPrioritySet(INT_TIMER4A, 0x80);

    // Wheel Sensor
    IntPrioritySet(INT_GPIOA, 0xC0);

    // Turn signal flash
    IntPrioritySet(INT_TIMER0A, 0xA0);

    // Hibernation timer
    IntPrioritySet(INT_TIMER3A, 0xF0);
}
