#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/hibernate.h"
#include "driverlib/timer.h"

#define ONE_MS (uint32_t)(SysCtlClockGet() / 1000) // One millisecond
#define WHEEL_SENSOR_PIN    GPIO_PIN_1

void setupSleep();
void setupHibernation();
void postponeHibernation();
void hibernateISR();
