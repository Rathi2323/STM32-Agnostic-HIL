#ifndef HAL_IO_H
#define HAL_IO_H

#include <stdint.h>

typedef enum {
	IO_LOW = 0,
	IO_HIGH = 1
}IO_State;

void run_system_tick(void);
void HAL_Init(void);
void HAL_GPIO_WritePin(uint8_t pin, IO_State state);
IO_State HAL_GPIO_ReadPin(uint8_t pin);
IO_State HAL_GetMockPinState(uint8_t pin);
void HAL_UART_Print(const char* str);
void HAL_SetMockSensor(uint16_t);
uint16_t HAL_ReadSensor(void);

#endif

