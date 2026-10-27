#include <stm32f411xe.h>
#include "cpu_frequency.h"



/* Wait for a register bit field to reach its expected value */
static int CPU_WaitForBits(volatile uint32_t *reg,
                           uint32_t mask,
                           uint32_t expected)
{
    uint32_t timeout = CPU_TIMEOUT;

    while ((*reg & mask) != expected)
    {
        if (timeout-- == 0U)
            return 0;
    }

    return 1;
}

/* Configure Flash for the target system clock */
static void CPU_ConfigureFlash(uint32_t frequency)
{
    uint32_t latency;

    if (frequency <= 30000000U)
        latency = 0U;
    else if (frequency <= 60000000U)
        latency = 1U;
    else if (frequency <= 90000000U)
        latency = 2U;
    else
        latency = 3U;

    /*
     * Set wait states before increasing the clock.
     * Enable instruction and data caches and prefetch.
     */
    FLASH->ACR =
        (1U << 12) |  /* Instruction cache */
        (1U << 10) |  /* Data cache */
        (1U <<  8) |  /* Prefetch */
        latency;

    (void)FLASH->ACR;
}

/* Configure AHB, APB1 and APB2 prescalers */
static void CPU_ConfigureBuses(void)
{
    /*
     * AHB  = SYSCLK / 1
     * APB1 = HCLK   / 2
     * APB2 = HCLK   / 1
     *
     * APB1 / 2 keeps PCLK1 at or below 50 MHz
     * when SYSCLK is 100 MHz.
     */
    RCC->CFGR &= ~((0xFU << 4) |
                   (0x7U << 10) |
                   (0x7U << 13));

    RCC->CFGR |= (4U << 10);
}

/* Return the PLL multiplier for the requested frequency */
static uint32_t CPU_GetPLLMultiplier(uint32_t frequency)
{
    switch (frequency)
    {
        case CPU_FREQ_48MHZ:
            return 192U;

        case CPU_FREQ_84MHZ:
            return 336U;

        case CPU_FREQ_100MHZ:
            return 400U;

        default:
            return 0U;
    }
}

/* Configure the MCU system clock */
CPU_FrequencyStatus CPU_Frequency_Init(CPU_Frequency frequency)
{
    uint32_t target = (uint32_t)frequency;
    uint32_t pll_n;

    /* Validate the requested frequency */
    if (target != CPU_FREQ_16MHZ  &&
        target != CPU_FREQ_48MHZ  &&
        target != CPU_FREQ_84MHZ  &&
        target != CPU_FREQ_100MHZ)
    {
        return CPU_FREQ_ERROR_INVALID;
    }

    /* Enable HSI */
    RCC->CR |= (1U << 0);

    if (!CPU_WaitForBits(&RCC->CR, (1U << 1), (1U << 1)))
        return CPU_FREQ_ERROR_TIMEOUT;

    /*
     * Switch to HSI before disabling/reconfiguring PLL.
     * SW = 00 selects HSI.
     */
    RCC->CFGR &= ~3U;

    if (!CPU_WaitForBits(&RCC->CFGR,
                         (3U << 2), 0U))
    {
        return CPU_FREQ_ERROR_TIMEOUT;
    }

    /* Disable PLL before changing PLL configuration */
    RCC->CR &= ~(1U << 24);

    if (!CPU_WaitForBits(&RCC->CR,
                         (1U << 25), 0U))
    {
        return CPU_FREQ_ERROR_TIMEOUT;
    }

    /* Configure Flash before increasing system frequency */
    CPU_ConfigureFlash(target);

    /* Enable the power interface and select voltage scale 1 */
    RCC->APB1ENR |= (1U << 28);
    (void)RCC->APB1ENR;

    PWR->CR = (PWR->CR & ~(3U << 14)) | (3U << 14);

    /* Configure bus prescalers */
    CPU_ConfigureBuses();

    /*
     * The internal 16 MHz HSI is used as PLL input.
     *
     * PLLM = 16
     * PLLN = 192, 336 or 400
     * PLLP = 4
     * PLLQ = 4
     *
     * VCO input = 16 MHz / 16 = 1 MHz
     * SYSCLK    = 1 MHz * PLLN / 4
     */
    if (target == CPU_FREQ_16MHZ)
    {
        /* HSI directly supplies SYSCLK */
        return CPU_FREQ_OK;
    }

    pll_n = CPU_GetPLLMultiplier(target);

    RCC->PLLCFGR =
        (16U << 0)  |  /* PLLM */
        (pll_n << 6) |  /* PLLN */
        (1U << 16)  |  /* PLLP = 4 */
        (0U << 22)  |  /* PLL source = HSI */
        (4U << 24);    /* PLLQ = 4 */

    /* Enable PLL */
    RCC->CR |= (1U << 24);

    if (!CPU_WaitForBits(&RCC->CR,
                         (1U << 25), (1U << 25)))
    {
        return CPU_FREQ_ERROR_TIMEOUT;
    }

    /* Select PLL as system clock: SW = 10 */
    RCC->CFGR = (RCC->CFGR & ~3U) | 2U;

    /* Wait until hardware confirms PLL is SYSCLK */
    if (!CPU_WaitForBits(&RCC->CFGR,
                         (3U << 2), (2U << 2)))
    {
        return CPU_FREQ_ERROR_TIMEOUT;
    }

    return CPU_FREQ_OK;
}

/* Return the configured system clock in Hz */
uint32_t CPU_Frequency_Get(void)
{
    uint32_t source = (RCC->CFGR >> 2) & 3U;

    /* HSI selected directly */
    if (source == 0U)
    {
        return CPU_HSI_HZ;
    }

    /* HSE selected directly (not supported yet) */
    if (source == 1U)
    {
        return 0U;
    }

    /* PLL selected */
    if (source == 2U)
    {
        uint32_t pll_m = RCC->PLLCFGR & 0x3FU;
        uint32_t pll_n = (RCC->PLLCFGR >> 6) & 0x1FFU;
        uint32_t pll_p =
            (((RCC->PLLCFGR >> 16) & 3U) + 1U) * 2U;

        uint32_t pll_source = (RCC->PLLCFGR >> 22) & 1U;

        /* This library currently supports HSI as the PLL input */
        if (pll_source != 0U)
        {
            return 0U;
        }

        if (pll_m == 0U || pll_p == 0U)
        {
            return 0U;
        }

        return (uint32_t)(((uint64_t)CPU_HSI_HZ * pll_n) /
                          ((uint64_t)pll_m * pll_p));
    }

    return 0U;
}



uint32_t CPU_Frequency_GetAPB1(void)
{
    uint32_t sysclk = CPU_Frequency_Get();
    uint32_t hpre;
    uint32_t ppre1;
    uint32_t hclk;
    uint32_t pclk1;

    if (sysclk == 0U)
        return 0U;

    /* AHB prescaler */
    hpre = (RCC->CFGR >> 4U) & 0xFU;

    if (hpre < 8U)
        hclk = sysclk;
    else
        hclk = sysclk >> (hpre - 6U);

    /* APB1 prescaler */
    ppre1 = (RCC->CFGR >> 10U) & 0x7U;

    if (ppre1 < 4U)
        pclk1 = hclk;
    else
        pclk1 = hclk >> (ppre1 - 3U);

    return pclk1;
}


void CPU_StartCycleCounter(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}


uint32_t CPU_CycleCounter_Get(void){
    return DWT->CYCCNT;
}


