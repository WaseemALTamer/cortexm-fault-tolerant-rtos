#ifndef UART_H
#define UART_H

#include "stm32f411xe.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    USART_TypeDef *instance;
    uint32_t baudrate;
    uint32_t peripheral_clock_hz;
} UART_Handle;

void UART_Init(UART_Handle *uart,
               USART_TypeDef *instance,
               uint32_t baudrate,
               uint32_t peripheral_clock_hz,
               GPIO_TypeDef *tx_port,
               uint8_t tx_pin,
               GPIO_TypeDef *rx_port,
               uint8_t rx_pin,
               uint8_t alternate_function);

void UART_WriteChar(UART_Handle *uart, char ch);

void UART_Print(UART_Handle *uart, const char *str);

void UART_Println(UART_Handle *uart, const char *str);



void UART_PrintInt(UART_Handle *uart, int32_t value);

void UART_PrintFloat(UART_Handle *uart,
                     float value,
                     uint8_t decimal_places);



uint8_t UART_Available(UART_Handle *uart);

char UART_ReadChar(UART_Handle *uart);

size_t UART_ReadString(UART_Handle *uart,
                       char *buffer,
                       size_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif