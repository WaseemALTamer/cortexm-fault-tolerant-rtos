
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* GPIO peripheral base addresses */
#define GPIOA_BASE  0x40020000U
#define GPIOB_BASE  0x40020400U
#define GPIOC_BASE  0x40020800U
#define GPIOD_BASE  0x40020C00U
#define GPIOE_BASE  0x40021000U
#define GPIOH_BASE  0x40021C00U

/* RCC registers */
#define RCC_BASE       0x40023800U
#define RCC_AHB1ENR    (*(volatile uint32_t *)(RCC_BASE + 0x30U))

/* GPIO register offsets */
#define GPIO_MODER_OFFSET    0x00U
#define GPIO_OTYPER_OFFSET   0x04U
#define GPIO_OSPEEDR_OFFSET  0x08U
#define GPIO_PUPDR_OFFSET    0x0CU
#define GPIO_IDR_OFFSET      0x10U
#define GPIO_ODR_OFFSET      0x14U
#define GPIO_BSRR_OFFSET     0x18U
#define GPIO_LCKR_OFFSET     0x1CU
#define GPIO_AFRL_OFFSET     0x20U
#define GPIO_AFRH_OFFSET     0x24U

/* Convert a port base address into a register */
#define GPIO_REG(base, offset) (*(volatile uint32_t *)((base) + (offset)))

/* GPIO registers */
#define GPIO_MODER(base)    GPIO_REG(base, GPIO_MODER_OFFSET)
#define GPIO_OTYPER(base)   GPIO_REG(base, GPIO_OTYPER_OFFSET)
#define GPIO_OSPEEDR(base)  GPIO_REG(base, GPIO_OSPEEDR_OFFSET)
#define GPIO_PUPDR(base)    GPIO_REG(base, GPIO_PUPDR_OFFSET)
#define GPIO_IDR(base)      GPIO_REG(base, GPIO_IDR_OFFSET)
#define GPIO_ODR(base)      GPIO_REG(base, GPIO_ODR_OFFSET)
#define GPIO_BSRR(base)     GPIO_REG(base, GPIO_BSRR_OFFSET)
#define GPIO_LCKR(base)     GPIO_REG(base, GPIO_LCKR_OFFSET)
#define GPIO_AFRL(base)     GPIO_REG(base, GPIO_AFRL_OFFSET)
#define GPIO_AFRH(base)     GPIO_REG(base, GPIO_AFRH_OFFSET)

/* Pin numbers */
#define GPIO_PIN_0   (1U << 0)
#define GPIO_PIN_1   (1U << 1)
#define GPIO_PIN_2   (1U << 2)
#define GPIO_PIN_3   (1U << 3)
#define GPIO_PIN_4   (1U << 4)
#define GPIO_PIN_5   (1U << 5)
#define GPIO_PIN_6   (1U << 6)
#define GPIO_PIN_7   (1U << 7)
#define GPIO_PIN_8   (1U << 8)
#define GPIO_PIN_9   (1U << 9)
#define GPIO_PIN_10  (1U << 10)
#define GPIO_PIN_11  (1U << 11)
#define GPIO_PIN_12  (1U << 12)
#define GPIO_PIN_13  (1U << 13)
#define GPIO_PIN_14  (1U << 14)
#define GPIO_PIN_15  (1U << 15)

/* GPIO clock enable bits in RCC_AHB1ENR */
#define GPIOA_CLOCK_ENABLE (1U << 0)
#define GPIOB_CLOCK_ENABLE (1U << 1)
#define GPIOC_CLOCK_ENABLE (1U << 2)
#define GPIOD_CLOCK_ENABLE (1U << 3)
#define GPIOE_CLOCK_ENABLE (1U << 4)
#define GPIOH_CLOCK_ENABLE (1U << 7)

#endif
