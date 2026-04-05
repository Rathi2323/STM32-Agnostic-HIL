#include "hal_io.h"
#include "stm32f4xx.h" // The header that defines registers like RCC and GPIOD
#include <string.h>

// --- Debug Buffer Logic ---
// We create a global array that lives in RAM.
// You can view this in the "Expressions" window in STM32CubeIDE.
volatile char debug_log_buffer[512];
uint16_t log_index = 0;
static uint16_t stubbed_value = 0;

void HAL_GPIO_WritePin(uint8_t pin, IO_State state) {
    if (state == IO_HIGH) {
        GPIOD->BSRR = (1 << pin);  // Atomic set
    } else {
        GPIOD->BSRR = (1 << (pin + 16)); // Atomic reset
    }
}

IO_State HAL_GPIO_ReadPin(uint8_t pin) {
    // Read PA0 (User Button)
    if (GPIOA->IDR & (1 << pin)) {
        return IO_HIGH;
    }
    return IO_LOW;
}

void HAL_UART_Print(const char* str) {
    // Since we don't have a UART converter, we do nothing here for now.
    // In Phase 3, we will use a "Debug Buffer".
	uint16_t len = strlen(str);

	    // Check if the message fits in the remaining buffer space
	    if (log_index + len < 512) {
	        memcpy((char*)&debug_log_buffer[log_index], str, len);
	        log_index += len;
	    } else {
	        // If buffer is full, reset to start (Circular Buffer behavior)
	        log_index = 0;
	        memset((char*)debug_log_buffer, 0, 512); // Clear old data
	        memcpy((char*)&debug_log_buffer[log_index], str, len);
	        log_index += len;
	    }
}

uint16_t HAL_ReadSensor(void)
{
	//Todo: wire up real ADC(PA1, ADC1 channel 1) in future phase
	return stubbed_value;
}
