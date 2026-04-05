#include "hal_io.h"
#include <stdio.h>
#include <string.h>

extern char last_uart_message[256];
void run_system_tick(void);

int tests_passed = 0;
int tests_failed = 0;

void test_sensor_above_threshold(void)
{
	HAL_SetMockSensor(4000);
	run_system_tick();
	if(strcmp(last_uart_message, "Warning! Sensor value exceeded threshold!!")==0)
	{
		printf("\n[PASS] sensor above threshold");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] sensor above threshold");	
		tests_failed++;
	}
}

void test_sensor_below_threshold(void)
{
	HAL_SetMockSensor(100);
	run_system_tick();
	if(strcmp(last_uart_message, "Warning! Sensor value exceeded threshold!!")!=0)
	{
		printf("\n[PASS] sensor below threshold");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] sensor below threshold");	
		tests_failed++;
	}
}

void test_boundary_value(void)
{
	HAL_SetMockSensor(3000);
	run_system_tick();
	if(strcmp(last_uart_message, "Warning! Sensor value exceeded threshold!!")!=0)
	{
		printf("\n[PASS] sensor at boundary value");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] sensor at boundary value");	
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
