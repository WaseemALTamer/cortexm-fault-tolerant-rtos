#include "uart.h"
#include <stdio.h>

/* Enable the GPIO port clock */
static void UART_EnableGPIOClock(GPIO_TypeDef *port)
{
    if (port == GPIOA)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    else if (port == GPIOB)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    else if (port == GPIOC)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    else if (port == GPIOD)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    else if (port == GPIOE)
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;

    (void)RCC->AHB1ENR;
}

/* Enable the selected USART peripheral clock */
static void UART_EnableClock(USART_TypeDef *instance)
{
    if (instance == USART1)
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    else if (instance == USART2)
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    else if (instance == USART6)
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;

    (void)RCC->APB1ENR;
    (void)RCC->APB2ENR;
}

/* Configure a pin for alternate-function operation */
static void UART_ConfigurePin(GPIO_TypeDef *port,
                              uint8_t pin,
                              uint8_t af)
{
    uint32_t shift = (uint32_t)pin * 2U;
    uint32_t afr_index = pin / 8U;
    uint32_t afr_shift = (uint32_t)(pin % 8U) * 4U;

    /* Alternate-function mode */
    port->MODER &= ~(3U << shift);
    port->MODER |=  (2U << shift);

    /* Push-pull output */
    port->OTYPER &= ~(1U << pin);

    /* High speed for UART signals */
    port->OSPEEDR &= ~(3U << shift);
    port->OSPEEDR |=  (2U << shift);

    /* No internal pull-up or pull-down */
    port->PUPDR &= ~(3U << shift);

    /* Select the alternate function */
    port->AFR[afr_index] &= ~(0xFU << afr_shift);
    port->AFR[afr_index] |= ((uint32_t)af << afr_shift);
}

/* Initialize UART */
void UART_Init(UART_Handle *uart,
               USART_TypeDef *instance,
               uint32_t baudrate,
               uint32_t peripheral_clock_hz,
               GPIO_TypeDef *tx_port,
               uint8_t tx_pin,
               GPIO_TypeDef *rx_port,
               uint8_t rx_pin,
               uint8_t alternate_function)
{
    if (uart == 0 || instance == 0 ||
        baudrate == 0 || peripheral_clock_hz == 0 ||
        tx_port == 0 || rx_port == 0 ||
        tx_pin > 15 || rx_pin > 15 ||
        alternate_function > 15)
        return;

    /* Store configuration */
    uart->instance = instance;
    uart->baudrate = baudrate;
    uart->peripheral_clock_hz = peripheral_clock_hz;

    /* Enable clocks */
    UART_EnableGPIOClock(tx_port);
    UART_EnableGPIOClock(rx_port);
    UART_EnableClock(instance);

    /* Configure TX and RX */
    UART_ConfigurePin(tx_port, tx_pin, alternate_function);
    UART_ConfigurePin(rx_port, rx_pin, alternate_function);

    /* Disable UART before configuration */
    instance->CR1 = 0;
    instance->CR2 = 0;
    instance->CR3 = 0;

    /*
     * Oversampling by 16.
     * BRR = peripheral clock / baud rate.
     */
    instance->BRR =
        (peripheral_clock_hz + baudrate / 2U) / baudrate;

    /* 8 data bits, no parity, 1 stop bit */
    instance->CR1 = USART_CR1_TE |
                    USART_CR1_RE |
                    USART_CR1_UE;
}


/* Transmit one character */
void UART_WriteChar(UART_Handle *uart, char ch)
{
    if (uart == 0 || uart->instance == 0)
        return;

    /* Wait until transmit data register is empty */
    while (!(uart->instance->SR & USART_SR_TXE))
    {
    }

    uart->instance->DR = (uint8_t)ch;
}

/* Transmit a null-terminated string */
void UART_Print(UART_Handle *uart, const char *str)
{
    if (uart == 0 || uart->instance == 0 || str == 0)
        return;

    while (*str != '\0')
    {
        UART_WriteChar(uart, *str++);
    }
}

/* Print a string followed by CRLF */
void UART_Println(UART_Handle *uart, const char *str)
{
    UART_Print(uart, str);
    UART_Print(uart, "\r\n");
}




void UART_PrintInt(UART_Handle *uart, int32_t value)
{
    char buffer[12];

    snprintf(buffer, sizeof(buffer), "%ld", (long)value);
    UART_Print(uart, buffer);
}


void UART_PrintFloat(UART_Handle *uart,
                     float value,
                     uint8_t decimal_places)
{
    char buffer[48];


    if (decimal_places > 6)
        decimal_places = 6;

    snprintf(buffer, sizeof(buffer), "%.*f",
             (int)decimal_places, (double)value);

    UART_Print(uart, buffer);
}






/* Check whether received data is available */
uint8_t UART_Available(UART_Handle *uart)
{
    if (uart == 0 || uart->instance == 0)
        return 0;

    return (uart->instance->SR & USART_SR_RXNE) ? 1U : 0U;
}

/* Receive one character (blocking) */
char UART_ReadChar(UART_Handle *uart)
{
    if (uart == 0 || uart->instance == 0)
        return '\0';

    while (!(uart->instance->SR & USART_SR_RXNE))
    {
    }

    return (char)(uart->instance->DR & 0xFFU);
}

/*
 * Read characters until newline or buffer capacity.
 * The buffer is always null-terminated when its size is nonzero.
 */
size_t UART_ReadString(UART_Handle *uart,
                       char *buffer,
                       size_t buffer_size)
{
    if (uart == 0 || uart->instance == 0 ||
        buffer == 0 || buffer_size == 0)
        return 0;

    size_t i = 0;

    while (i < buffer_size - 1U)
    {
        char ch = UART_ReadChar(uart);

        if (ch == '\r' || ch == '\n')
            break;

        buffer[i++] = ch;
    }

    buffer[i] = '\0';

    return i;
}