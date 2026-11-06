#include "stm32f446xx_gpio_driver.h"
#include <stdio.h>

void ADC_Init(void) {
    // Enable clock for GPIOA
    RCC->AHB1ENR |= (1 << 0);
    // Enable clock for ADC1
    RCC->APB2ENR |= (1 << 8);

    // Set PA1 to analog mode
    GPIOA->MODER |= (3 << (1 * 2));

    // Configure ADC1: 12-bit resolution
    ADC1->CR1 &= ~(3 << 24);
    // Continuous conversion mode
    ADC1->CR2 |= (1 << 1);
    // Enable ADC
    ADC1->CR2 |= (1 << 0);
}

uint16_t ADC_Read(void) {
    // Start conversion
    ADC1->CR2 |= (1 << 30);
    // Wait until conversion is complete
    while (!(ADC1->SR & (1 << 1)));
    return ADC1->DR;
}

int main(void) {
    ADC_Init();
    char buffer[50];

    while (1) {
        uint16_t adc_val = ADC_Read();
        // Convert ADC value to temperature (approximate for LM35)
        float voltage = (adc_val * 3.3f) / 4095.0f;
        float temp = voltage * 100.0f;

        sprintf(buffer, "Temperature: %.2f C\r\n", temp);
        // Add UART transmission logic here
    }
}
