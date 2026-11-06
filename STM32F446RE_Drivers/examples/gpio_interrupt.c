/*
 * gpio_interrupt.c
 *
 *  Created on: Dec 9, 2025
 *      Author: neeraj
 */


#include<string.h>
#include "stm32f446xx_gpio_driver.h"

#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW

void delay(void)
{
    // ~200ms delay at 16MHz
    for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GPIOBtn;

    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GPIOBtn, 0, sizeof(GPIOBtn));

    // LED configuration for User LED (PA5)
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&GpioLed);

    // Button configuration for User Button (PC13)
    GPIOBtn.pGPIOx = GPIOC;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_PeriClockControl(GPIOC, ENABLE);
    GPIO_Init(&GPIOBtn);

    GPIO_WriteToOutputPin(GPIOA, GPIO_PIN_NO_5, GPIO_PIN_RESET);

    // IRQ configurations
    GPIO_IRQPriorityConfig(IRQ_NO_EXTI15_10, NVIC_IRQ_PRI15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXTI15_10, ENABLE);

    while(1);
}

void EXTI15_10_IRQHandler(void)
{
    GPIO_IRQHandling(GPIO_PIN_NO_13);
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
