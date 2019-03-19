#ifndef ultrasonic_sensor_h
#define ultrasonic_sensor_h



#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
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
#include "driverlib/pwm.h"
#include "driverlib/ssi.h"
#include "driverlib/systick.h"
#include <string.h>


#include "pin_assignments.h"

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


// Functions for USS 1
uint32_t uss_measure_distance_1(void);
void echo_int_1(void);
void ConfigureTimer_1(void);
void uss_setup_pins_1(void);


//Functions for USS 2
uint32_t uss_measure_distance_2(void);
void echo_int_2(void);
void ConfigureTimer_2(void);
void uss_setup_pins_2(void);



void checkBlindSpot();

void setup_blind_spot_leds();

#endif
