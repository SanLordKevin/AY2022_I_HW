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
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    #include "cytypes.h"
    #include "stdio.h"
    #define BYTE_TO_SEND_DOUBLE 4
    #define TRANSMIT_BUFFER_SIZE_DOUBLE 1+BYTE_TO_SEND_DOUBLE+1
    #define BYTE_TO_SEND_SINGLE 2
    #define TRANSMIT_BUFFER_SIZE_SINGLE 1+BYTE_TO_SEND_SINGLE+1
    #define CH0_MSB 3 //where i save the mean temp
    #define CH0_LSB 4 //where i save the mean temp
    #define CH1_MSB 5 //where i save the mean lux
    #define CH1_LSB 6 //where i save the mean lux
    #define CONTROL_REG0 0 //position of control register 0 in the slave buffer
    #define CONTROL_REG1 1 //position of control register 1 in the slave buffer
    
    
    CY_ISR_PROTO (Custom_ISR_ADC);


#endif
/* [] END OF FILE */
