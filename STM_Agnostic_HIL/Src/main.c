#include "hal_io.h"
#include "stm32f4xx.h"
void run_system_tick(void);
int main(void) {
    // 1. Enable Clocks for GPIOD (LEDs) and GPIOA (Button)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN;

    // 2. Configure PD12 as Output
    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |= (1 << 24);

    // 3. Configure PA0 as Input (Button)
    GPIOA->MODER &= ~(3 << 0);
    // 4. Configure PD13 as output (for sensor)
    GPIOD->MODER &= ~(3 << 26);  // PD13 — pin 13 × 2 = bit 26
    GPIOD->MODER |=  (1 << 26);
    while (1) {
        run_system_tick();// This calls your agnostic logic!
        for(volatile int i=0; i<100000; i++);
    }
}
