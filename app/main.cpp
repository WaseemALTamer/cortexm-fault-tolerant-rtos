
#include <cpu_frequency.h>
#include <stm32f411xe.h>
#include <gpio.h>
#include <uart.h>


#include <tick_controller.h>


#include <stdio.h>
#include <stdint.h>





#include "main.h"



//UART_Handle uart2;



UART_Handle uart2;




void init(void){


    UART_Init(&uart2,
            USART2,
            115200,
            CPU_Frequency_GetAPB1(),
            GPIOA, 2,     
            GPIOA, 3,    
            7);           

    UART_Println(&uart2, "STM32 UART ready!");
    UART_Println(&uart2, "");    



    GPIO_Init(GPIOA, 5, GPIO_MODE_OUTPUT, GPIO_LOW);
}

void loop(void)
{

    
    UART_PrintInt(&uart2, TickController_GetUptimeSeconds());
    UART_Println(&uart2, "");

    TickController_delay_us(1000000);
    GPIO_Toggle(GPIOA, 5);
}

