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
uint8_t status=0; ///control status 0)apento 1)CH temp on 2)CH ph on 3) Both on
int16 period=10; //variabile per leggere il control register 1 con il periodo del timer all'interno 

#define STATO0 0b00010100   //in exadecimale 14 SONO I COMANDI DA INSERIRE NEL BRIDGE CONTROL
#define STATO1 0b00010101   //in exadecimale 15
#define STATO2 0b00010110   //in exadecimale 16
#define STATO3 0b00010111   //in exadecimale 17




int main(void){
    CyGlobalIntEnable; // Enable global interrupts. 
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
    slaveBuffer[CONTROL_REG0] = 0b00010100; //primi 2 bit servono per cambiareclo stato, i bit dal 2 al 5 invece rappresentano il numero di campion usati per la media
                                            //qui imposto che non manda nessun dato, metto status bit a 00
    
    // Set up who am i register
    slaveBuffer[POS_WHO_AM_I] = 0xBC;
    
    //Set to zero registers of the variables
    slaveBuffer[CH0_MSB] = 0x00;
    slaveBuffer[CH0_LSB] = 0x00;
    
    slaveBuffer[CH1_MSB] = 0x00;
    slaveBuffer[CH1_LSB] = 0x00;
    
    
    // Set up EZI2C buffer
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE - 1 ,slaveBuffer);
    // Set up control register 1
    slaveBuffer[CONTROL_REG1]=10;

    Pin_LED_Write(0);

    for(;;)
    {
    // salvo cosa c'è scritto nel registro nella var

        if (period != slaveBuffer[CONTROL_REG1]){
            period=10*slaveBuffer[CONTROL_REG1];
            Timer_ADC_WritePeriod(period);
        }
    
    }
}   
/* [] END OF FILE */