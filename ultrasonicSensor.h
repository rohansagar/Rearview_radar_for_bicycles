#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_pwm.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include <string.h>

#define sys_trigger_port    SYSCTL_PERIPH_GPIOA
#define trigger_port    GPIO_PORTA_BASE
#define trigger_pin     GPIO_PIN_3

#define sys_echo_port   SYSCTL_PERIPH_GPIOA
#define echo_port   GPIO_PORTA_BASE
#define echo_pin    GPIO_PIN_2

/* Add this to startup.c
extern inputInt(void);
input_int, for the GPIO handler

*/
void inputInt();
void ConfigureTimer();
void ConfigureUART(void);
void checkBlindSpot();

