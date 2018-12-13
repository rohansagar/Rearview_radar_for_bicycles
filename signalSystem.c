#include "signalSystem.h"
#include "LEDMatrix.h"

/*
 * ============== ALL VARIABLE DEFINITIONS GO HERE ==============
 */

// This is where we define the picture that will appear on the matrix
// Each row is one 8x8 square. Each binary bit is a light.
uint8_t rightArrow[8][4] = {
            { B8(00000000), B8(00000000), B8(00000000), B8(00001000) },
            { B8(00000000), B8(00000000), B8(00000000), B8(00001100) },
            { B8(00000000), B8(00000000), B8(00000000), B8(00001110) },
            { B8(00000000), B8(00000000), B8(00000000), B8(11111111) },
            { B8(00000000), B8(00000000), B8(00000000), B8(11111111) },
            { B8(00000000), B8(00000000), B8(00000000), B8(00001110) },
            { B8(00000000), B8(00000000), B8(00000000), B8(00001100) },
            { B8(00000000), B8(00000000), B8(00000000), B8(00001000) }
};

uint8_t leftArrow[8][4] = {
            { B8(00010000), B8(00000000), B8(00000000), B8(00000000) },
            { B8(00110000), B8(00000000), B8(00000000), B8(00000000) },
            { B8(01110000), B8(00000000), B8(00000000), B8(00000000) },
            { B8(11111111), B8(00000000), B8(00000000), B8(00000000) },
            { B8(11111111), B8(00000000), B8(00000000), B8(00000000) },
            { B8(01110000), B8(00000000), B8(00000000), B8(00000000) },
            { B8(00110000), B8(00000000), B8(00000000), B8(00000000) },
            { B8(00010000), B8(00000000), B8(00000000), B8(00000000) }
};

uint8_t brakeSign[8][4] = {
           { B8(00000000), B8(00111111), B8(11111100), B8(00000000) },
           { B8(00000000), B8(01111111), B8(11111110), B8(00000000) },
           { B8(00000000), B8(11111111), B8(11111111), B8(00000000) },
           { B8(00000000), B8(11111111), B8(11111111), B8(00000000) },
           { B8(00000000), B8(11111111), B8(11111111), B8(00000000) },
           { B8(00000000), B8(11111111), B8(11111111), B8(00000000) },
           { B8(00000000), B8(01111111), B8(11111110), B8(00000000) },
           { B8(00000000), B8(00111111), B8(11111100), B8(00000000) }
};

enum signalType { left, right, hazards };
uint8_t currentSignal;
bool signalOn = false;
bool displayingBlank = false;


/*
 * ============== ALL SETUP FUNCTIONS GO HERE ==============
 */

// LED matrix
inline void setupLights(){
    setupLEDMatrix(SSI0, 4);

    // Just for debugging
    setupBILEDs();

    // The timer which enables flashing turn signals
    setupLightsTimer();
}

inline void setupLightsTimer(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    // Set up timer to count down then restart
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    // Set the count time to 500ms
    TimerLoadSet(TIMER0_BASE, TIMER_A, 500 * ONE_MS);

    // Set timer ISR
    TimerIntRegister(TIMER0_BASE, TIMER_A, timer0ISR);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

// Built in LEDs
inline void setupBILEDs(){
    // Enable port F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Enable LEDs
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED | GREEN_LED | BLUE_LED);
}

inline void setupButtons(){
    // ===== Port F is used for built in lights and switches ====== //

    // Enable and wait for the port to be ready for access
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));

    // Unlock Port F
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R |= 0x01; // Especially PF0

    // Enable SW1 and SW2 with weak pull ups
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, PUSH1 | PUSH2);
    GPIOPadConfigSet(GPIO_PORTF_BASE, PUSH1 | PUSH2, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure interrupts on SW1 and SW2
    GPIOIntRegister(GPIO_PORTF_BASE, turnButtonISR);
    GPIOIntTypeSet(GPIO_PORTF_BASE, PUSH1 | PUSH2, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, PUSH1 | PUSH2);

    // Set as lowest priority
    IntPrioritySet(INT_GPIOF, 0xE0);


    // ===== Port B0 is used for the brake switch ===== //

    // Enable Port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));

    // Enable PB0 with a weak pull up
    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, BRAKE_SWITCH);
    GPIOPadConfigSet(GPIO_PORTB_BASE, BRAKE_SWITCH, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure brake switch interrupt
    // This will trigger on both rising and falling edge
    // Rising edge will trigger the lights off
    // Falling edge will trigger the lights on
    GPIOIntRegister(GPIO_PORTB_BASE, brakeSwitchISR);
    GPIOIntTypeSet(GPIO_PORTB_BASE, BRAKE_SWITCH, GPIO_BOTH_EDGES);
    GPIOIntEnable(GPIO_PORTB_BASE, BRAKE_SWITCH);

    // Set the brake as one of the higher priorities
    IntPrioritySet(INT_GPIOB, 0x80);
}

/*
 * ============== ALL ISR FUNCTIONS GO HERE ==============
 */

