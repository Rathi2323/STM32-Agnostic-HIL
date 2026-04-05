#include "hal_io.h"

//static uint8_t button_press = 0;
void run_system_tick(void) {
    // Read the User Button (Pin 0)
    if (HAL_GPIO_ReadPin(0) == IO_HIGH) {
        // Turn on the LED (Pin 12)
        HAL_GPIO_WritePin(12, IO_HIGH);
        HAL_UART_Print("Status: Button Pressed - LED ON\n");
        }
   else
   {
        HAL_GPIO_WritePin(12, IO_LOW);
    }

    uint16_t sensor = HAL_ReadSensor();

    if(sensor>3000)
    {
    	HAL_GPIO_WritePin(13, IO_HIGH);
    	HAL_UART_Print("Warning! Sensor value exceeded threshold!!");
    }
	else if(sensor == 3000)
	{
		HAL_GPIO_WritePin(13, IO_LOW);
	}
    else
    {
    	HAL_GPIO_WritePin(13, IO_LOW);
    }
}


