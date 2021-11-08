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
#define STATO0 0b00010100   //in exadecimale 14 SONO I COMANDI DA INSERIRE NEL BRIDGE CONTROL
#define STATO1 0b00010101   //in exadecimale 15
#define STATO2 0b00010110   //in exadecimale 16
#define STATO3 0b00010111   //in exadecimale 17


// Include header
#include "InterruptRoutines.h"
// Include required header files
#include "project.h"

// Variables declaration */
int32 value_digit; //support variable for ADC sampling
int32 value_temp;
int32 value_ph;
int32 Temp_mean;
int32 Ph_mean;
int32 counter_SP=0; // sampling counter



// Our global variables
extern uint8_t status;
extern uint8_t slaveBuffer[]; 

CY_ISR(Custom_ISR_ADC)
{
    Timer_ADC_ReadStatusRegister();

    // Update status
    if ( slaveBuffer[CONTROL_REG0] == STATO0 && status!=0 )
    {
       status=0;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == STATO1 && status!=1 ) 
    {
       status=1;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == STATO2 && status!=2 ) 
    {
       status=2;
       Pin_LED_Write(0);
    };
    
    if ( slaveBuffer[CONTROL_REG0] == STATO3 && status!=3 ) 
    
    {
       status=3;
       Pin_LED_Write(1);
    }; 

   
     //conteggio campioni effettuati
    counter_SP++;
    
    
    //select the Temp channel  

    if (status==1 || status==3 )
    {
        AMux_FastSelect(CH_TEMP) ;
        // Read Timer status register to bring interrupt line low
        value_digit = ADC_DelSig_Read32();
        if (value_digit < 0) 
            value_digit = 0;
        if (value_digit > 65535) 
            value_digit = 65535;
        value_temp = ADC_DelSig_CountsTo_mVolts(value_digit);// the sensibility is 10 mV\c
        Temp_mean=Temp_mean+value_temp;
    }


    if (status==2 || status==3 )
    {
        //select the Photodiode channel
        AMux_FastSelect(CH_PH) ;
        // Read Timer status register to bring interrupt line low
        value_digit = ADC_DelSig_Read32();
        if (value_digit < 0) 
            value_digit = 0;
        if (value_digit > 65535)    
            value_digit = 65535;   
        value_ph= ADC_DelSig_CountsTo_mVolts(value_digit);
        Ph_mean=Ph_mean+value_ph;

    }


    if (counter_SP==NUMBER_OF_SAMPLES)
    {
        // Format ADC result for transmition
        Ph_mean=Ph_mean/NUMBER_OF_SAMPLES; //average of 5 samples
        Temp_mean=Temp_mean/NUMBER_OF_SAMPLES;
        
        // Write bytes in buffer
        slaveBuffer[CH0_MSB] = Ph_mean >> 8;
        slaveBuffer[CH0_LSB] = Ph_mean & 0xFF;
        slaveBuffer[CH1_MSB] = Temp_mean >> 8;
        slaveBuffer[CH1_LSB] = Temp_mean & 0xFF;
        counter_SP=0;
        Temp_mean=0;
        Ph_mean=0;
    }

}
 

/* [] END OF FILE */
