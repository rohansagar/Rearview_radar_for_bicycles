#include "powerManagement.h"

inline void setupSleep(){
    SysCtlPeripheralClockGating(true);

    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER1);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER2);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER3);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_TIMER4);
}

inline void setupHibernation(){

    // ===== Setup timer for hibernation countdown =====

    // Setup a timer to countdown for hibernation
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER3));

    // Set up timer to count down then restart
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);

    // Use the PIOSC for this one so that it doesn't change in sleep mode
    //TimerClockSourceSet(TIMER3_BASE, TIMER_CLOCK_PIOSC);

    // Set timer ISR
    TimerIntRegister(TIMER3_BASE, TIMER_A, hibernateISR);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerLoadSet(TIMER3_BASE, TIMER_A, 10*ONE_SEC);
    TimerEnable(TIMER3_BASE, TIMER_A);

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

    // ===== Setup hibernation module itself =====

    // Now get the hibernation module setup
    SysCtlPeripheralEnable(SYSCTL_PERIPH_HIBERNATE);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_HIBERNATE));

    // Turn off GPIO retention, we don't need anything on if the user isn't pedaling anywhere
    HibernateGPIORetentionDisable();
    HibernateWakeSet(HIBERNATE_WAKE_PIN);
    //GPIOPinTypeWakeHigh(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN);
}

void postponeHibernation(){
    // If the wheel is spinning or a button is pushed, reset the hibernation timer
    GPIOIntClear(GPIO_PORTA_BASE, WHEEL_SENSOR_PIN);
    TimerLoadSet(TIMER3_BASE, TIMER_A, 10*ONE_SEC_PIOSC);
}

void hibernateISR(){
    // Here we will put the device into hibernation mode
    TimerIntClear(TIMER3_BASE, TIMER_A);

    // Turn off the timer for the blind spot, or else it will wake us up
    TimerDisable(TIMER4_BASE,TIMER_A);

    HibernateEnableExpClk(SysCtlClockGet());
    HibernateRequest();
}

