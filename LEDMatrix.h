#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"

// Possible SSI modules on the Tiva
#define SSI0    0
#define SSI1    1
#define SSI2    2
#define SSI3    3

// Control register addresses
#define NOOP        0x00
#define D0          0x01
#define D1          0x02
#define D2          0x03
#define D3          0x04
#define D4          0x05
#define D5          0x06
#define D6          0x07
#define D7          0x08
#define DECODE      0x09
#define INTENSITY   0x0A
#define SCANLIMIT   0x0B
#define SHUTDOWN    0x0C
#define DISPTEST    0x0F

// Decode Modes
#define NODECODE    0x00
#define CB0         0x01
#define CB30        0x0F
#define CBALL       0xFF

void setupLEDMatrix(uint8_t selectedSSI, uint8_t numDisplays);
void displayTest(bool enableDisable);
void scanLimit(uint8_t digits);
void decodeMode(uint8_t mode);
void clearDisplay();
void intensity(uint8_t value);
void shutdownMode(bool state);
void setDisplay(uint8_t lights[8][]);
void changeDisplay(uint8_t mask[8][], uint8_t setBit);
