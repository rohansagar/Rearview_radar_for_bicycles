#ifndef application_h
#define application_h



#include "ultrasonic_sensor.h" //ultrasonic sensor library
//#include "signalSystem.h"
#include "powerManagement.h"

// ===== PIN RESERVATIONS =====
// PA0 - PA1: UART0 reserved for debugging (PA0-RX PA1-TX)
// PA2 - PA6: SSI0 reserved for matrix display ( PA2-SCK PA3-CS PA4-MISO PA5-MOSI ) PA-6 for CS bc my dumb microcontroller has a dead pin :(
// PA7      : Wheel sensor (PA7)
// PB0 - PB1: USS1 (PB0-Trigger PB1-Echo)
// PC0 - PC3: JTAG
// PD0 - PD1: USS2 (PD0-Trigger PD1-Echo)
// PE0      : Brake switch (PE0)
// Port F is being used for debugging purposes (built in LEDs and switches)

// ===== INTERRUPT RESERVATIONS =====
// Port A - Wheel sensor interrupts, but also UART and SSI are using it so beware
// Port B - USS1 interrupts
// Port C - No interrupts, but used for JTAG debugging so beware
// Port D - USS2 interrupts
// Port E - Brake sensor interrupts - could possibly be combined with turn signal interrupts if needed, but on board LEDs would have to be unused
// Port F - Turn signal interrupts

// ===== INTERRUPT PRIORITIES =====
// Port A  - 0x70
// Port B  - 0x00
// Port C  -
// Port D  - 0x00
// Port E
// Port F
// Timer0A -
// Timer1A -
// Timer2A -
// Timer3A -
// Timer4A -

// ===== OTHER RESERVATIONS =====
// Timer 0A reserved for turn signals
// Timer 1A reserved for USS1
// Timer 2A reserved for USS2
// Timer 3A reserved for hibernation / power management
// Timer 4A reserved for triggering USSs

void setup();
void loop();


#endif
