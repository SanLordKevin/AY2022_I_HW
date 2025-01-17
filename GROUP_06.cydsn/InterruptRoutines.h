/* ========================================
 * Casali Federico
 * Sangalli Kevin
 *
 * ========================================
*/
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    #include "cytypes.h"
    #include "stdio.h"
    #define CH0_MSB 3 //where i save the mean temp
    #define CH0_LSB 4 //where i save the mean temp
    #define CH1_MSB 5 //where i save the mean lux
    #define CH1_LSB 6 //where i save the mean lux
    #define CONTROL_REG0 0 //position of control register 0 in the slave buffer
    #define CONTROL_REG1 1 //position of control register 1 in the slave buffer
    
    
    CY_ISR_PROTO (Custom_ISR_ADC);


#endif

/* [] END OF FILE */