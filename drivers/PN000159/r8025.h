/********************************************************************************
 * Copyright (C), 1997-2011, SUNGROW POWER SUPPLY CO., LTD. 
 * File name      :rtc.h
 * Author         :Linfei
 * Date           :2012.07.13
 * Description    :DS1340C驱动头文件，模拟I2C方式，移植时需修改该文件中的引脚定义
 *                 部分以及置高置低宏(STM32F10xx)，目前该驱动头文件适用STM32F4xx
 * Others         :None
 *-------------------------------------------------------------------------------
 * 2012.07.11 : 1.0.0 : Linfei
 * Modification   : 初始代码编写。
 ********************************************************************************/
#ifndef R8025_H_
#define R8025_H_

#include "stm32f4xx.h"
#include "stdint.h"
#include "global.h"
#include "io_interface.h"



/* 总线置高置低宏 */
#ifdef STM32F4XX
#define R8025_SCL_H        (R8025_SCL_GROUP->BSRRL = R8025_SCL_PIN)
#define R8025_SCL_L        (R8025_SCL_GROUP->BSRRH = R8025_SCL_PIN)  
#define R8025_SDA_H        (R8025_SDA_GROUP->BSRRL = R8025_SDA_PIN)
#define R8025_SDA_L        (R8025_SDA_GROUP->BSRRH = R8025_SDA_PIN)
#else
#define R8025_SCL_H        (R8025_SCL_GROUP->BSRR = R8025_SCL_PIN)
#define R8025_SCL_L        (R8025_SCL_GROUP->BRR = R8025_SCL_PIN)  
#define R8025_SDA_H        (R8025_SDA_GROUP->BSRR = R8025_SDA_PIN)
#define R8025_SDA_L        (R8025_SDA_GROUP->BRR = R8025_SDA_PIN)
#endif

/* 总线值读取宏 */
#define R8025_SCL_read     (GPIO_ReadInputDataBit(R8025_SCL_GROUP, R8025_SCL_PIN) == Bit_SET)
#define R8025_SDA_read     (GPIO_ReadInputDataBit(R8025_SDA_GROUP, R8025_SDA_PIN) == Bit_SET)

/* 参考年月日，如果读出的RTC时间小于该日期则报错 */
#define R8025_REF_YEAR    0x12U
#define R8025_REF_MONTH   0x07U
#define R8025_REF_DATE    0x13U


#ifndef  DSUCCESS
 #define  DSUCCESS                      0           /* 操作成功 */
#endif

#ifndef  OPFAULT
 #define  OPFAULT                       (-1)        /* 操作出错 */
#endif

#ifndef DEVBUSY
 #define DEVEBUSY                       (-2)        /* 忙 */
#endif



/*函数声明 */
int32_t r8025_open(int32_t flag, int32_t mode);
int32_t r8025_read(uint8_t buf[], uint32_t count);
int32_t r8025_write(const uint8_t buf[], uint32_t count);
int32_t r8025_release(void);
#endif
