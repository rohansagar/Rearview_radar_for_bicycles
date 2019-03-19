/*This program is used to test the LED matirx display*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"

#include "pin_assignments.h"
#include "led_matrix.h"
#include "signal_interupts.h"
#include "ultrasonic_sensor.h"



void set_interrupt_priorities();


/*
 * test example for led matrix
 */


void main(void)
{
    // set system clock to 80 mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    // wait for like half a second
    SysCtlDelay(SysCtlClockGet()/3/2);
    // initialize the LED Matrix
    initialize_matrix(0xff);
    // initialize the signals
    initialize_signals();

    // setting up blindspot indicator leds
        setup_blind_spot_leds();
        // setup ultrasonic sensor pins. These pins are defined in ultrasonic_sensor.h
        uss_setup_pins_1();
        uss_setup_pins_2();
        SysCtlDelay(10);
       // enable_uss_trigger_timer();


        // Optional for Debugging
        //ConfigureUART();
  //     set_interrupt_priorities();




    // display something
    while(1)
    {
        checkBlindSpot();
        SysCtlDelay(SysCtlClockGet()/1000/3); // 1 ms delay



    }

}






