#include "hal_io.h"
#include <stdio.h>
#include <string.h>

extern char last_uart_message[256];
void run_system_tick(void);

int tests_passed = 0;
int tests_failed = 0;

enum
{
	SIM_IRQ_TIM2=2,
	SIM_IRQ_EXTI0=6
};

static char irq_trace[32];
static int irq_trace_index=0;

static void irq_trace_push(char marker)
{
	if(irq_trace_index < (int)(sizeof(irq_trace)-1))
	{
		irq_trace[irq_trace_index++] = marker;
		irq_trace[irq_trace_index] = '\0';
	}
}

static void reset_irq_trace(void)
{
	memset(irq_trace, 0, sizeof(irq_trace));
	irq_trace_index=0;
}

static void low_priority_irq_handler(void)
{
	irq_trace_push('A');
	HAL_SIM_TriggerIRQ(SIM_IRQ_EXTI0);
	irq_trace_push('C');
}

static void high_priority_irq_handler(void)
{
	irq_trace_push('B');                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       

}

static void higher_priority_irq_handler(void)
{
	irq_trace_push('X');
	HAL_SIM_TriggerIRQ(SIM_IRQ_TIM2);
	irq_trace_push('Y');
}

static void lower_priority_irq_handler(void)
{
	irq_trace_push('Z');
}

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

void test_nested_interrupt_preempts_lower_priority(void)
{
	HAL_SIM_Reset();
	reset_irq_trace();
	HAL_SIM_RegisterIRQ(SIM_IRQ_TIM2,2,low_priority_irq_handler);
	HAL_SIM_RegisterIRQ(SIM_IRQ_EXTI0,1,high_priority_irq_handler);
	HAL_SIM_TriggerIRQ(SIM_IRQ_TIM2);
	if(strcmp(irq_trace,"ABC")==0)
	{
		printf("\n[PASS] Higher Priority IRQ preempts lower priority IRQ");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] Higher Priority IRQ preempts lower priority IRQ");
		tests_failed++;
	}
}

void test_lower_priority_irq_waits_until_return(void)
{
	HAL_SIM_Reset();
	reset_irq_trace();
	HAL_SIM_RegisterIRQ(SIM_IRQ_EXTI0, 1, higher_priority_irq_handler);
	HAL_SIM_RegisterIRQ(SIM_IRQ_TIM2, 2, lower_priority_irq_handler);

	HAL_SIM_TriggerIRQ(SIM_IRQ_EXTI0);

	if (strcmp(irq_trace, "XYZ") == 0)
	{
		printf("\n[PASS] Lower priority IRQ stays pending until ISR exits");
		tests_passed++;
	}
	else
	{
		printf("\n[FAIL] Lower priority IRQ stays pending until ISR exits");
		tests_failed++;
	}
}

int main(void)
{
	test_sensor_above_threshold();
	test_sensor_below_threshold();
	test_boundary_value();
	test_nested_interrupt_preempts_lower_priority();
	test_lower_priority_irq_waits_until_return();
	printf("\n===Results: %d passed, %d failed===\n", tests_passed, tests_failed);
	return tests_failed;
}
