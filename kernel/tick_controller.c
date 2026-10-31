#include "tick_controller.h"
#include "constants.h"
#include "time.h"



#include <cpu_frequency.h>
#include <systick.h>



static volatile uint32_t tick_count = 0U;
static volatile uint32_t uptime_seconds = 0U;
static uint32_t subsecond_ticks = 0U;

uint32_t TickController_GetTick(void){
    return tick_count;
}




void TickController_init(){
    SysTick_Init(SYSTICK_FREQUENCY_HZ);
    CPU_StartCycleCounter(); // start the cpu cycle counter
}

uint32_t TickController_get_current_time(){


}


void TickController_delay_us(uint32_t microseconds){
    const uint32_t cycles_per_us = CPU_Frequency_Get() / 1000000U;
    const uint32_t start = CPU_CycleCounter_Get();
    const uint32_t cycles = cycles_per_us * microseconds;

    while ((uint32_t)(CPU_CycleCounter_Get() - start) < cycles){

    }
}



void TickController_OnTick(){
    tick_count++;

    subsecond_ticks++;

    if (subsecond_ticks >= 1000U){
        subsecond_ticks = 0U;
        uptime_seconds++;
    }

    // this will also trigger the task manager for us as well
    // but it is not created yet it needs to be implemented

}


uint32_t TickController_GetUptimeSeconds(void)
{
    /*
    uint32_t primask = __get_PRIMASK(); // save current interupt state
    __disable_irq(); // disable the interuptts

    uint32_t result = uptime_seconds; // read the time and store it in a local safe var

    __set_PRIMASK(primask); // reset the interput state to what it was before
    return result;
    
    
    */


    // the interrupt will not update the value while you are reading it
    // so you can simply fly the code
    return uptime_seconds;

}


// this will handel the interupt
void SysTick_Handler(void){
    TickController_OnTick();
}