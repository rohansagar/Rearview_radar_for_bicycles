/*
 * led_matrix.h
 *
 *  Created on: Feb 11, 2019
 *      Author: rohan
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_
#include "pin_assignments.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

// This srtucture keeps track of the active lights
struct{
    bool right;
    bool right_flash;
    bool left;
    bool left_flash;
    bool stop;
    bool flash_status;
    uint8_t intensity;
}status;



static const uint8_t signal_array[3][8][4] =
{
 {
  { 0b00000000, 0b00000000, 0b00000000, 0b00001000 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00001100 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00001110 },
  { 0b00000000, 0b00000000, 0b00000000, 0b11111111 },
  { 0b00000000, 0b00000000, 0b00000000, 0b11111111 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00001110 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00001100 },
  { 0b00000000, 0b00000000, 0b00000000, 0b00001000 }
 },
{
  { 0b00010000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00110000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b01110000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b11111111, 0b00000000, 0b00000000, 0b00000000 },
  { 0b11111111, 0b00000000, 0b00000000, 0b00000000 },
  { 0b01110000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00110000, 0b00000000, 0b00000000, 0b00000000 },
  { 0b00010000, 0b00000000, 0b00000000, 0b00000000 }
 },
{
  { 0b00000000, 0b00111111, 0b11111100, 0b00000000 },
  { 0b00000000, 0b01111111, 0b11111110, 0b00000000 },
  { 0b00000000, 0b11111111, 0b11111111, 0b00000000 },
  { 0b00000000, 0b11111111, 0b11111111, 0b00000000 },
  { 0b00000000, 0b11111111, 0b11111111, 0b00000000 },
  { 0b00000000, 0b11111111, 0b11111111, 0b00000000 },
  { 0b00000000, 0b01111111, 0b11111110, 0b00000000 },
  { 0b00000000, 0b00111111, 0b11111100, 0b00000000 }
 }
};


// The singnals to be displayed
typedef enum{
    RIGHT,
    LEFT,
    STOP,
}signal;




// Control Registers
typedef enum {
    NOOP        =    0,
    DECODE      =    9,
    INTENSITY   =   10,
    SCANLIMIT   =   11,
    SHUTDOWN    =   12,
    DISPTEST    =   15,
}control_register;


// Decode Modes
enum {
    NODECODE    =   0,
    CB0         =   1,
    CB30        =   15,
    CBALL       =  255,
} decode_mode;



void initialize_matrix(uint8_t);
void set_control_register(control_register control_reg_temp , uint8_t value);
void set_signal(signal,bool);
void clear_display(void);


#endif /* LED_MATRIX_H_ */
