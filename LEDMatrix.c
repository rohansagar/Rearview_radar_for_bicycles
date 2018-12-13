#include "LEDMatrix.h"
#define ONE_US (uint32_t)(SysCtlClockGet() / (1000*1000)) // One millisecond

uint32_t SSIBase, SSIPeripheral, numDisplays;
uint16_t dataPacket;

// Max daisy chain is 4 panels, just for our needs
uint8_t dataHold[8][4];

inline void setupLEDMatrix(uint8_t selectedSSI, uint8_t num){
    numDisplays = num;

    switch (selectedSSI){
    case SSI0:
        SSIPeripheral = SYSCTL_PERIPH_SSI0;
        SSIBase = SSI0_BASE;
        break;
    case SSI1:
        SSIPeripheral = SYSCTL_PERIPH_SSI1;
        SSIBase = SSI1_BASE;
        break;
    case SSI2:
        SSIPeripheral = SYSCTL_PERIPH_SSI2;
        SSIBase = SSI2_BASE;
        break;
    case SSI3:
        SSIPeripheral = SYSCTL_PERIPH_SSI3;
        SSIBase = SSI3_BASE;
        break;
    }

    // Enable SSI peripheral
    SysCtlPeripheralEnable(SSIPeripheral);
    while(!SysCtlPeripheralReady(SSIPeripheral));

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    //
    // Configure the pin muxing for SSI0 functions on port A2, A3, A4, and A5.
    // This step is not necessary if your part does not support pin muxing.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    //GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);

    // Configure SSI
    // Polarity 0, Phase 0, clock speed = 10MHz (Maximum of display is 10MHz)
    // A full update of the display takes ~1ms
    volatile uint32_t speed = SysCtlClockGet();
    SSIConfigSetExpClk(SSIBase, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10*1000*1000, 16);

    // Enable the SSI module.
    SSIEnable(SSIBase);

    // shutdown off, no decide, scan all
    shutdownMode(false);
    displayTest(true);
    displayTest(false);
    decodeMode(NODECODE);
    scanLimit(7);
    intensity(0x00);
    clearDisplay();
}

void displayTest(bool state){
    // If state = true, turn on test mode, otherwise turn it off
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    SysCtlDelay(500);
    if(state){
        for (int i = 0; i < numDisplays; i++) {
            dataPacket = (DISPTEST << 8) + 0x01;
            SSIDataPut(SSIBase, dataPacket);
        }
    }
    else {
        for (int i = 0; i < numDisplays; i++) {
            dataPacket = (DISPTEST << 8) + 0x00;
            SSIDataPut(SSIBase, dataPacket);
        }
    }
    while(SSIBusy(SSIBase));
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);
}

void scanLimit(uint8_t digits){
    // Any number 0-7, determines how many digits are scanned for
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    SysCtlDelay(500);
    for (int i = 0; i < numDisplays; i++) {
        dataPacket = (SCANLIMIT << 8) + digits;
        SSIDataPut(SSIBase, dataPacket);
    }
    while(SSIBusy(SSIBase));
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);
}

void decodeMode(uint8_t mode){
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    SysCtlDelay(500);
    for (int i = 0; i < numDisplays; i++) {
        dataPacket = (DECODE << 8) + mode;
        SSIDataPut(SSIBase, dataPacket);
    }
    while(SSIBusy(SSIBase));
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);
}

void clearDisplay(){
    volatile int currentRow;
    for(currentRow = 0; currentRow < 8; currentRow++){
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
        SysCtlDelay(500);
        volatile int currentDisp;
        for (currentDisp = numDisplays-1; currentDisp >= 0; currentDisp--){
            dataHold[currentRow][currentDisp] = 0x00;
            dataPacket = ((currentRow+1) << 8) + dataHold[currentRow][currentDisp];
            SSIDataPut(SSIBase, dataPacket);
        }
        while(SSIBusy(SSIBase));
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
        SysCtlDelay(500);
    }
}

void intensity(uint8_t value){
    // Can be any number 0-15
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    SysCtlDelay(500);
    for (int i = 0; i < numDisplays; i++) {
        dataPacket = (INTENSITY << 8) + value;
        SSIDataPut(SSIBase, dataPacket);
    }
    while(SSIBusy(SSIBase));
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);
}

void shutdownMode(bool state){
    // If state = true, turn on shutdown mode, otherwise turn on normal operation
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
    SysCtlDelay(500);
    if(state){
        for (int i = 0; i < numDisplays; i++) {
            dataPacket = (SHUTDOWN << 8) + 0x00;
            SSIDataPut(SSIBase, dataPacket);
        }
    }
    else {
        for (int i = 0; i < numDisplays; i++) {
            dataPacket = (SHUTDOWN << 8) + 0x01;
            SSIDataPut(SSIBase, dataPacket);
        }
    }
    while(SSIBusy(SSIBase));
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
    SysCtlDelay(500);
}

// Sets all LEDs on all panels
void setDisplay(uint8_t lights[8][numDisplays]){
   for(int currentRow = 0; currentRow < 8; currentRow++){
       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
       SysCtlDelay(500);
       for (int currentDisp = 0; currentDisp < numDisplays; currentDisp++){
            dataHold[currentRow][currentDisp] = lights[currentRow][currentDisp];
            dataPacket = ((currentRow+1) << 8) + dataHold[currentRow][currentDisp];
            SSIDataPut(SSIBase, dataPacket);
        }
       while(SSIBusy(SSIBase));
       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
       SysCtlDelay(500);
    }

}

// Will only modify selected LEDs on all panels
// use setBit to select if it will change 0s or 1s
void changeDisplay(uint8_t mask[8][numDisplays], uint8_t setBit){
    for(int currentRow = 0; currentRow < 8; currentRow++){
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0x00);
        SysCtlDelay(500);
        for (int currentDisp = 0; currentDisp < numDisplays; currentDisp++){

            // if setBit == 0, then we will change all the 0s in the mask to 0 on dataHold
            if(setBit == 0){
                dataHold[currentRow][currentDisp] &= ~mask[currentRow][currentDisp];
            }
            // else if setBit == 1 (or higher), then we will change all the 1s in the mask to 1 on dataHold
            else if(setBit == 1){
                dataHold[currentRow][currentDisp] |= mask[currentRow][currentDisp];
            }

            dataPacket = ((currentRow+1) << 8) + dataHold[currentRow][currentDisp];
            SSIDataPut(SSIBase, dataPacket);
        }
        while(SSIBusy(SSIBase));
        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
        SysCtlDelay(500);
    }
}
