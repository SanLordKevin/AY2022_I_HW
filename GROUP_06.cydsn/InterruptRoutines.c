/* ========================================
 * Casali Federico
 * Sangalli Kevin
 *
 * ========================================
*/
#define CH_TEMP 0
#define CH_PH 1
#define STATO0 0b00000000   //This is 0 in exidecimal notation 
#define STATO1 0b00000001   //This is 01 in exidecimal notation 
#define STATO2 0b00000010   //This is 02 in exidecimal notation 
#define STATO3 0b00000011   //This is 03 in exidecimal notation 
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
//counter for the sampling
int32 counter_SP=0; 



// Our global variables
extern uint8_t status;
extern uint8_t slaveBuffer[]; 
extern int number_samples;
 

CY_ISR(Custom_ISR_ADC)
{
    Timer_ADC_ReadStatusRegister();

    // Update status according to option written in the control register 0
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO0 && status!=0 ) // use of  mask to identify teh first two bits and than compare with status 0
    {   
        status=0;
        Pin_LED_Write(0);
    };
    
    
    
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO1 && status!=1 ) 
    {
        status=1;
        Pin_LED_Write(0);
    };
    
    if ((slaveBuffer[CONTROL_REG0] & 0b11) == STATO2 && status!=2 ) 
    {
       
        status=2;
        Pin_LED_Write(0);
    };
    
    if ( (slaveBuffer[CONTROL_REG0] & 0b11) == STATO3 && status!=3 ) 
    {
        status=3;
        Pin_LED_Write(1); //led switch on at status 11 as per requirements
    }; 

   
     //incremental counter of the sample effectuated
    counter_SP++;
    

    if (status==1 || status==3 ) // sampling of the temperature if the statu is equal to 1 or 3
    {   
        //select the Temp channel
        AMux_FastSelect(CH_TEMP) ;
       //read the ADC
        value_digit = ADC_DelSig_Read32();
        if (value_digit < 0) //if value is below the minimum value that of the ADC output 
            value_digit = 0;
        if (value_digit > 65535) 
            value_digit = 65535;
        value_temp = ADC_DelSig_CountsTo_mVolts(value_digit);// tranformation from digits to millivolt
        Temp_mean=Temp_mean+value_temp;
    }


    if (status==2 || status==3 )  // sampling of the luminosity if the statu is equal to 1 or 3
    {
        //select the Photoresistor channel
        AMux_FastSelect(CH_PH) ;
        //read the ADC
        value_digit = ADC_DelSig_Read32();
        if (value_digit < 0) 
            value_digit = 0;
        if (value_digit > 65535)    
            value_digit = 65535;   
        value_ph= ADC_DelSig_CountsTo_mVolts(value_digit);
        Ph_mean=Ph_mean+value_ph;

    }

    //when acquired the correct number of samples do the mean and put values in the buffer
    if (counter_SP == number_samples)
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
