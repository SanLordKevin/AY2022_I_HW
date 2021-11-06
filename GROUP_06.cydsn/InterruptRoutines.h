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
#define BYTE_TO_SEND 2
#define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1
    
CY_ISR_PROTO (Custom_ISR_ADC);


uint8_t DataBufferPh[TRANSMIT_BUFFER_SIZE];
uint8_t DataBufferTemp[TRANSMIT_BUFFER_SIZE];
volatile uint8 PacketReadyFlag;
#endif
/* [] END OF FILE */
