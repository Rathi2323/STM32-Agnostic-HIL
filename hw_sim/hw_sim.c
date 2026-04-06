#include "hal_io.h"
#include <stdio.h>
#include <string.h>

void HAL_Init(void){};

char last_uart_message[256];

// We "Simulate" a button press for testing
static IO_State mock_button = IO_LOW;
static uint16_t mock_sensor_value=0;
static IO_State mock_pin_states[16]={0};

void HAL_GPIO_WritePin(uint8_t pin, IO_State state) 
{
    printf("\n[SIM] GPIO Pin %d is now %s\n", pin, (state == IO_HIGH ? "HIGH" : "LOW"));
	mock_pin_states[pin] = state;
}

IO_State HAL_GPIO_ReadPin(uint8_t pin) {
    return mock_button; // Controlled by our test script
}
IO_State HAL_GetMockPinState(uint8_t pin)
{
	return mock_pin_states[pin];
}
void HAL_UART_Print(const char* str) {
    printf("[UART] %s", str);
	strcpy(last_uart_message,str);
}

void HAL_SetMockSensor(uint16_t value)
{
	mock_sensor_value = value;
	memset(&last_uart_message, 0, sizeof(last_uart_message)); //clear previous message
}

uint16_t HAL_ReadSensor(void)
{
	return mock_sensor_value;
}
