#ifndef application_h
#define application_h

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
//#include "inc/tm4c123gh6pm.h"
//#include "signalSystem.h"
//#include "powerManagement.h"
//#include "ultrasonicSensor.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/ssi.h"
#include "inc/hw_ssi.h"
#include "driverlib/ssi.h"
#include "driverlib/uart.h"

#include "ultrasonic_sensor.h" //ultrasonic sensor library
#include "signalSystem.h"
#include "powerManagement.h"

// ===== PIN RESERVATIONS =====
// UART0 reserved for debugging (PA0-RX PA1-TX)
// SSI0 reserved for matrix display ( PA2-SCK PA3-CS PA4-MISO PA5-MOSI ) PA-6 for CS bc my dumb microcontroller has a dead pin :(
// PB0 - PB1 reserved for USS1
// PD0 - PD1 reserved for USS2
// Port F is being used for debugging purposes (built in LEDs and switches)
// PC0 - PC3 reserved for JTAG
// PE0 will be used for the brake switch
// PD1 will be used for the wheel sensor

// ===== INTERRUPT RESERVATIONS =====
// Port A - No interrupts, but UART and SSI are using it so beware
// Port B - USS1 interrupts
// Port C - No interrupts, but used for JTAG debugging so beware
// Port D - USS2 interrupts
// Port E - Brake sensor interrupts - could possibly be combined with turn signal interrupts if needed, but on board LEDs would have to be unused
// Port F - Turn signal interrupts

// ===== OTHER RESERVATIONS =====
// Timer 0A reserved for turn signals
// Timer 1A reserved for USS1
// Timer 2A reserved for USS2
// Timer 3A reserved for hibernation / power management

// @info: This is the distance limit of the ultrasonic sensor
#define DISTANCE_TRESHOLD 40

#define SYS_BLIND_SPOT_LED_PORT_LEFT SYSCTL_PERIPH_GPIOF
#define SYS_BLIND_SPOT_LED_PORT_RIGHT SYSCTL_PERIPH_GPIOF
#define BASE_BLIND_SPOT_LED_PORT_LEFT GPIO_PORTF_BASE
#define BASE_BLIND_SPOT_LED_PORT_RIGHT GPIO_PORTF_BASE
#define BLIND_SPOT_LED_PIN_LEFT GPIO_PIN_3
#define BLIND_SPOT_LED_PIN_RIGHT GPIO_PIN_2

void setup();
void loop();
void checkBlindSpot();
void setup_blind_spot_leds();
void ConfigureUART();


#endif
