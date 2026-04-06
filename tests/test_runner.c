#include "hal_io.h"
#include <stdio.h>
#include <string.h>

extern char last_uart_message[256];
void run_system_tick(void);

int tests_passed = 0;
int tests_failed = 0;

int temptoraw(float temp)
{
	float voltage = (temp-25)*0.0025 + 0.76;
	float raw = voltage * 4096 / 3.3;
	return (int)raw;
}

void test_sensor_above_threshold(void)
{
	int raw = temptoraw(50.0f);
	HAL_SetMockSensor(raw);
	run_system_tick();
	if(HAL_GetMockPinState(13)==IO_HIGH)
	{
		printf("\n[PASS] LED ON above threshold");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] LED ON above threshold");	
		tests_failed++;
	}
}

void test_sensor_below_threshold(void)
{
	int raw = temptoraw(10.0f);
	HAL_SetMockSensor(raw);
	run_system_tick();
	if(HAL_GetMockPinState(13)==IO_LOW)
	{
		printf("\n[PASS] LED OFF below threshold");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] LED OFF below threshold");	
		tests_failed++;
	}
}

void test_boundary_value(void)
{
	int raw = temptoraw(40.0f);
	HAL_SetMockSensor(raw);
	run_system_tick();
	if(HAL_GetMockPinState(13)==IO_LOW)
	{
		printf("\n[PASS] LED OFF at boundary value");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] LED OFF at boundary value");	
		tests_failed++;
	}
}


int main(void)
{
	test_sensor_above_threshold();
	test_sensor_below_threshold();
	test_boundary_value();
	printf("\n===Results: %d passed, %d failed===\n", tests_passed, tests_failed);
	return tests_failed;
}
