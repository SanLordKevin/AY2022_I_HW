/* ========================================
 * Casali Federico
 * Sangalli Kevin
 *
 * ========================================
*/
#define CH_TEMP 0
#define CH_PH 1
//#define NUMBER_OF_SAMPLES 5
#define STATO0 0b00000000   //This is 14 in exidecimal notation 
#define STATO1 0b00000001   //This is 15 in exidecimal notation 
#define STATO2 0b00000010   //This is 16 in exidecimal notation 
#define STATO3 0b00000011   //This is 17 in exidecimal notation 
//To select the status you write in the control register 0 in exidecimal notation using bridge control

// Include header
#include "InterruptRoutines.h"
// Include required header files
#include "project.h"

// Variables declaration
//support variable for ADC sampling
int32 value_digit; 
//support variables for temperature and lux 
int32 value_temp;  
int32 value_ph;
int32 Temp_mean;
int32 Ph_mean;
//counter for the 
int32 counter_SP=0; 



// Our global variables
extern uint8_t status;
extern uint8_t slaveBuffer[]; 
extern int number_samples;
 

CY_ISR(Custom_ISR_ADC)
{
    Timer_ADC_ReadStatusRegister();

    // Update status according to option written in the control register 0
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO0 && status!=0 )
    {
        status=0;
        ADC_DelSig_Sleep(); //put the ADC on sleep if unusued
        Pin_LED_Write(0);
    };
    
    
    
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO1 && status!=1 ) 
    {
        ADC_DelSig_Wakeup();
        status=1;
        Pin_LED_Write(0);
    };
    
    if ((slaveBuffer[CONTROL_REG0] & 0b11) == STATO2 && status!=2 ) 
    {
       
        ADC_DelSig_Wakeup();
        status=2;
        Pin_LED_Write(0);
    };
    
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO3 && status!=3 ) 
    {
        ADC_DelSig_Wakeup();
        status=3;
        Pin_LED_Write(1);
    }; 

   
     //incremental counter of the sample effectuated
    counter_SP++;
    

    if (status==1 || status==3 )
    {   
        //select the Temp channel
        AMux_FastSelect(CH_TEMP) ;
        // Read Timer status register to bring interrupt line low
        value_digit = ADC_DelSig_Read32();
        if (value_digit < 0) 
            value_digit = 0;
        if (value_digit > 65535) 
            value_digit = 65535;
        value_temp = ADC_DelSig_CountsTo_mVolts(value_digit);
        Temp_mean=Temp_mean+value_temp;
    }


    if (status==2 || status==3 )
    {
        //select the Photoresistor channel
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

    //when acquired the correct number of samples do the mean and put values in the buffer
    if (counter_SP == number_samples && status != 0)
    {
        // Format ADC result for transmition
        Ph_mean=Ph_mean/number_samples; //average of samples
        Temp_mean=Temp_mean/number_samples;
        
        // Write bytes in buffer
        slaveBuffer[CH0_MSB] = Temp_mean >> 8;
        slaveBuffer[CH0_LSB] = Temp_mean & 0xFF;
        slaveBuffer[CH1_MSB] = Ph_mean >> 8;
        slaveBuffer[CH1_LSB] = Ph_mean & 0xFF;
        
        counter_SP=0; //reset number of value collected
        Temp_mean=0;  //reset temp mean
        Ph_mean=0;    //reset lux mean
    }

}
 

/* [] END OF FILE */
