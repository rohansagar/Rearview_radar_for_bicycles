#include "ultrasonicSensor.h"

//This is to avoid doing the math everytime you do a reading
const double temp = 1.0/80.0;

//Stores the pulse length
volatile uint32_t pulse=0;

//Tells the main code if the a pulse is being read at the moment
volatile uint8_t echowait=0;

void setupUltrasonic(){
    //Set system clock to 80Mhz
    // Current is running at 50, should it be 80?
    //SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
    ConfigureUART();
    //Configures the timer
    ConfigureTimer();

    //setting up the Trigger pin
    SysCtlPeripheralEnable(sys_trigger_port);
    SysCtlDelay(3);
    GPIOPinTypeGPIOOutput(trigger_port, trigger_pin);

    //setting up the Echo pin
    SysCtlPeripheralEnable(sys_echo_port);
    SysCtlDelay(3);
    GPIOPinTypeGPIOInput(echo_port, echo_pin);
    GPIOIntEnable(echo_port, echo_pin);
    GPIOIntTypeSet(echo_port, echo_pin,GPIO_BOTH_EDGES);
    GPIOIntRegister(echo_port,inputInt);
}

void checkBlindSpot(){

    //Checks if a pulse read is in progress
    if(echowait != 1){
      //Does the required pulse of 10uS
      GPIOPinWrite(trigger_port, trigger_pin, trigger_pin);
      SysCtlDelay(266);
      GPIOPinWrite(trigger_port, trigger_pin, ~trigger_pin);


      while(echowait != 0); // wait until the echo pin goes low

      //Converts the counter value to cm.
      pulse =(uint32_t)(temp * pulse);
      pulse = pulse / 58;

      //Prints out the distance measured.
      //UARTprintf("distance = %2dcm \n" , pulse);
    }
      //wait about 10ms until the next reading.
      SysCtlDelay(400000);


}

void inputInt(){
  //Clear interrupt flag. Since we only enabled on this is enough
  GPIOIntClear(echo_port, echo_pin);

  /*
    If it's a rising edge then set he timer to 0
    It's in periodic mode so it was in some random value
  */
  if ( GPIOPinRead(echo_port, echo_pin) == echo_pin){
    HWREG(TIMER2_BASE + TIMER_O_TAV) = 0; //Loads value 0 into the timer.
    TimerEnable(TIMER2_BASE,TIMER_A);
    echowait=1;
  }
  /*
    If it's a falling edge that was detected, then get the value of the counter
  */
  else{
    pulse = TimerValueGet(TIMER2_BASE,TIMER_A); //record value
    TimerDisable(TIMER2_BASE,TIMER_A);
    echowait=0;
  }


}
void ConfigureTimer(){
  /*
    Set the timer to be periodic.
  */
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
  SysCtlDelay(3);
  TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);
  TimerEnable(TIMER2_BASE,TIMER_A);
}


void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

