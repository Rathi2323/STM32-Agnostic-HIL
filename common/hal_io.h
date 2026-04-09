#ifndef HAL_IO_H
#define HAL_IO_H

#include <stdint.h>

typedef enum {
	IO_LOW = 0,
	IO_HIGH = 1
}IO_State;

typedef void (*HAL_InterruptHandler)(void);

void run_system_tick(void);
void HAL_Init(void);
void HAL_GPIO_WritePin(uint8_t pin, IO_State state);
IO_State HAL_GPIO_ReadPin(uint8_t pin);
IO_State HAL_GetMockPinState(uint8_t pin);
void HAL_UART_Print(const char* str);
void HAL_SetMockSensor(uint16_t);
uint16_t HAL_ReadSensor(void);

void HAL_SIM_Reset(void);
void HAL_SIM_RegisterIRQ(uint8_t irq, uint8_t priority, HAL_InterruptHandler handler);
void HAL_SIM_TriggerIRQ(uint8_t);

#endif

