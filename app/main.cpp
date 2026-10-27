#include <gpio.h>
#include <stm32f411xe.h>
#include <uart.h>

#include <stdio.h>
#include <stdint.h>
#include <cpu_frequency.h>

#include "main.h"



UART_Handle uart2;

uint32_t clock_speed;

void delay(uint32_t micro_seconds){

    uint32_t factor = 1000000 / micro_seconds;

    uint32_t total_cycles_to_wait = clock_speed / factor;

    UART_PrintInt(&uart2, total_cycles_to_wait);
    UART_Println(&uart2, "");

    while(total_cycles_to_wait) total_cycles_to_wait--;
}



void init(void){

    CPU_Frequency_Init(CPU_FREQ_100MHZ);
    clock_speed = CPU_Frequency_Get();



    UART_Init(&uart2,
              USART2,
              115200,
              CPU_Frequency_GetAPB1(),
              GPIOA, 2,     /* TX */
              GPIOA, 3,     /* RX */
              7);           /* AF7 */

    UART_Println(&uart2, "STM32 UART ready!");
    UART_Println(&uart2, "");
    

    UART_PrintInt(&uart2, clock_speed);
    UART_Println(&uart2, "");
    GPIO_Init(GPIOA, 5, GPIO_MODE_OUTPUT, GPIO_LOW);
}

void loop(void)
{


    
    delay(1000000);
    GPIO_Toggle(GPIOA, 5);
}

