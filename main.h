#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

#include "signalSystem.h"
#include "powerManagement.h"
#include "ultrasonicSensor.h"

// Timer 0A reserved for turn signals
// UART0 reserved for ??? (PA0-RX PA1-TX)
// SSI0 reserved for matrix display ( PA2-SCK PA3-CS PA4-MISO PA5-MOSI ) PA-6 for CS bc my dumb microcontroller has a dead pin :(
// Port F is being used for debugging purposes (built in LEDs and switches)
// PC0 - PC3 reserved for JTAG
// PB0 will be used for the brake switch
// PD1 will be used for the wheel sensor
