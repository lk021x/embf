/********************************************************************************
 * Copyright (C), 1997-2011, SUNGROW POWER SUPPLY CO., LTD. 
 * File name      :io_interface.c
 * Author         :Xu Shun'an
 * Date           :2011.08.04
 * Description    :IO控制器的端口初始化
 * Others         :None
 *-------------------------------------------------------------------------------
 * 2011-08-04 : 1.0.0 : xusa
 * Modification   : 初始代码编写。
 ********************************************************************************/

#include "io_interface.h"
extern void ETH_GPIO_Conf(void);
uint8_t device_search_by_id(uint8_t device_id);//内部函数，不暴露



/* 设备配置表 */
Device_cfg const device_cfg[] = 
{
#ifdef P_A_000092
    {.device_id = DEVICE_ID_EEPROM, .periph_bus = PERIPH_APB1, .periph_rccid = RCC_APB1Periph_SPI2, .periph_addr = SPI2},
    {.device_id = DEVICE_ID_FLASH, .periph_bus = PERIPH_APB1, .periph_rccid = RCC_APB1Periph_SPI3, .periph_addr = SPI3},
    {.device_id = DEVICE_ID_NET, .periph_bus = PERIPH_AHB1, .periph_rccid = RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx | RCC_AHB1Periph_ETH_MAC_Rx, .periph_addr = ETH},
#endif
};


