/*
 *This file consists of all the pin assignment macros
  */
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
// Pin assignments for LED matrix display
#define     SYS_SSI_PERIPH          SYSCTL_PERIPH_SSI0
#define     SSI_PERIPH_BASE         SSI0_BASE
#define     SYS_CS_PORT             SYSCTL_PERIPH_GPIOA
#define     CS_PORT                 GPIO_PORTA_BASE
#define     CS_PIN                  GPIO_PIN_6
#define     CLOCK_PIN               GPIO_PA2_SSI0CLK
#define     TX_PIN                  GPIO_PA5_SSI0TX
#define     RX_PIN                  GPIO_PA4_SSI0RX
#define     NUM_DISPLAYS            4
// Note: Make sure that you have all the SSI pins on one Port 

// Pin assignments for Turn and Brake switches
#define SYS_TURN_PORT          SYSCTL_PERIPH_GPIOF
#define TURN_PORT_BASE         GPIO_PORTF_BASE
#define LEFT_TURN_PIN               GPIO_PIN_4
#define RIGHT_TURN_PIN              GPIO_PIN_0
// Note: Both the turn switches are on one port

#define SYS_STOP_SWITCH_PORT        SYSCTL_PERIPH_GPIOE
#define STOP_SWITCH_PORT_BASE       GPIO_PORTB_BASE
#define STOP_SWITCH_PIN             GPIO_PIN_1

// Pin assignments for light flashing Timer
#define SYS_TIMER_PORT      SYSCTL_PERIPH_TIMER0
#define TIMER_PORT_BASE     TIMER0_BASE

//Pin assignments for Ultrasonic snesors

//Base address for the ports
#define TRIGGER_PORT_BASE_1 GPIO_PORTB_BASE
#define ECHO_PORT_BASE_1    GPIO_PORTB_BASE

// Addresses to enable the ports
#define SYS_TRIGGER_PORT_1   SYSCTL_PERIPH_GPIOB
#define SYS_ECHO_PORT_1      SYSCTL_PERIPH_GPIOB

// Pins in the port
#define TRIGGER_1       GPIO_PIN_0
#define ECHO_1          GPIO_PIN_1

//Timer to be used to calculate the width of the echo
#define TIMER_USS_1     TIMER1_BASE

// Led indicator defines
#define SYS_BLIND_SPOT_LED_1    SYSCTL_PERIPH_GPIOF
#define BLIND_SPOT_LED_1        GPIO_PORTF_BASE
#define BLIND_SPOT_LED_PIN_1    GPIO_PIN_3
//-------------------------------------------------------------

//Base address for the ports
#define TRIGGER_PORT_BASE_2 GPIO_PORTD_BASE
#define ECHO_PORT_BASE_2    GPIO_PORTD_BASE

// Addresses to enable the ports
#define SYS_TRIGGER_PORT_2  SYSCTL_PERIPH_GPIOD
#define SYS_ECHO_PORT_2     SYSCTL_PERIPH_GPIOD

// Pins in the port
#define TRIGGER_2   GPIO_PIN_0
#define ECHO_2      GPIO_PIN_1

//Timer to be used to calculate the width of the echo
#define TIMER_USS_2     TIMER2_BASE

// led indicator defines
#define SYS_BLIND_SPOT_LED_2    SYSCTL_PERIPH_GPIOF
#define BLIND_SPOT_LED_2        GPIO_PORTF_BASE
#define BLIND_SPOT_LED_PIN_2    GPIO_PIN_2

// Trigger Timer
#define TIMER_TRIGGER   TIMER4_BASE
// Backup timer
#define TIMER_BACKUP    TIMER5_BASE

