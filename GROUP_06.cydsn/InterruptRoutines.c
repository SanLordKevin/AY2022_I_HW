/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#define CH_TEMP 0
#define CH_PH 1
#define NUMBER_OF_SAMPLES 5
#define CONTROL_REG0 0
#define CONTROL_REG1 1
#define TRANSMISSION 20 //ogni 20 interrupt trasmetto il dato
// Include header
#include "InterruptRoutines.h"
// Include required header files
#include "project.h"
// Variables declaration */
int32 value_digit;
int32 value_temp;
int32 value_ph;
int32 Temp_mean;
int32 Ph_mean;
int32 counter_SP=0; //counter dei campioni
int32 counter_TR=0; //counter della trasmissione

// Our global variables
extern uint8_t status;
extern uint8_t slaveBuffer[];

CY_ISR(Custom_ISR_ADC)
{Timer_ADC_ReadStatusRegister();
 // Increment counter in slave buffer
    slaveBuffer[CONTROL_REG1]++;
 //conteggio campioni effettuati
    counter_SP++;
//select the Temp channel   RISOLVERE PROBLEMA SENSORE DIFETTOSO

/*if (status==1 || status==3 ){
AMux_FastSelect(CH_TEMP) ;
// Read Timer status register to bring interrupt line low
value_digit = ADC_DelSig_Read32();
if (value_digit < 0) value_digit = 0;
if (value_digit > 65535) value_digit = 65535;
value_temp = ADC_DelSig_CountsTo_mVolts(value_digit)/10;// the sensibility is 10 mV\c
Temp_mean=Temp_mean+value_ph;}*/

if (status==2 || status==3 ){
//select the Photodiode channel
AMux_FastSelect(CH_PH) ;
// Read Timer status register to bring interrupt line low

value_digit = ADC_DelSig_Read32();
if (value_digit < 0) value_digit = 0;
if (value_digit > 65535) value_digit = 65535;
value_ph= ADC_DelSig_CountsTo_mVolts(value_digit);//da trovare il datasheet
Ph_mean=Ph_mean+value_ph;}


if (counter_SP==NUMBER_OF_SAMPLES){
// Format ADC result for transmition
Ph_mean=Ph_mean/NUMBER_OF_SAMPLES; //average of 5 samples
//Temp_mean=Temp_mean/NUMBER_OF_SAMPLES;
// Write bytes in buffer
DataBufferPh[1] = Ph_mean >> 8;
DataBufferPh[2] = Ph_mean & 0xFF;}

if (counter_TR==TRANSMISSION){
PacketReadyFlag=1;
counter_SP=0;
counter_TR=0;
Temp_mean=0;
Ph_mean=0;}
}


/**
*   This function is called when exiting the EZI2C_ISR. Here we
*   perform all the tasks based on the requests.
*/
void EZI2C_ISR_ExitCallback(void)
{
  // Update status
    if ( slaveBuffer[CONTROL_REG0] == 0x00 )
    {
       status=0;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == 0x01 )
    {
       status=1;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == 0x10 )
    {
       status=2;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == 0x11 )
    {
       status=3;
       Pin_LED_Write(1);
    }; 
}

/* [] END OF FILE */
