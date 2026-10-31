#include "task_manager.h"
#include "constants.h"

#include <cpu_frequency.h> // this needs to change, this allows you to use the function __get_PSP()



Task tasks[MAX_TASKS];

Task *current_task;
int current_task_index = 0;




void switch_tasks(Task new_task){
    uint32_t *task_sp = (uint32_t *)__get_PSP();
    // save current task
        // save the current task_sp to the task stack
        // save r4-r11 on the same stack 

    // load the new task
        // load the saved task_sp from stack
        // point the current stack to task stack 
        // load the current program counter
        // load r4-r11 from the stack

    //exit
}



void update(){

    // check tasks and if they are expired

}



void PendSV_Handler(void){
    update();
}