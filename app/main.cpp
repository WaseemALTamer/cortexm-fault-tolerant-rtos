#include <gpio.h>
#include <stm32f411xe.h>
#include <uart.h>

#include <stdio.h>
#include <stdint.h>


#include "main.h"



UART_Handle uart2;


void init(void){

    UART_Init(&uart2,
              USART2,
              115200,
              16000000U,
              GPIOA, 2,     /* TX */
              GPIOA, 3,     /* RX */
              7);           /* AF7 */

    UART_Println(&uart2, "STM32 UART ready!");
    GPIO_Init(GPIOA, 5, GPIO_MODE_OUTPUT, GPIO_LOW);
}

void loop(void)
{


    
    for (uint32_t i = 0; i < 1000U; i++){
        UART_PrintInt(&uart2, i);
        UART_Print(&uart2, "\n");
    }

    GPIO_Toggle(GPIOA, 5);
}

