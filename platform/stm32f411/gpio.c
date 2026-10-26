#include "gpio.h"

/* Enable the clock for the selected GPIO port */
static void GPIO_EnableClock(GPIO_TypeDef *port)
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

    /* Ensure the clock-enable write has completed */
    (void)RCC->AHB1ENR;
}

/* Configure a GPIO pin */
void GPIO_Init(GPIO_TypeDef *port,
               uint8_t pin,
               GPIO_Mode mode,
               GPIO_State initial_state)
{
    /* Reject invalid arguments */
    if (port == 0 || pin > 15 || mode > GPIO_MODE_ANALOG)
        return;

    GPIO_EnableClock(port);

    uint32_t shift = (uint32_t)pin * 2U;

    /*
     * Set a known initial output value before
     * switching the pin to output mode.
     */
    if (mode == GPIO_MODE_OUTPUT)
    {
        if (initial_state == GPIO_HIGH)
            port->BSRR = (1U << pin);
        else
            port->BSRR = (1U << (pin + 16U));
    }

    /* Configure pin mode */
    port->MODER &= ~(3U << shift);
    port->MODER |= ((uint32_t)mode << shift);

    /* Default to push-pull output */
    port->OTYPER &= ~(1U << pin);

    /* Default to low output speed */
    port->OSPEEDR &= ~(3U << shift);

    /* Disable internal pull-up and pull-down */
    port->PUPDR &= ~(3U << shift);
}

/* Write HIGH or LOW */
void GPIO_Write(GPIO_TypeDef *port,
                uint8_t pin,
                GPIO_State state)
{
    if (port == 0 || pin > 15)
        return;

    if (state == GPIO_HIGH)
        port->BSRR = (1U << pin);
    else
        port->BSRR = (1U << (pin + 16U));
}

/* Read the input level */
GPIO_State GPIO_Read(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == 0 || pin > 15)
        return GPIO_LOW;

    return (port->IDR & (1U << pin))
           ? GPIO_HIGH : GPIO_LOW;
}

/* Invert the current output state */
void GPIO_Toggle(GPIO_TypeDef *port, uint8_t pin)
{
    if (port == 0 || pin > 15)
        return;

    if (port->ODR & (1U << pin))
        GPIO_Write(port, pin, GPIO_LOW);
    else
        GPIO_Write(port, pin, GPIO_HIGH);
}