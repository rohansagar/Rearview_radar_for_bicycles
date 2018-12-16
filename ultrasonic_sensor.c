/*****************************************************
 * using this library
 * From main call
 * uss_setup_pins() finction
 *
 * then call uss_measure_distance() function
 * the measure distance function returns a uint32_t
 *
 *****************************************************/

#include "ultrasonic_sensor.h"





//This is to avoid doing the math everytime you do a reading
const double temp = 1.0/80.0;

//Stores the pulse length
volatile uint32_t pulse_1 = 0 ;

//Stores the pulse length
volatile uint32_t pulse_2 = 0 ;

//Tells the main code if the a pulse is being read at the moment
volatile uint8_t echowait_1 =0;

volatile uint8_t echowait_2 =0;





/* Add this to startup.c
extern inputInt(void);
input_int, for the GPIO handler

*/


void uss_setup_pins_1()
{

    //Configures the timer
     ConfigureTimer_1();

     //setting up the Trigger pin
     SysCtlPeripheralEnable(sys_trigger_port_1);
     SysCtlDelay(3);
     GPIOPinTypeGPIOOutput(trigger_port_1, trigger_pin_1);

     //setting up the Echo pin
     SysCtlPeripheralEnable(sys_echo_port_1);
     SysCtlDelay(3);
     GPIOPinTypeGPIOInput(echo_port_1, echo_pin_1);
     GPIOIntEnable(echo_port_1, echo_pin_1);
     GPIOIntTypeSet(echo_port_1, echo_pin_1,GPIO_BOTH_EDGES);
     GPIOIntRegister(echo_port_1,echo_int_1);

}


uint32_t uss_measure_distance_1()
{

    //Checks if a pulse read is in progress
    if(echowait_1 != 1)
    {
    //Does the required pulse of 10uS
    GPIOPinWrite(trigger_port_1, trigger_pin_1, trigger_pin_1);
    SysCtlDelay(SysCtlClockGet()/3/100000);
    SysCtlDelay(3);
    GPIOPinWrite(trigger_port_1, trigger_pin_1, ~trigger_pin_1);


    while(echowait_1 != 0); // wait until the echo pin goes low

    //Converts the counter value to cm.
    pulse_1 =(uint32_t)(temp * pulse_1);
    pulse_1 = pulse_1 / 58;
    }
    return (pulse_1);

}


void echo_int_1()
{
    //Clear interrupt flag. Since we only enabled on this is enough
    GPIOIntClear(echo_port_1, echo_pin_1);

    /*
        If it's a rising edge then set he timer to 0
        It's in periodic mode so it was in some random value
     */
    if ( GPIOPinRead(echo_port_1, echo_pin_1) == echo_pin_1)
    {
        HWREG(TIMER1_BASE + TIMER_O_TAV) = 0; //Loads value 0 into the timer.
        TimerEnable(TIMER1_BASE,TIMER_A);
        echowait_1=1;
    }
    /*
    If it's a falling edge that was detected, then get the value of the counter
     */
    else
    {
        pulse_1 = TimerValueGet(TIMER1_BASE,TIMER_A); //record value
        TimerDisable(TIMER1_BASE,TIMER_A);
        echowait_1=0;
    }

}


void ConfigureTimer_1()
{
  /*
    Set the timer to be periodic.
  */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  SysCtlDelay(3);
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC_UP);
  TimerEnable(TIMER1_BASE,TIMER_A);
}




//
//
//int main(){
//
//    uss_setup_pins();
//    uint32_t dist = measure_distance();
//}







void uss_setup_pins_2()
{

    //Configures the timer
     ConfigureTimer_2();

     //setting up the Trigger pin
     SysCtlPeripheralEnable(sys_trigger_port_2);
     SysCtlDelay(3);
     GPIOPinTypeGPIOOutput(trigger_port_2, trigger_pin_2);

     //setting up the Echo pin
     SysCtlPeripheralEnable(sys_echo_port_2);
     SysCtlDelay(3);
     GPIOPinTypeGPIOInput(echo_port_2, echo_pin_2);
     GPIOIntEnable(echo_port_2, echo_pin_2);
     GPIOIntTypeSet(echo_port_2, echo_pin_2,GPIO_BOTH_EDGES);
     GPIOIntRegister(echo_port_2,echo_int_2);

}


uint32_t uss_measure_distance_2()
{

    //Checks if a pulse read is in progress
    if(echowait_2 != 1)
    {
    //Does the required pulse of 10uS
    GPIOPinWrite(trigger_port_2, trigger_pin_2, trigger_pin_2);
    SysCtlDelay(SysCtlClockGet()/3/100000);
    SysCtlDelay(3);
    GPIOPinWrite(trigger_port_2, trigger_pin_2, ~trigger_pin_2);


    while(echowait_2 != 0); // wait until the echo pin goes low

    //Converts the counter value to cm.
    pulse_2 =(uint32_t)(temp * pulse_2);
    pulse_2 = pulse_2 / 58;
    }
    return (pulse_2);

}


void echo_int_2()
{
    //Clear interrupt flag. Since we only enabled on this is enough
    GPIOIntClear(echo_port_2, echo_pin_2);

    /*
        If it's a rising edge then set he timer to 0
        It's in periodic mode so it was in some random value
     */
    if ( GPIOPinRead(echo_port_2, echo_pin_2) == echo_pin_2)
    {
        HWREG(TIMER2_BASE + TIMER_O_TAV) = 0; //Loads value 0 into the timer.
        TimerEnable(TIMER2_BASE,TIMER_A);
        echowait_2=1;
    }
    /*
    If it's a falling edge that was detected, then get the value of the counter
     */
    else
    {
        pulse_2 = TimerValueGet(TIMER2_BASE,TIMER_A); //record value
        TimerDisable(TIMER2_BASE,TIMER_A);
        echowait_2=0;
    }

}


void ConfigureTimer_2()
{
  /*
    Set the timer to be periodic.
  */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  SysCtlDelay(3);
  TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
  TimerEnable(TIMER2_BASE,TIMER_A);
}




