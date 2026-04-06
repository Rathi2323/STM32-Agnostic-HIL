#include "hal_io.h"
#include "stm32f4xx.h"

int main(void) {
	HAL_Init();
    while (1) {
        run_system_tick();// This calls your agnostic logic!
        for(volatile int i=0; i<100000; i++);
    }
}
