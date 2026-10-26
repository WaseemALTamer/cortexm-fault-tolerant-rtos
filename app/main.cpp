#include <gpio.h>
#include <stm32f411xe.h>



#include "main.h"






void init(void){
    GPIO_Init(GPIOA, 5, GPIO_MODE_OUTPUT, GPIO_LOW);
}

void loop(void){


    GPIO_Toggle(GPIOA, 5);

    for (volatile uint32_t i = 0;
            i < 500000U;
            i++)
    {

    }


}

