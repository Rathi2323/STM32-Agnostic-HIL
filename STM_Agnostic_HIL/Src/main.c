#include "hal_io.h"
#include "stm32f4xx.h"

int main(void) {
	HAL_Init();
    while (1) {
        //run_system_tick();// Calling agnostic logic
        __WFI();
    }
}
