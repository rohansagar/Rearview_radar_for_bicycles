/*
 *This file consists of all the pin assignment macros
  */
#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/pin_map.h"






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
#define STOP_SWITCH_PORT_BASE       GPIO_PORTE_BASE
#define STOP_SWITCH_PIN             GPIO_PIN_1

// Pin assignments for light flashing Timer
#define SYS_TIMER_PORT      SYSCTL_PERIPH_TIMER0
#define TIMER_PORT_BASE     TIMER0_BASE

// Pin definitions for ultrasonic sensors

/*PIN MAP
 *  USS 1  TRIGGER B0
 *  USS 1  ECHO    B1
*/
#define sys_trigger_port_1    SYSCTL_PERIPH_GPIOB
#define trigger_port_1        GPIO_PORTB_BASE
#define trigger_pin_1         GPIO_PIN_0

#define sys_echo_port_1       SYSCTL_PERIPH_GPIOB
#define echo_port_1           GPIO_PORTB_BASE
#define echo_pin_1            GPIO_PIN_1


/*PIN MAP
 *  USS 2  TRIGGER D0
 *  USS 2  ECHO    D1
*/
#define sys_trigger_port_2    SYSCTL_PERIPH_GPIOD
#define trigger_port_2        GPIO_PORTD_BASE
#define trigger_pin_2         GPIO_PIN_0

#define sys_echo_port_2       SYSCTL_PERIPH_GPIOD
#define echo_port_2           GPIO_PORTD_BASE
#define echo_pin_2            GPIO_PIN_1



#define SYS_BLIND_SPOT_LED_PORT_LEFT SYSCTL_PERIPH_GPIOF
#define SYS_BLIND_SPOT_LED_PORT_RIGHT SYSCTL_PERIPH_GPIOF
#define BASE_BLIND_SPOT_LED_PORT_LEFT GPIO_PORTF_BASE
#define BASE_BLIND_SPOT_LED_PORT_RIGHT GPIO_PORTF_BASE
#define BLIND_SPOT_LED_PIN_LEFT GPIO_PIN_3
#define BLIND_SPOT_LED_PIN_RIGHT GPIO_PIN_2



#define DISTANCE_TRESHOLD 40
#define ONE_MS (uint32_t)(SysCtlClockGet()/1000)



uint32_t distance_l; // variable to store the distance measured by the left ultrasonic sensor
uint32_t distance_r; // variable to store the distance measured by the right ultrasonic sensor

