#include "hal_io.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

void HAL_Init(void){};

char last_uart_message[256];

// We "Simulate" a button press for testing
static IO_State mock_button = IO_LOW;
static uint16_t mock_sensor_value=0;
static IO_State mock_pin_states[16]={0};

#define SIM_IRQ_COUNT 16

typedef struct
{
    uint8_t enabled;
    uint8_t pending;
    uint8_t priority;
    HAL_InterruptHandler handler;
} SimIRQ;

static SimIRQ sim_irqs[SIM_IRQ_COUNT];
static int current_irq_stack[SIM_IRQ_COUNT];
static uint8_t current_irq_depth = 0;

static void HAL_SIM_RenderVirtualLed(uint8_t pin, IO_State state)
{
    printf("[VIRTUAL LED P%d] [%s] %s\n",
           pin,
           state == IO_HIGH ? "####" : "    ",
           state == IO_HIGH ? "ON" : "OFF");
}

static int HAL_SIM_GetCurrentPriority(void)
{
    if (current_irq_depth == 0) {
        return INT_MAX;
    }

    return sim_irqs[current_irq_stack[current_irq_depth - 1]].priority;
}

static int HAL_SIM_FindRunnableIRQ(void)
{
    int best_irq = -1;
    int best_priority = HAL_SIM_GetCurrentPriority();

    for (int irq = 0; irq < SIM_IRQ_COUNT; irq++) {
        if (!sim_irqs[irq].enabled || !sim_irqs[irq].pending || sim_irqs[irq].handler == NULL) {
            continue;
        }

        if (sim_irqs[irq].priority < best_priority) {
            best_priority = sim_irqs[irq].priority;
            best_irq = irq;
        }
    }

    return best_irq;
}

static void HAL_SIM_DispatchPending(void)
{
    int next_irq;

    while ((next_irq = HAL_SIM_FindRunnableIRQ()) >= 0) {
        sim_irqs[next_irq].pending = 0;
        current_irq_stack[current_irq_depth++] = next_irq;
        sim_irqs[next_irq].handler();
        current_irq_depth--;
    }
}

void HAL_GPIO_WritePin(uint8_t pin, IO_State state) 
{
    printf("\n[SIM] GPIO Pin %d is now %s\n", pin, (state == IO_HIGH ? "HIGH" : "LOW"));
	mock_pin_states[pin] = state;
    HAL_SIM_RenderVirtualLed(pin, state);
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

void HAL_SIM_Reset(void)
{
    memset(sim_irqs, 0, sizeof(sim_irqs));
    memset(current_irq_stack, 0, sizeof(current_irq_stack));
    current_irq_depth = 0;
    mock_button = IO_LOW;
    mock_sensor_value = 0;
    memset(mock_pin_states, 0, sizeof(mock_pin_states));
    memset(last_uart_message, 0, sizeof(last_uart_message));
}

void HAL_SIM_RegisterIRQ(uint8_t irq, uint8_t priority, HAL_InterruptHandler handler)
{
    if (irq >= SIM_IRQ_COUNT) {
        return;
    }

    sim_irqs[irq].enabled = 1;
    sim_irqs[irq].pending = 0;
    sim_irqs[irq].priority = priority;
    sim_irqs[irq].handler = handler;
}

void HAL_SIM_TriggerIRQ(uint8_t irq)
{
    if (irq >= SIM_IRQ_COUNT || !sim_irqs[irq].enabled || sim_irqs[irq].handler == NULL) {
        return;
    }

    sim_irqs[irq].pending = 1;
    HAL_SIM_DispatchPending();
}
