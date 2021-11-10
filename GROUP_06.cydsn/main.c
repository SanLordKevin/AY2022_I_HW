/* ========================================
 * Casali Federico
 * Sangalli Kevin
 *
 * ========================================
*/
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

#define SLAVE_BUFFER_SIZE 8
#define POS_WHO_AM_I 2 //position of register who am i  in the slave buffer

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; /// Buffer for the slave device
uint8_t status=0; ///control status 0)off  1) CH temp on  2) CH ph on  3) Both on
int16 period=10; //variable red from the control register 1 the period of the sampling

int main(void){
    CyGlobalIntEnable; // Enable global interrupts 
    // Start the components
    ADC_DelSig_Start();
    AMux_Start() ;
    EZI2C_Start();
    PGA_TEMP_Start();
    isr_ADC_StartEx(Custom_ISR_ADC);
    Timer_ADC_Start();

    // Start the ADC conversion
    ADC_DelSig_StartConvert(); 

    // Set up Slave Buffer
    slaveBuffer[CONTROL_REG0] = 0b00010100; //bit[1:0]-->STATUS (default status is 00)
                                            //bit[5:2]-->NUMBER OF SAMPLES FOR THE MEAN
                                            
    
    // Set up who am i register
    slaveBuffer[POS_WHO_AM_I] = 0xBC;
    
    //Set to zero registers of the variables
    slaveBuffer[CH0_MSB] = 0x00; //temp
    slaveBuffer[CH0_LSB] = 0x00;
    
    slaveBuffer[CH1_MSB] = 0x00; //lux
    slaveBuffer[CH1_LSB] = 0x00;
    
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1 ,slaveBuffer);
    // Set up control register 1
    slaveBuffer[CONTROL_REG1]=40; //Period of the timer that gave us a frequency of 250 Hz
                                  //This frequency is the minimum necessary to sample 5 times before transmission
    
    //set led off
    Pin_LED_Write(0);

    for(;;)
    {
       if (period != slaveBuffer[CONTROL_REG1]) //control if the period has changed
          {
            period=10*slaveBuffer[CONTROL_REG1]; //The user write the period in ms;The period of the timer, instead, is the number of count before th overflow.
                                                 //With a clock set at 10 kHz to have an interrupt every 1ms you set the period of the timer to 10
            Timer_ADC_WritePeriod(period);       //set the new period
          }
    
    }
}   
/* [] END OF FILE */