#include "stm32f446xx_gpio_driver.h"
#include <stdio.h>

void SPI1_Init(void) {
    // Enable clock for GPIOA and SPI1
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB2ENR |= (1 << 12);

    // Set PA5, PA6, PA7 to alternate function mode (SPI1 SCK, MISO, MOSI)
    GPIOA->MODER &= ~((3 << 10) | (3 << 12) | (3 << 14));
    GPIOA->MODER |= ((2 << 10) | (2 << 12) | (2 << 14));

    // Set alternate function to AF5 for SPI1
    GPIOA->AFR[0] &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOA->AFR[0] |= ((5 << 20) | (5 << 24) | (5 << 28));

    // Configure SPI1: Master mode, software slave management, baud rate fPCLK/4
    SPI1->CR1 = (1 << 2) | (1 << 8) | (1 << 9);
    SPI1->CR1 |= (1 << 6); // Enable SPI1
}

void UART2_Init(void) {
    RCC->AHB1ENR |= (1 << 0);
    RCC->APB1ENR |= (1 << 17);

    GPIOA->MODER &= ~(3 << 4);
    GPIOA->MODER |= (2 << 4);
    GPIOA->AFR[0] |= (7 << 8);

    USART2->BRR = 0x683;
    USART2->CR1 |= (1 << 3) | (1 << 13);
}

void UART_SendString(const char* str) {
    while (*str) {
        while (!(USART2->SR & (1 << 7)));
        USART2->DR = *str++;
    }
}

uint8_t SPI1_TransmitReceive(uint8_t data) {
    while (!(SPI1->SR & (1 << 1))); // Wait until TX buffer is empty
    SPI1->DR = data;
    while (!(SPI1->SR & (1 << 0))); // Wait until RX buffer is not empty
    return SPI1->DR;
}

int main(void) {
    SPI1_Init();
    UART2_Init();
    char buffer[50];

    while (1) {
        // Send a test pattern via SPI and receive response
        uint8_t received_data = SPI1_TransmitReceive(0xA5);

        sprintf(buffer, "SPI Received: 0x%X\r\n", received_data);
        UART_SendString(buffer);

        for(int i = 0; i < 1000000; i++);
    }
}
