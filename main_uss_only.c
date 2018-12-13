/*
 * mian.c
 *
 *  Created on: Dec 13, 2018
 *      Author: rohan
 */



#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/ssi.h"
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"

#include"driverlib/uart.h"
#include"ultrasonic_sensor.h"


void
ConfigureUART(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}
uint32_t distance_1;
uint32_t distance_2;


int main(){



        ROM_FPULazyStackingEnable();
        ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ |
                           SYSCTL_OSC_MAIN);
        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
        ConfigureUART();

        uss_setup_pins_1();
        uss_setup_pins_2();
        while(1){


        distance_1 = uss_measure_distance_1();
        distance_2 = uss_measure_distance_2();

        //UARTprintf("distance1 = %2dcm \n" , distance_1);
        UARTprintf("distance2= %2dcm \n" , distance_2);

        SysCtlDelay(SysCtlClockGet()/3/5);
        }

}