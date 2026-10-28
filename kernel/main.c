#include "../app/main.h"


#include <uart.h>

#include "cpu_frequency.h"
#include "tick_controller.h"




// interuppt tick check the startup file to konw




int main(void){

    CPU_Frequency_Init(CPU_FREQ_100MHZ); // set the cpu frequency    
    TickController_init();


    init(); // excaute the app init function

    while (1){
        loop(); // loop through the system supper loop
    }

    return 0;
}