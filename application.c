#include "main.h"
uint32_t distance_l; // variable to store the distance measured by the left ultrasonic sensor
uint32_t distance_r; // variable to store the distance measured by the right ultrasonic sensor


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
    setupHibernation();


    // setting up blindspot indicator leds
    setup_blind_spot_leds();
    // setup ultrasonic sensor pins. These pins are defined in ultrasonic_sensor.h 
    uss_setup_pins_1();
    uss_setup_pins_2();
    // Optional for Debugging 
    //ConfigureUART(void)

}

/*
@info : this is the function that executes repeatedly and performs distance measurement
*/

void loop()
{
    checkBlindSpot();
    // If none of the turn signals are on, then go into deep sleep
    // We use normal sleep because the turn signals rely on a timer that
    //    gets messed up in deep sleep mode
    // SSI function is currently blocking so we don't have to worry about that
    
    // if(signalOn) {
    //     SysCtlSleep();
    // }
    // else {
    //     // Set the count time to 10 seconds
    //     // 30000 is the Khz of the int oscillator being used by the deep sleep module
    //     //TimerLoadSet(TIMER1_BASE, TIMER_A, 10*30000);
    //     //TimerEnable(TIMER1_BASE, TIMER_A);

    //     //SysCtlDeepSleep();
    // }
}

int main(void) 
{
    if(HibernateIsActive()){
        HibernateDisable();
    }
    else {
        setup();
    }
    while(1) loop();
}


/*
@info: this function calculaes the distance and displays blind spot alerts
*/
void checkBlindSpot()
{
        distance_l = uss_measure_distance_1(); // measure the distance on left sensor
        distance_r = uss_measure_distance_2(); // measure the distance on right sensor

        // OPTIONAL FOR DEBUGGING
        //UARTprintf("distance1 = %2dcm \n" , distance_1);
        //UARTprintf("distance2= %2dcm \n" , distance_2);

        // measure the distance and switch on leds accordingly
        if(distance_l < DISTANCE_TRESHOLD )
        GPIOPinWrite(BASE_BLIND_SPOT_LED_PORT_LEFT, BLIND_SPOT_LED_PIN_LEFT, BLIND_SPOT_LED_PIN_LEFT);
        else
        GPIOPinWrite(BASE_BLIND_SPOT_LED_PORT_LEFT, BLIND_SPOT_LED_PIN_LEFT, 0);


        if(distance_r < DISTANCE_TRESHOLD)
        GPIOPinWrite(BASE_BLIND_SPOT_LED_PORT_RIGHT, BLIND_SPOT_LED_PIN_RIGHT, BLIND_SPOT_LED_PIN_RIGHT );        
        else
        GPIOPinWrite(BASE_BLIND_SPOT_LED_PORT_RIGHT, BLIND_SPOT_LED_PIN_RIGHT, 0);

}

/*
@info: this function sets up all the pins needed for the blind spot indicator leds
*/
void setup_blind_spot_leds()
{
    // Initialize the port for the left led
    if(!SysCtlPeripheralReady(SYS_BLIND_SPOT_LED_PORT_LEFT))
    {
            SysCtlPeripheralEnable(SYS_BLIND_SPOT_LED_PORT_LEFT);
    }
    //wait until it is done
    while(!SysCtlPeripheralReady(SYS_BLIND_SPOT_LED_PORT_LEFT));

    // Initialize the port for the right led
    if(!SysCtlPeripheralReady(SYS_BLIND_SPOT_LED_PORT_RIGHT))
    {
            SysCtlPeripheralEnable(SYS_BLIND_SPOT_LED_PORT_RIGHT);
    }
    //wait until it is done
    while(!SysCtlPeripheralReady(SYS_BLIND_SPOT_LED_PORT_RIGHT));

    // Set the pins to output
    GPIOPinTypeGPIOOutput(BASE_BLIND_SPOT_LED_PORT_LEFT, BLIND_SPOT_LED_PIN_LEFT);
    GPIOPinTypeGPIOOutput(BASE_BLIND_SPOT_LED_PORT_RIGHT, BLIND_SPOT_LED_PIN_RIGHT);

}

// optional for debugging
void ConfigureUART(void)
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(0, 115200, 16000000);
}