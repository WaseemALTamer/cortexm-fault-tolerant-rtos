#ifndef TICK_CONTROLLER_H
#define TICK_CONTROLLER_H

#include <stdint.h>
#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

void TickController_init(void);

uint32_t TickController_GetTick(void);

uint32_t TickController_get_current_time(void);

void TickController_delay_us(uint32_t microseconds);

uint32_t TickController_GetUptimeSeconds(void);

void TickController_OnTick(void);

#ifdef __cplusplus
}
#endif

#endif /* TICK_CONTROLLER_H */