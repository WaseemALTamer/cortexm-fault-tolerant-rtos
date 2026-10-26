#ifndef GPIO_H
#define GPIO_H

#include "stm32f411xe.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    GPIO_MODE_INPUT  = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_AF     = 2,
    GPIO_MODE_ANALOG = 3
} GPIO_Mode;

typedef enum
{
    GPIO_LOW  = 0,
    GPIO_HIGH = 1
} GPIO_State;

typedef enum
{
    GPIO_NO_PULL   = 0,
    GPIO_PULL_UP   = 1,
    GPIO_PULL_DOWN = 2
} GPIO_Pull;

void GPIO_Init(GPIO_TypeDef *port,
               uint8_t pin,
               GPIO_Mode mode,
               GPIO_State initial_state);

void GPIO_Write(GPIO_TypeDef *port,
                uint8_t pin,
                GPIO_State state);

GPIO_State GPIO_Read(GPIO_TypeDef *port, uint8_t pin);

void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif