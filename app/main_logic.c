#include "hal_io.h"
#include <stdio.h>
#include <string.h>

void run_system_tick(void)
{
    // --- ADC Temperature Sensor ---
    uint16_t raw = HAL_ReadSensor();

    // convert raw to temperature
    float voltage = (float)raw * 3.3f / 4096.0f;
    float temp_c  = (voltage - 0.76f) / 0.0025f + 25.0f;

    // convert float to string (no printf on STM32 bare metal)
    char buf[50];
    int temp_int = (int)temp_c;
    int temp_frac = (int)((temp_c-temp_int)*10);

    //formatting manually
    buf[0] = '\0';
    sprintf(buf, "Chip Temp: %d.%d C\r\n",temp_int, temp_frac);

    HAL_UART_Print(buf);

    //LED on pin 13 if temp above 40 degree C
    if(temp_c > 40.0f)
    {
    	HAL_GPIO_WritePin(13, IO_HIGH);
	}
    else
    {
    	HAL_GPIO_WritePin(13, IO_LOW);
    }
}


