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
    #define CH0_MSB 3
    #define CH0_LSB 4
    #define CH1_MSB 5
    #define CH1_LSB 6

    CY_ISR_PROTO (Custom_ISR_ADC);


#endif
/* [] END OF FILE */
