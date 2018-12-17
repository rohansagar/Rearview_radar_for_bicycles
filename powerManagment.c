#include "powerManagement.h"

inline void setupSleep(){

    // Enable all peripherals being used, we're just putting the processor to sleep here
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOF);

    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI0);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI2);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_SSI3);

    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_TIMER1);

    //SysCtlDeepSleepClockSet(SYSCTL_DSLP_DIV_1 | SYSCTL_DSLP_OSC_INT30);

}

inline void setupHibernation(){

    // ===== Setup timer for hibernation countdown =====

    // Setup a timer to countdown for hibernation
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3));

    // Set up timer to count down then restart
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);

    // Set timer ISR
    TimerIntRegister(TIMER3_BASE, TIMER_A, hibernateISR);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER3_BASE, TIMER_A);

    // ===== Setup Port A for Wheel Sensor =====

    // Enable Port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

    // Enable PA7 with a weak pull up
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN);
    GPIOPadConfigSet(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure wheel sensor ISR
    // We're going to stick the wheel sensor on the same port as the brake, so we will have to use the same ISR
    // See signalSystem.c for portBISR
    GPIOIntRegister(GPIO_PORTA_BASE, postponeHibernation);
    GPIOIntTypeSet(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN);

    // Set the wheel sensor as one of the lower priorities
    IntPrioritySet(INT_GPIOA, 0x70);

    // Now get the hibernation module setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_HIBERNATE));

    // Turn off GPIO retention, we don't need anything on if the user isn't pedaling anywhere
    HibernateGPIORetentionDisable();
    HibernateWakeSet(HIBERNATE_WAKE_GPIO);
    GPIOPinTypeWakeHigh(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN);
}

void postponeHibernation(){
    // If the wheel is spinning, we're just gonna wakeup the MCU from deep sleep, thus resetting the hibernation timer
    //TimerDisable(TIMER3_BASE, TIMER_A);
}

void hibernateISR(){
    // Here we will put the device into hibernation mode
    SysCtlDelay(1);

    HibernateEnableExpClk(SysCtlClockGet());
    HibernateRequest();
}

