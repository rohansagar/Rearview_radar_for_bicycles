/*
 *This file consists of all the pin assignment macros
  */

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

#define SYS_STOP_SWITCH_PORT        SYSCTL_PERIPH_GPIOB
#define STOP_SWITCH_PORT_BASE       GPIO_PORTB_BASE
#define STOP_SWITCH_PIN             GPIO_PIN_0

// Pin assignments for Timer
#define SYS_TIMER_PORT      SYSCTL_PERIPH_TIMER0
#define TIMER_PORT_BASE     TIMER0_BASE

//Pin assignments for Ultrasonic snesors
#define TRIGGER_PORT_BASE_1
#define SYS_TRIGGER_PORT_1
#define TRIGGER_1
#define ECHO_1
#define TRIGGER_PORT_BASE_2
#define SYS_TRIGGER_PORT_2
#define TRIGGER_2
#define ECHO_2
