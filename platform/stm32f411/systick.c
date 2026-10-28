#include <stm32f411xe.h>
#include "systick.h"
#include "cpu_frequency.h"



int SysTick_Init(uint32_t frequency_hz){
    uint32_t ticks;

    uint32_t system_clock = CPU_Frequency_Get();

    if (frequency_hz == 0U || frequency_hz > system_clock){
        return -1;
    }

    ticks = system_clock / frequency_hz;

    if ((system_clock % frequency_hz) != 0U ||
        ticks == 0U ||
        ticks > 0x01000000U)
    {
        return -1;
    }


    SysTick->CTRL = 0U;


    SysTick->LOAD = ticks - 1U;
    SysTick->VAL  = 0U;



    NVIC_SetPriority(SysTick_IRQn, 15U);


    SysTick->CTRL =
        SysTick_CTRL_CLKSOURCE_Msk |
        SysTick_CTRL_TICKINT_Msk   |
        SysTick_CTRL_ENABLE_Msk;

    return 0;
}