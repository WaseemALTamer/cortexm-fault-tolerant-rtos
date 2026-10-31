#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdint.h>


typedef struct Task{
    uint32_t* stack_pointer_start;
    uint32_t* stack_size;
    uint32_t* stack_position;
    uint8_t is_running;
    uint8_t is_finished;
} Task;





#endif