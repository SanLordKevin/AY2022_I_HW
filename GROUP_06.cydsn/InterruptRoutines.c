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

#define STATO0 0b00010100   //in exadecimale 14 SONO I COMANDI DA INSERIRE NEL BRIDGE CONTROL
#define STATO1 0b00010101   //in exadecimale 15
#define STATO2 0b00010110   //in exadecimale 16
#define STATO3 0b00010111   //in exadecimale 17


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
    
    if ( slaveBuffer[CONTROL_REG0] == STATO3 && status!=3) 
    
    {
       status=3;
       Pin_LED_Write(1);
    }; 

   
 //conteggio campioni effettuati
    counter_SP++;
    
    //c++;
    //if(c%50==0){Pin_LED_Write(x); 
    //        if(x==0){x=1;}
    //        else{x=0;}}
    
//select the Temp channel   RISOLVERE PROBLEMA SENSORE DIFETTOSO

if (status==1 || status==3 ){
AMux_FastSelect(CH_TEMP) ;
// Read Timer status register to bring interrupt line low
value_digit = ADC_DelSig_Read32();
if (value_digit < 0) value_digit = 0;
if (value_digit > 65535) value_digit = 65535;
value_temp = ADC_DelSig_CountsTo_mVolts(value_digit);// the sensibility is 10 mV\c
Temp_mean=Temp_mean+value_temp;}


if (status==2 || status==3 ){
//select the Photodiode channel
AMux_FastSelect(CH_PH) ;
// Read Timer status register to bring interrupt line low
value_digit = ADC_DelSig_Read32();
if (value_digit < 0) value_digit = 0;
if (value_digit > 65535) value_digit = 65535;
value_ph= ADC_DelSig_CountsTo_mVolts(value_digit);//da trovare il datasheet
Ph_mean=Ph_mean+value_ph;


}


if (counter_SP==NUMBER_OF_SAMPLES){
// Format ADC result for transmition
Ph_mean=Ph_mean/NUMBER_OF_SAMPLES; //average of 5 samples
Temp_mean=Temp_mean/NUMBER_OF_SAMPLES;
// Write bytes in buffer
DataBufferPh[1] = Ph_mean >> 8;
DataBufferPh[2] = Ph_mean & 0xFF;

DataBufferTemp[1] = Temp_mean >> 8;
DataBufferTemp[2] = Temp_mean & 0xFF;

DataBufferDouble[1] = Ph_mean >> 8;
DataBufferDouble[2] = Ph_mean & 0xFF;
DataBufferDouble[3] = Temp_mean >> 8;
DataBufferDouble[4] = Temp_mean & 0xFF;
counter_SP=0;
Temp_mean=0;
Ph_mean=0;
}

}

CY_ISR(Custom_ISR_DATA){
    

    if (status==2 ){
        UART_PutArray(DataBufferPh, TRANSMIT_BUFFER_SIZE_SINGLE);
        PacketReadyFlag=0;};

    if (status==1){
        UART_PutArray(DataBufferTemp, TRANSMIT_BUFFER_SIZE_SINGLE);
        PacketReadyFlag=0;};

    if (status==3){
        UART_PutArray(DataBufferDouble, TRANSMIT_BUFFER_SIZE_DOUBLE);
        PacketReadyFlag=0;};
    
    
}
    


/* [] END OF FILE */