void timer0ISR(){
    TimerIntClear(TIMER0_BASE, TIMER_A);
    postponeHibernation();
    if(signalOn){
        // This may seem like backwards logic, but if displayingBlank = true, then we will show the arrow
        // if displayingBlank = false, we will output nothing to the display
        // The reason for the backwards logic is because this function fires at the END of the timer, but displayingBlank resets to false on initialization
        if (displayingBlank){
            displayingBlank = false;
            switch(currentSignal){
            case left:
                displayLeftSignal();
                break;
            case right:
                displayRightSignal();
                break;
            case hazards:
                displayHazardSignal();
                break;
            }
        }
        else {
            displayBlankTurn();
            displayingBlank = true;
        }
    }
}


void turnButtonISR(){
    SysCtlDelay((50*ONE_MS)/3); // Poor man's debounce
    postponeHibernation();

    GPIOIntClear(GPIO_PORTF_BASE, PUSH1 | PUSH2);

    if(GPIOPinRead(GPIO_PORTF_BASE, PUSH1 | PUSH2) == 0x00){
        displayBlankTurn();
        toggleSignal(hazards);
    }
    else if(GPIOPinRead(GPIO_PORTF_BASE, PUSH1) == 0x00){
        displayBlankTurn();
        toggleSignal(left);
    }
    else if(GPIOPinRead(GPIO_PORTF_BASE, PUSH2) == 0x00){
        displayBlankTurn();
        toggleSignal(right);
    }

}

void brakeSwitchISR(){
    SysCtlDelay((5*ONE_MS)/3); // Poor man's debounce
    postponeHibernation();

    // Falling edge turns brake light on
    if (GPIOPinRead(GPIO_PORTB_BASE, BRAKE_SWITCH) == 0x00) {
        displayBrakeSignal();
    }

    // Rising edge turns the brake light off
    if (GPIOPinRead(GPIO_PORTB_BASE, BRAKE_SWITCH) == BRAKE_SWITCH) {
        displayBlankBrake();
    }

    GPIOIntClear(GPIO_PORTB_BASE, BRAKE_SWITCH);
}

/*
 * ============== ALL SIGNALING FUNCTIONS GO HERE ==============
 */

void toggleSignal(uint8_t signal){
    // If it's a different button, then make signalOn false
    if (signal != currentSignal) signalOn = false;

    // If we pushed the same button then turn off the signal
    if(signalOn){
        TimerDisable(TIMER0_BASE, TIMER_A);
        displayBlankTurn();
        signalOn = false;
    }

    // If this is our first time pushing the button, turn on the signal
    else {
        TimerLoadSet(TIMER0_BASE, TIMER_A, SIGNAL_FLASH_RATE);
        TimerEnable(TIMER0_BASE, TIMER_A);
        signalOn = true;
        currentSignal = signal;
        switch(signal){
        case left:
            displayLeftSignal();
            break;
        case right:
            displayRightSignal();
            break;
        case hazards:
            displayHazardSignal();
            break;
        }
    }
    displayingBlank = false;
}

void displayLeftSignal(){
    GPIOPinWrite(GPIO_PORTF_BASE, BLUE_LED, BLUE_LED);
    changeDisplay(leftArrow, 1);
}

void displayRightSignal(){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED, GREEN_LED);
    changeDisplay(rightArrow, 1);
}

void displayHazardSignal(){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED | BLUE_LED, GREEN_LED | BLUE_LED);

    static bool hazardArrowsYet = false;
    static uint8_t hazardArrows[8][4];

    if(!hazardArrowsYet){
        for (int i = 0; i<8; i++){
            for (int j = 0; j<4; j++){
                hazardArrows[i][j] = (leftArrow[i][j] | rightArrow[i][j]);
            }
        }
        hazardArrowsYet = true;
    }

    changeDisplay(hazardArrows, 1);
}

void displayBrakeSignal(){
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED, RED_LED);
    changeDisplay(brakeSign, 1);
}

void displayBlankTurn(){
    GPIOPinWrite(GPIO_PORTF_BASE, GREEN_LED | BLUE_LED, ~(GREEN_LED | BLUE_LED));

    static bool blankArrowsYet = false;
    static uint8_t blankArrows[8][4];

    if(!blankArrowsYet){
        for (int i = 0; i<8; i++){
            for (int j = 0; j<4; j++){
                blankArrows[i][j] = ~(leftArrow[i][j] | rightArrow[i][j]);
            }
        }
        blankArrowsYet = true;
    }
    changeDisplay(blankArrows, 0);
}

void displayBlankBrake(){
    GPIOPinWrite(GPIO_PORTF_BASE, RED_LED, ~(RED_LED));

    static bool blankBrakeYet = false;
    static uint8_t blankBrake[8][4];

    if(!blankBrakeYet){
        for (int i = 0; i<8; i++){
            for (int j = 0; j<4; j++){
                blankBrake[i][j] = ~(brakeSign[i][j]);
            }
        }
        blankBrakeYet = true;
    }
    changeDisplay(blankBrake, 0);
}