/* GPIO口配置表 */
TGPIOA const gGPIOAll[] = 
{
#ifdef P_A_000092
    {.device_id = DEVICE_ID_EEPROM, .pin_type = PIN_GPIO_CS, .Base = GPIOB,  {.GPIO_Pin = GPIO_Pin_9,  .GPIO_Mode = GPIO_Mode_OUT,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_HIGH},     // SPI2_NSS_EEPROM
    {.device_id = DEVICE_ID_EEPROM, .af_periph = GPIO_AF_SPI2, .af_pinsource = GPIO_PinSource2, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_2, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SPI2_MISO_EEPROM
    {.device_id = DEVICE_ID_EEPROM, .af_periph = GPIO_AF_SPI2, .af_pinsource = GPIO_PinSource3, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_3, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SPI2_MOSI_EEPROM
    {.device_id = DEVICE_ID_EEPROM, .af_periph = GPIO_AF_SPI2, .af_pinsource = GPIO_PinSource10, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_10,  .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},   // SPI2_SCK_EEPROM
    
    {.device_id = DEVICE_ID_FLASH, .pin_type = PIN_GPIO_CS, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_15, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // SPI3_NSS_W25Qx
    {.device_id = DEVICE_ID_FLASH, .af_periph = GPIO_AF_SPI3, .af_pinsource = GPIO_PinSource10, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_10, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SPI3_SCK_W25Qx
    {.device_id = DEVICE_ID_FLASH, .af_periph = GPIO_AF_SPI3, .af_pinsource = GPIO_PinSource11, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_11, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SPI3_MISO_W25Qx
    {.device_id = DEVICE_ID_FLASH, .af_periph = GPIO_AF_SPI3, .af_pinsource = GPIO_PinSource12, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_12, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SPI3_MOSI_W25Qx    

    {.device_id = DEVICE_ID_EXTWDG, .pin_type = PIN_GPIO_EXTWDG_FEED, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_5, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // EXTWDG 
               
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_MISO,  .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_6, .GPIO_Mode = GPIO_Mode_IN,  .GPIO_PuPd = GPIO_PuPd_UP,   .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},                                // MISO_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_MOSI,  .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_5, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // MOSI_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_SCK,   .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_4, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // SCK_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_CS,    .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_3, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // NSS_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_RESET, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_2, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // RESET_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_START, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_1, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // START_ADS1247
    {.device_id = DEVICE_ID_ADS1247, .pin_type = PIN_GPIO_DRDY,  .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_0, .GPIO_Mode = GPIO_Mode_IN,  .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},                                 // DRDY_ADS1247

    {.device_id = DEVICE_ID_ATT7022B, .pin_type = PIN_GPIO_SCK,  .Base = GPIOA,  {.GPIO_Pin = GPIO_Pin_11,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // ATT7022B_SCK
    {.device_id = DEVICE_ID_ATT7022B, .pin_type = PIN_GPIO_MISO, .Base = GPIOG,  {.GPIO_Pin = GPIO_Pin_6,  .GPIO_Mode = GPIO_Mode_IN,  .GPIO_PuPd = GPIO_PuPd_UP,   .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},                               // ATT7022B_MISO
    {.device_id = DEVICE_ID_ATT7022B, .pin_type = PIN_GPIO_MOSI, .Base = GPIOC,  {.GPIO_Pin = GPIO_Pin_6,   .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // ATT7022B_MOSI
    {.device_id = DEVICE_ID_ATT7022B, .pin_type = PIN_GPIO_CS,   .Base = GPIOA,  {.GPIO_Pin = GPIO_Pin_12,    .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // ATT7022B_NSS
    {.device_id = DEVICE_ID_ATT7022B, .pin_type = PIN_GPIO_RESET,.Base = GPIOG,  {.GPIO_Pin = GPIO_Pin_8, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_HIGH},    // ATT7022B_RESET
    {.device_id = DEVICE_ID_ATT7022B, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_3, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},                                   // ATT7022B_REVP  未用

    {.device_id = DEVICE_ID_RTC, .pin_type = PIN_GPIO_SCK, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_8, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_OD, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // I2C1_SCL
    {.device_id = DEVICE_ID_RTC, .pin_type = PIN_GPIO_SDA, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_7, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_OD, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // I2C1_SDA

    {.device_id = DEVICE_ID_RS485_1, .pin_type = PIN_GPIO_485_EN, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_4, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // 485_EN1
    {.device_id = DEVICE_ID_RS485_1, .af_periph = GPIO_AF_USART1, .af_pinsource = GPIO_PinSource9, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_9, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // USART1_TX
    {.device_id = DEVICE_ID_RS485_1, .af_periph = GPIO_AF_USART1, .af_pinsource = GPIO_PinSource10, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_10, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},    // USART1_RX
        
    {.device_id = DEVICE_ID_RS485_2, .pin_type = PIN_GPIO_485_EN, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_10, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},    // 485_EN2
    {.device_id = DEVICE_ID_RS485_2, .af_periph = GPIO_AF_USART2, .af_pinsource = GPIO_PinSource5, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_5, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // UART2_TX
    {.device_id = DEVICE_ID_RS485_2, .af_periph = GPIO_AF_USART2, .af_pinsource = GPIO_PinSource6, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_6, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // UART2_RX

    {.device_id = DEVICE_ID_RS485_3, .pin_type = PIN_GPIO_485_EN, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_11, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},    // 485_EN3
    {.device_id = DEVICE_ID_RS485_3, .af_periph = GPIO_AF_USART3, .af_pinsource = GPIO_PinSource8, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_8, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // UART3_TX
    {.device_id = DEVICE_ID_RS485_3, .af_periph = GPIO_AF_USART3, .af_pinsource = GPIO_PinSource9, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_9, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // UART3_RX

    {.device_id = DEVICE_ID_CAN1, .af_periph = GPIO_AF_CAN1, .af_pinsource = GPIO_PinSource0, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_0, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // CAN1_RX
    {.device_id = DEVICE_ID_CAN1, .af_periph = GPIO_AF_CAN1, .af_pinsource = GPIO_PinSource1, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_1, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // CAN1_TX
        
    {.device_id = DEVICE_ID_CAN2, .af_periph = GPIO_AF_CAN2, .af_pinsource = GPIO_PinSource5, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_5, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // CAN2_RX
    {.device_id = DEVICE_ID_CAN2, .af_periph = GPIO_AF_CAN2, .af_pinsource = GPIO_PinSource6, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_6, .GPIO_Mode = GPIO_Mode_AF, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // CAN2_TX    
    
    {.device_id = DEVICE_ID_NET, .pin_type = PIN_GPIO_RESET, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_0, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},      // ETH_RESET_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource1, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_1, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_REF_CLK_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource2, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_2, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_MDIO_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource7, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_7, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_CRS_DV_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource11, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_11,.GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_TX_EN_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource12, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_12,.GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_TXD0_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource13, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_13,.GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_TXD1_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource1, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_1, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_MDC_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource4, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_4, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_RXD0_Pin
    {.device_id = DEVICE_ID_NET, .af_periph = GPIO_AF_ETH, .af_pinsource = GPIO_PinSource5, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_5, .GPIO_Mode = GPIO_Mode_AF,  .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_100MHz}, .sinit = IO_NULL},     // ETH_RMII_RXD1_Pin

    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_9,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH0(ADC3_IN7)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_7,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH1(ADC3_IN5)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_6,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH2(ADC3_IN4)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_5,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH3(ADC3_IN15)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_4,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH4(ADC3_IN14)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_3,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH5(ADC3_IN9)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_10, .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH6(ADC3_IN8)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_3,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_VBAT(未用，可以考虑让给ADC_CH8T)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_4,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_IBAT(未用，而且PD4不具有ADC的功能？)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOC, {.GPIO_Pin = GPIO_Pin_0,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH7_T(ADC3_IN10已用，用来连接IO-D的扩展板)
    {.device_id = DEVICE_ID_ADC, .Base = GPIOA, {.GPIO_Pin = GPIO_Pin_4,  .GPIO_Mode = GPIO_Mode_AN, .GPIO_PuPd = GPIO_PuPd_NOPULL, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       //  ADC_CH8_T(未用，因为PA4为ADC12_IN4,驱动不支持)

    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI1, .Base = GPIOD, {.GPIO_Pin = GPIO_Pin_7,  .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN1-主板数字输入
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI2, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_11, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN2
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI3, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_12, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN3
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI4, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_13, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN4
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI5, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_15, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN5
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI6, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_0,  .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN6
    
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI7, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_11, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN1_C-扩展板数字输入
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI8, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_13, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN2_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI9, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_15, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},        // DIG_IN3_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI10, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_1,  .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       // DIG_IN4_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI11, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_1,  .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       // DIG_IN5_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI12, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_12, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       // DIG_IN6_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI13, .Base = GPIOF, {.GPIO_Pin = GPIO_Pin_14, .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       // DIG_IN7_C
    {.device_id = DEVICE_ID_DI, .pin_type = PIN_GPIO_DI14, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_0,  .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},       // DIG_IN8_C
    
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO1, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_12, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // DIG_OUT1-预留第1路继电器输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO2, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_13, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // DIG_OUT2-预留第2路继电器输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO3, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_14, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // DIG_OUT3-预留第3路继电器输出	
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO4, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_1,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // DIG_OUT4
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO5, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_2,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // DIG_OUT5
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO6, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_7,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // FAN1_OUT-控制风扇继电器输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO7, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_8,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // FAN2_OUT-控制风扇继电器输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO8, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_9,  .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_LOW},     // FAN3_OUT-控制风扇继电器输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO9, .Base = GPIOG, {.GPIO_Pin = GPIO_Pin_14, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_HIGH},    // DO_P3	光耦输出
    {.device_id = DEVICE_ID_DO, .pin_type = PIN_GPIO_DO10, .Base = GPIOE, {.GPIO_Pin = GPIO_Pin_15, .GPIO_Mode = GPIO_Mode_OUT, .GPIO_OType = GPIO_OType_PP, .GPIO_PuPd = GPIO_PuPd_UP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_HIGH},   // RUN_LED  运行指示灯    
 
    {.device_id = DEVICE_ID_EXT_INTERRUPT, .Base = GPIOB, {.GPIO_Pin = GPIO_Pin_14 , .GPIO_Mode = GPIO_Mode_IN, .GPIO_PuPd = GPIO_PuPd_DOWN, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // 11 -IO-24V掉电检测，上升沿触发中断
    #if 0
    {.Base = GPIOE, {.GPIO_Pin = GPIO_Pin_10 , .GPIO_Mode = GPIO_Mode_Out_PP, .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = 1},     // 51-CLR_OUT-清除EPO（紧急停机按键）信号
    {.Base = GPIOE, {.GPIO_Pin = GPIO_Pin_11,  .GPIO_Mode = GPIO_Mode_IPU,    .GPIO_Speed = GPIO_Speed_50MHz}, .sinit = IO_NULL},     // 52-EPO_IN-EPO信号输入
    #endif
#endif
    
};


/**********************************************************************************
 * Function       : device_search_by_id
 * Author         : Linfei
 * Date           : 2013.01.23
 * Description    : 根据设备ID号找出设备在设备列表中的索引
 * Calls          : None
 * Input          : device_id：设备ID号
 * Output         : None
 * Return         : 返回设备在设备配置表中的索引
 *-------------------------------------------------------------------------------
 * 2013.01.23 : 1.0.0 : Linfei
 * Modification   : 初始代码编写。
 ***********************************************************************************/
uint8_t device_search_by_id(uint8_t device_id)
{
    uint8_t tmp = sizeof(device_cfg) / sizeof(Device_cfg);
    uint8_t idx;
    for(idx = 0; idx < tmp; idx++)
    {
        if(device_cfg[idx].device_id == device_id)
        {
            break;
        }
    }
    if(idx == tmp)
    {
        while(1)
        {
            ; // Device configure table Error !!!
        }
    }
    return idx;
}

/**********************************************************************************
 * Function       : device_rcc_config
 * Author         : Linfei
 * Date           : 2013.01.23
 * Description    : 根据ID号开启关闭时钟，并返回设备寄存器操作基地址
 * Calls          : None
 * Input          : device_id：设备ID号
 *                  is_init：使能时钟(TRUE)/(除能时钟)FALSE
 * Output         : None
 * Return         : 返回设备寄存器操作基地址
 *-------------------------------------------------------------------------------
 * 2013.01.23 : 1.0.0 : Linfei
 * Modification   : 初始代码编写。
 ***********************************************************************************/
void* device_rcc_config(uint8_t device_id, bool is_init)
{
    uint8_t idx = device_search_by_id(device_id);
    FunctionalState NewState;
    if(is_init == FALSE)
    {
        NewState = DISABLE;
    }
    else
    {
        NewState = ENABLE;
    }
    if(device_cfg[idx].periph_bus == PERIPH_APB1)
    {
        RCC_APB1PeriphClockCmd(device_cfg[idx].periph_rccid, NewState);
    }
    else if(device_cfg[idx].periph_bus == PERIPH_APB2)
    {
        RCC_APB2PeriphClockCmd(device_cfg[idx].periph_rccid, NewState);
    }
    else if(device_cfg[idx].periph_bus == PERIPH_AHB1)
    {
        RCC_AHB1PeriphClockCmd(device_cfg[idx].periph_rccid, NewState);
    }
    else if(device_cfg[idx].periph_bus == PERIPH_AHB2)
    {
        RCC_AHB2PeriphClockCmd(device_cfg[idx].periph_rccid, NewState);
    }
    return device_cfg[idx].periph_addr;
}

/**********************************************************************************
 * Function       : get_gpio_ctl_attr
 * Author         : Linfei
 * Date           : 2013.01.23
 * Description    : 根据ID号，获取对应引脚类型的引脚控制信息
 * Calls          : None
 * Input          : device_id：设备ID号
 *                  pin_type：管脚类型
 * Output         : None
 * Return         : 返回GPIO口操作结构体
 *-------------------------------------------------------------------------------
 * 2013.01.23 : 1.0.0 : Linfei
 * Modification   : 初始代码编写。
 ***********************************************************************************/
GPIO_CTL get_gpio_ctl_attr(uint8_t device_id, PIN_TYPE pin_type)
{
    GPIO_CTL gpioctl = { 0 , 0 };
    uint8_t len = sizeof(gGPIOAll)/sizeof(TGPIOA);
    for(uint16_t i = 0; i < len; i++)
    {
        if((device_id == gGPIOAll[i].device_id) && (pin_type == gGPIOAll[i].pin_type))
        {
            gpioctl.port = gGPIOAll[i].Base;
            gpioctl.pin = gGPIOAll[i].io.GPIO_Pin;
            break;
        }
    }
    return gpioctl;
}


/**********************************************************************************
 * Function       : device_gpio_config
 * Author         : Linfei
 * Date           : 2013.01.23
 * Description    : 根据设备ID号初始化该设备所使用的相关GPIO管脚 
 * Calls          : None
 * Input          : device_id：设备ID号
 *                  is_init：初始化(TRUE)/(复位)FALSE
 * Output         : None
 * Return         : None
 *-------------------------------------------------------------------------------
 * 2013.01.23 : 1.0.0 : Linfei
 * Modification   : 初始代码编写。
 ***********************************************************************************/
void device_gpio_config(uint8_t device_id, bool is_init)
{
    uint16_t i;
    uint8_t len = sizeof(gGPIOAll)/sizeof(TGPIOA);
    for(i = 0; i < len; i++)
    {
        if(device_id == gGPIOAll[i].device_id)
        {
            if(is_init == TRUE)
            {
                /* GPIO初始化 */
                GPIO_Init(gGPIOAll[i].Base,(GPIO_InitTypeDef*)&gGPIOAll[i].io);
                /* 复用口配置 */
                if(gGPIOAll[i].io.GPIO_Mode == GPIO_Mode_AF)
                {
                    GPIO_PinAFConfig(gGPIOAll[i].Base, gGPIOAll[i].af_pinsource, gGPIOAll[i].af_periph);
                }
                
                /* IO口电平初始化 */
                if(gGPIOAll[i].sinit == IO_LOW)
                {
                    GPIO_ResetBits(gGPIOAll[i].Base,gGPIOAll[i].io.GPIO_Pin);
                }
                else if(gGPIOAll[i].sinit == IO_HIGH)
                {
                    GPIO_SetBits(gGPIOAll[i].Base,gGPIOAll[i].io.GPIO_Pin);
                }
                else if(gGPIOAll[i].sinit == IO_NULL)
                {
                }
            }
            else
            {
                TGPIOA tmpgpio = gGPIOAll[i];
                tmpgpio.io.GPIO_Mode = GPIO_Mode_IN;
                tmpgpio.io.GPIO_PuPd = GPIO_PuPd_NOPULL;
                GPIO_Init(tmpgpio.Base, &tmpgpio.io);
            }
        }
    }
}



