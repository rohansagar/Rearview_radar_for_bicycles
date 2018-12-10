#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/ssi.h"
#include "driverlib/timer.h"

#include "binary.h"

// For our testing, we will use Red for brake, blue for left, and green for right
#define RED_LED    GPIO_PIN_1 // PF1
#define BLUE_LED   GPIO_PIN_2 // PF2
#define GREEN_LED  GPIO_PIN_3 // PF3
#define PUSH1      GPIO_PIN_4 // PF4
#define PUSH2      GPIO_PIN_0 // PF0
#define BRAKE_SWITCH GPIO_PIN_0 // PB0

#define ONE_MS (uint32_t)(SysCtlClockGet() / 1000) // One millisecond
#define SIGNAL_FLASH_RATE 500 * ONE_MS

extern uint8_t leftArrow[][];
extern uint8_t rightArrow[][];
extern uint8_t brakeSign[][];

void setupButtons();
void setupLights();
void setupBILEDs();
void setupLightsTimer();

void turnButtonISR();
void timer0ISR();
void brakeSwitchISR();

void toggleSignal(uint8_t signal);
void displayLeftSignal();
void displayRightSignal();
void displayHazardSignal();
void displayBrakeSignal();
void displayBlankTurn();
void displayBlankBrake();
