#include "stm32f446xx_gpio_driver.h"
#include <stdio.h>

void I2C1_Init(void) {
    // Enable clock for GPIOB and I2C1
    RCC->AHB1ENR |= (1 << 1);
    RCC->APB1ENR |= (1 << 21);

    // Set PB8 and PB9 to alternate function mode
    GPIOB->MODER &= ~(0xF << 16);
    GPIOB->MODER |= (0xA << 16);
    GPIOB->OTYPER |= (1 << 8) | (1 << 9); // Open-drain
    GPIOB->AFR[1] |= (4 << 0) | (4 << 4); // AF4 for I2C1

    // Configure I2C1: 100kHz standard mode
    I2C1->CR2 |= 16; // 16MHz clock
    I2C1->CCR = 80;
    I2C1->TRISE = 17;
    I2C1->CR1 |= (1 << 0); // Enable I2C1
}

void UART2_Init(void) {
    // Enable clock for GPIOA and USART2
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 17);

    // Set PA2 to alternate function
    GPIOA->MODER &= ~(3 << 4);
    GPIOA->MODER |= (2 << 4);
    GPIOA->AFR[0] |= (7 << 8);

    // Configure USART2: 9600 baud rate
    USART2->BRR = 0x683;
    USART2->CR1 |= (1 << 3) | (1 << 13);
}

void UART_SendString(const char* str) {
    while (*str) {
        while (!(USART2->SR & (1 << 7)));
        USART2->DR = *str++;
    }
}

int main(void) {
    I2C1_Init();
    UART2_Init();
    char buffer[50];

    while (1) {
        // Pseudo-code for reading temperature from sensor
        float temp = 25.5f; // Replace with actual I2C read logic

        sprintf(buffer, "Temperature: %.2f C\r\n", temp);
        UART_SendString(buffer);

        // Add appropriate delay here
        for(int i = 0; i < 1000000; i++);
    }
}
