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
#include "project.h"
#include "stdio.h"
#include "InterruptRoutines.h"

#define SLAVE_BUFFER_SIZE 8
#define POS_WHO_AM_I 2 //poizione nello slave del registro who am i
#define CONTROL_REG0 0
#define CONTROL_REG1 1

uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; ///< Buffer for the slave device
uint8_t status=0;     ///control status 0)apento 1)CH temp on 2)CH ph on 3) Both on
int main(void){
CyGlobalIntEnable; // Enable global interrupts. 
// Start the components
ADC_DelSig_Start();
AMux_Start() ;
UART_Start();
EZI2C_Start();
Timer_ADC_Start();
isr_ADC_StartEx(Custom_ISR_ADC);
Pin_LED_Write(0);
// Set up header and tail of Ph sensor buffer
DataBufferPh[0] = 0xA0;
DataBufferPh[TRANSMIT_BUFFER_SIZE_SINGLE-1] = 0xC0;

// Set up header and tail of Temp sensor buffer
DataBufferTemp[0] = 0xA0;
DataBufferTemp[TRANSMIT_BUFFER_SIZE_SINGLE-1] = 0xC0;

// Set up header and tail of sensors buffer
DataBufferDouble[0] = 0xA0;
DataBufferDouble[TRANSMIT_BUFFER_SIZE_DOUBLE-1] = 0xC0;

// Initialize send flag
PacketReadyFlag = 0;

// Start the ADC conversion
ADC_DelSig_StartConvert(); 


// Set up Slave Buffer
slaveBuffer[CONTROL_REG0] = status;

// Set up who am i register
slaveBuffer[POS_WHO_AM_I] = 0xBC;

// Set up EZI2C buffer
EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1 ,slaveBuffer);
    
for(;;){
if (PacketReadyFlag==1){
// Send out the data
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
}
}
/* [] END OF FILE */
//fine seconda del file