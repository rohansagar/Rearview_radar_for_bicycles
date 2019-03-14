/*
 * led_matrix.c
 *
 *  Created on: Feb 11, 2019
 *      Author: rohan
 */
#include "led_matrix.h"

/*
@info: this function initializes the led matrix by setting up the necessary pins, peripherlas and registers
@parms: in: intensity of the pixels. 
*/
void initialize_matrix(uint8_t intensity)
{
    // saving the intensity
    status.intensity == intensity;
    // Enabling the SSI Peripheral 
    SysCtlPeripheralEnable(SYS_SSI_PERIPH);
    while(!SysCtlPeripheralReady(SYS_SSI_PERIPH));

    // Enable the Port for Chip Select
    SysCtlPeripheralEnable(SYS_CS_PORT);
    while(!SysCtlPeripheralReady(SYS_CS_PORT));

    // Configuring the Pins for the SSI
    GPIOPinConfigure(CLOCK_PIN);
    GPIOPinConfigure(TX_PIN);
    
    // Setting up the Chip Select pin
    GPIOPadConfigSet(CS_PORT, CS_PIN , GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOOutput(CS_PORT, CS_PIN);
    GPIOPinWrite(CS_PORT, CS_PIN, CS_PIN);
    
    //Setting GPIO Pin types
    GPIOPinTypeSSI(CS_PORT, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);

    // Setting SSI Parameters (Speed: 10mhz)
    SSIConfigSetExpClk(SSI_PERIPH_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 10*1000*1000, 16);

    // Enable the SSI module.
    SSIEnable(SSI_PERIPH_BASE);


    // Initialization steps for the LED matrix display
    set_control_register(DISPTEST, 0x00);
    set_control_register(SCANLIMIT,0x07 );
    set_control_register(INTENSITY, status.intensity);
    set_control_register(DECODE, (uint8_t)NODECODE);
    clear_display();
    set_control_register(SHUTDOWN, 0x01);

    // setting the flash indicators to Off
    status.left_flash = 0;
    status.right_flash = 0;

}


/*
 * @info: this function clears the display
 *
 * */
void clear_display(void)
{
    status.right = 0;
    status.left = 0;
    status.stop = 0;

    for(int row = 0; row<8; row++)
    {
        
        GPIOPinWrite(CS_PORT, CS_PIN, 0x00);
        SysCtlDelay(3);
        uint16_t packet = ((row+1)<<8) + 0x00;
        for(int col = 0; col <4; col++)
        {
            SSIDataPut(SSI_PERIPH_BASE, packet);
        }
        while(SSIBusy(SSI_PERIPH_BASE));
        GPIOPinWrite(CS_PORT, CS_PIN, CS_PIN);
        SysCtlDelay(3);
    }


}


/*
 * @info: this function sets the control registers
 * @params: conrtrol register and the value unsigned 8bit integer
 * */
void set_control_register(control_register control_reg_temp , uint8_t value)
{
    GPIOPinWrite(CS_PORT, CS_PIN, 0x00);
    SysCtlDelay(3);
    uint16_t data_packet;
    for (int i = 0; i < NUM_DISPLAYS; i++)
        {
            data_packet =(control_reg_temp  << 8) + value;
            SSIDataPut(SSI_PERIPH_BASE, data_packet);
        }
    while(SSIBusy(SSI_PERIPH_BASE));

    GPIOPinWrite(CS_PORT, CS_PIN, CS_PIN);
    SysCtlDelay(3);


}


/*
 * @info: this function is used to turn on or off a particular signal
 * @params: signal and on/off boolean
 * */
void set_signal(signal sig, bool sig_on_off)
{

    if (sig == RIGHT)
    {
        if(sig_on_off){
            status.right = 1;
        }

        else{
            status.right =0;
        }

    }

    else if (sig == LEFT)
    {
        if(sig_on_off){
                   status.left = 1;
               }

               else{
                   status.left =0;
               }

    }
    else if (sig == STOP)
    {
        if(sig_on_off){
                   status.stop = 1;
               }

               else{
                   status.stop =0;
               }

        }

   for(int row = 0; row < 8;row++)
   {

       GPIOPinWrite(CS_PORT, CS_PIN, 0x00);
       SysCtlDelay(3);

       uint8_t  sub_packet;
       uint16_t packet;
       for(int col = 0; col < NUM_DISPLAYS; col++)
       {
           sub_packet = (signal_array[0][row][col] * status.right) |(signal_array[1][row][col] * status.left) | (signal_array[2][row][col] * status.stop);
           packet = ( (row+1)<<8) + sub_packet;
           SSIDataPut(SSI_PERIPH_BASE,packet);
       }
       while(SSIBusy(SSI_PERIPH_BASE));
       GPIOPinWrite(CS_PORT, CS_PIN, CS_PIN);
       SysCtlDelay(3);

   }

}
