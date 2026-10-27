#include <gpio.h>
#include <stm32f411xe.h>
#include <uart.h>

#include <stdio.h>
#include <stdint.h>
#include <cpu_frequency.h>

#include "main.h"



UART_Handle uart2;

uint32_t clock_speed;



void delay_us(uint32_t microseconds)
{
    const uint32_t cycles_per_us = clock_speed / 1000000U;
    const uint32_t start = CPU_CycleCounter_Get();
    const uint32_t cycles = cycles_per_us * microseconds;

    while ((uint32_t)(CPU_CycleCounter_Get() - start) < cycles)
    {
        // Wait for the hardware cycle counter.
    }
}



void init(void){

    CPU_Frequency_Init(CPU_FREQ_16MHZ);
    clock_speed = CPU_Frequency_Get();

    CPU_StartCycleCounter();


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


    
    delay_us(1000000);
    GPIO_Toggle(GPIOA, 5);
}

