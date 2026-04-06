#include "hal_io.h"
#include "stm32f4xx.h" // The header that defines registers like RCC and GPIOD
#include <string.h>
// ─── Forward declarations ─────────────────────────
// these functions are defined below, declared here so
// HAL_Init() can call them before they're defined
void uart2_init(void);
void uart2_send_char(char c);
void uart2_send_string(const char* str);
void ADC_Init(void);

// --- Debug Buffer Logic ---
//global array that lives in RAM.
volatile char debug_log_buffer[512];
uint16_t log_index = 0;

void HAL_Init(void)
{
	// Enable FPU — MUST be first before any float operations!
	SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));  // enable CP10 and CP11
    // 1. Enable Clocks for GPIOD (LEDs) and GPIOA (Button)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOAEN;

    // 2. Configure PD12 as Output
    GPIOD->MODER &= ~(3 << 24);
    GPIOD->MODER |= (1 << 24);

    // 3. Configure PA0 as Input (Button)
    GPIOA->MODER &= ~(3 << 0);
    // 4. Configure PD13 as output (for sensor)
    GPIOD->MODER &= ~(3 << 26);  // PD13 — pin 13 × 2 = bit 26
    GPIOD->MODER |=  (1 << 26);

    uart2_init();   // ← UART first so we can debug

    ADC_Init();     // ← ADC after

}

// ─── UART ────────────────────────────────────────
void uart2_send_char(char c) {
    while (!(USART2->SR & (1 << 7)));
    USART2->DR = c;
}

void uart2_send_string(const char* str) {
    while (*str) {
        uart2_send_char(*str++);
    }
}

void uart2_init(void) {
    RCC->AHB1ENR  |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR  |= RCC_APB1ENR_USART2EN;
    GPIOA->MODER  &= ~(3 << 4);
    GPIOA->MODER  |=  (2 << 4);
    GPIOA->AFR[0] |=  (7 << 8);
    USART2->BRR    = 0x0683;
    USART2->CR1   |= (1 << 3);
    USART2->CR1   |= (1 << 13);
}

// ─── ADC ─────────────────────────────────────────
void ADC_Init(void)
{
	//Enable ADC1 clock
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	for(volatile int i = 0; i < 10000; i++);
	//Enable temperature sensor and VREF-common control register
	ADC->CCR |= ADC_CCR_TSVREFE;
	for(volatile int i = 0; i < 100000; i++);
	//Set ADC resolution to 12-bit default
	ADC1->CR1 &= ~ADC_CR1_RES;
	//Set sample time for channel 16(temp sensor)
	//needs minimum 10us - so use 480 cycles(18:20 - 111) slowest
	ADC1->SMPR1 |= (7<<18);
	//Set channel 16 as first conversion
	ADC1->SQR3 = 16;
	//conversion mode- can be single or continuous
	//single - Read-convert-stop
	//continuous - Read-convert-convert-convert..EOC
	ADC1->CR2 &= ~ADC_CR2_CONT;
	//Turn on ADC
	ADC1->CR2 |= ADC_CR2_ADON;
	//Wait for ADC to stabilize(delay)
	for(volatile int i=0; i<100000;i++);
}

// ─── HAL Implementation ──────────────────────────
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
	uart2_send_string(str);// sending over real UART

	//write to debug buffer
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
	//start conversion
	ADC1->CR2 |= ADC_CR2_SWSTART;
	uint32_t timeout = 100000;
	//Wait until conversion complete (EOC has to become 1)
	while(!(ADC1->SR & ADC_SR_EOC))
	{
		if(--timeout==0)
		{
			uart2_send_string("ADC timeout!\r\n");
			return 0;
		}
	}
	//Return raw value from DR
	return (uint16_t)ADC1->DR;
}
