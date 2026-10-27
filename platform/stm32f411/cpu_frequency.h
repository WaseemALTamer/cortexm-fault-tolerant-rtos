
#ifndef CPU_FREQUENCY_H
#define CPU_FREQUENCY_H

#include <stm32f411xe.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HSI oscillator frequency */
#define CPU_HSI_HZ       16000000U

/* Prevent waiting forever for hardware flags */
#define CPU_TIMEOUT      1000000U

typedef enum{
    CPU_FREQ_16MHZ  = 16000000U,
    CPU_FREQ_48MHZ  = 48000000U,
    CPU_FREQ_84MHZ  = 84000000U,
    CPU_FREQ_100MHZ = 100000000U
} CPU_Frequency;

typedef enum{
    CPU_FREQ_OK = 0,
    CPU_FREQ_ERROR_INVALID,
    CPU_FREQ_ERROR_TIMEOUT
} CPU_FrequencyStatus;

/* Configure the MCU system clock */
CPU_FrequencyStatus CPU_Frequency_Init(CPU_Frequency frequency);

/* Return the configured system clock in Hz */
uint32_t CPU_Frequency_Get(void);

uint32_t CPU_Frequency_GetAPB1(void);

#ifdef __cplusplus
}
#endif

#endif
