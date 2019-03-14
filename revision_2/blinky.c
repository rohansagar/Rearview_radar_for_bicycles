/*This program is used to test the LED matirx display*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"

#include "sensorlib/i2cm_drv.h"
#include "sensorlib/hw_mpu6050.h"
#include "sensorlib/mpu6050.h"




#include "led_matrix.h"
#include "signal_interupts.h"


/*
 * test example for led matrix
 */


void main(void){
    // set system clock to 80 mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    // wait for like half a second
    SysCtlDelay(SysCtlClockGet()/3/2);
    // initialize the LED Matrix
    initialize_matrix(0xff);
    // initialize the signals
    initialize_signals();
    // display something
    while(1)
    {

//         set_signal(LEFT,1);
//        SysCtlDelay(SysCtlClockGet()/3/4);
//        set_signal(LEFT,0);
//        SysCtlDelay(SysCtlClockGet()/3/4);
//
//
//        set_signal(STOP,1);
//        SysCtlDelay(SysCtlClockGet()/3/4);
//        set_signal(STOP,0);
//        SysCtlDelay(SysCtlClockGet()/3/4);
//
//        set_signal(RIGHT, 1);
//        SysCtlDelay(SysCtlClockGet()/3/4);
//        set_signal(RIGHT, 0);
//        SysCtlDelay(SysCtlClockGet()/3/4);


    }

}





