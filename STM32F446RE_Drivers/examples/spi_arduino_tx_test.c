/*
 * spi_arduino_tx_test.c
 *
 *  Created on: Dec 12, 2025
 *      Author: neeraj
 */

#include <string.h>
#include "stm32f446xx_gpio_driver.h"
#include "stm32f446xx_spi_driver.h"

void delay(void)
{
    for(uint32_t i = 0 ; i < 500000 ; i ++);
}

void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins;
    memset(&SPIPins, 0, sizeof(SPIPins));

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // SCLK
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    // MOSI
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    // NSS
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}

void SPI2_Inits(void)
{
    SPI_Handle_t SPI2handle;
    memset(&SPI2handle, 0, sizeof(SPI2handle));

    SPI2handle.pSPIx = SPI2;
    SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV32;
    SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI;

    SPI_Init(&SPI2handle);
}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GPIOBtn;
    memset(&GPIOBtn, 0, sizeof(GPIOBtn));

    // Button configuration for User Button (PC13)
    GPIOBtn.pGPIOx = GPIOC;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_Init(&GPIOBtn);
}

int main(void)
{
    char user_data[] = "An Arduino Uno board is best suited for beginners who have just started using microcontrollers.";

    GPIO_ButtonInit();
    SPI2_GPIOInits();
    SPI2_Inits();
    SPI_SSOEConfig(SPI2, ENABLE);

    while(1)
    {
        while( ! GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

        delay();

        SPI_PeripheralControl(SPI2, ENABLE);

        uint8_t dataLen = strlen(user_data);
        SPI_SendData(SPI2, &dataLen, 1);
        SPI_SendData(SPI2, (uint8_t*)user_data, strlen(user_data));

        while( SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG) );

        SPI_PeripheralControl(SPI2, DISABLE);
    }
    return 0;
}
