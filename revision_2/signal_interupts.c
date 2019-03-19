/*
 * signal_interupts.c
 *
 *  Created on: Feb 20, 2019
 *      Author: rohan
 */

#include "signal_interupts.h"

// This functions sets up the signal interrupts
void initialize_signals()
{

    // Enable the Switch ports.
    SysCtlPeripheralEnable(SYS_TURN_PORT);
    while(!SysCtlPeripheralReady(SYS_TURN_PORT));

    //Unlocking Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x01; 

    //Configuring Pins as inputs
    GPIOPinTypeGPIOInput(TURN_PORT_BASE, LEFT_TURN_PIN|RIGHT_TURN_PIN);
    GPIOPadConfigSet(TURN_PORT_BASE, LEFT_TURN_PIN|RIGHT_TURN_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Configuring Interrupts
    GPIOIntRegister(TURN_PORT_BASE, buttons_ISR);
    GPIOIntTypeSet(TURN_PORT_BASE, LEFT_TURN_PIN|RIGHT_TURN_PIN , GPIO_FALLING_EDGE);
    GPIOIntEnable(TURN_PORT_BASE, LEFT_TURN_PIN|RIGHT_TURN_PIN);

    //Enabling the port for the stop Switch
    SysCtlPeripheralEnable(SYS_STOP_SWITCH_PORT);
    while(!SysCtlPeripheralReady(SYS_STOP_SWITCH_PORT));
                
    // Configuring the pins for the stop switch
    GPIOPinTypeGPIOInput(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN);
    GPIOPadConfigSet(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntRegister(STOP_SWITCH_PORT_BASE, stop_button_ISR);
    GPIOIntTypeSet(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN , GPIO_BOTH_EDGES);
    GPIOIntEnable(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN);

    //Setting up the timer
    SysCtlPeripheralEnable(SYS_TIMER_PORT);
    while(!SysCtlPeripheralReady(SYS_TIMER_PORT));

    TimerConfigure(TIMER_PORT_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, blink_interval*(SysCtlClockGet()/1000));

    // Set timer ISR
    TimerIntRegister(TIMER_PORT_BASE, TIMER_A, timer_ISR);
    TimerIntEnable(TIMER_PORT_BASE, TIMER_TIMA_TIMEOUT);


}


// this function is for turn switches

void buttons_ISR()
{
    // storing the interrupt status register
    uint32_t int_status = GPIOIntStatus(TURN_PORT_BASE, true);

    if  ( (int_status & LEFT_TURN_PIN) == LEFT_TURN_PIN)
    {
        if(status.left_flash == 0)
        {
            status.left_flash = 1;
            //start flashing
//            TimerLoadSet(TIMER0_BASE, TIMER_A, blink_interval*(SysCtlClockGet()/1000));
//            TimerEnable(TIMER0_BASE, TIMER_A);

        }

        else
        {
            status.left_flash = 0;
            // stop flashing
///            TimerDisable(TIMER0_BASE, TIMER_A);
            //stop the signal
            set_signal(LEFT,0);
        }

    }

     if((int_status & RIGHT_TURN_PIN) == RIGHT_TURN_PIN)
    {
            if(status.right_flash == 0)
            {
                status.right_flash = 1;
                //start flashing
//                TimerLoadSet(TIMER0_BASE, TIMER_A, blink_interval*(SysCtlClockGet()/1000));
//                TimerEnable(TIMER0_BASE, TIMER_A);
            }

            else
            {
                status.right_flash = 0;
                // stop flashing
//                TimerDisable(TIMER0_BASE, TIMER_A);
                //stop the signal
                set_signal(RIGHT,0);
            }

        }

     if(status.right_flash == 1 || status.left_flash ==1)
     {
         TimerLoadSet(TIMER0_BASE, TIMER_A, blink_interval*(SysCtlClockGet()/1000));
         TimerEnable(TIMER0_BASE, TIMER_A);
     }
     else{

          TimerDisable(TIMER0_BASE, TIMER_A);
     }

     GPIOIntClear(TURN_PORT_BASE, LEFT_TURN_PIN| RIGHT_TURN_PIN );

}




/* This function is for stop button
   It is triggered on both edges hence we use    
*/
void stop_button_ISR()
{
    GPIOIntClear(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN);
    // determine the state of the button
    // if button is pressed display the stop signal
    if(GPIOPinRead(STOP_SWITCH_PORT_BASE, STOP_SWITCH_PIN) == 0x00 )
    {
        set_signal(STOP, 1);
    }

    else{
        set_signal(STOP, 0);
    }


}


// this is for the timer
void timer_ISR()
{
    TimerIntClear(TIMER_PORT_BASE, TIMER_A);
    status.flash_status = !(status.flash_status);

    // If left turn signal is on
    if(status.left_flash)
    {
            set_signal(LEFT, status.flash_status);


    }

    if(status.right_flash)
    {

            set_signal(RIGHT, status.flash_status);
        }



}
