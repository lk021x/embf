/********************************************************************************
 * Copyright (C), 1997-2011, SUNGROW POWER SUPPLY CO., LTD.
 * File name      :W25Qx.c
 * Author         :Xu Shun'an
 * Date           :2011.06.04
 * Description    :STM32F10x系列处理器的型号为W25Q型FLASH模块函数，采用SPI通讯，
 *                 STM32工作在主模式，FLASH作为从设备。定义了SPI读写函数以及
 *                 FLASH打开、关闭、读数据、写数据、设置全局地址指针、擦除数据的
 *                 功能函数
 * Others         :None
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式，有返回值的函数进行处理，主要是根据返回值判断下
 *                   一步进行何种操作
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写。
 ********************************************************************************/
#include "W25Qx.h"



/* 页大小 */
#define PAGE_SIZE    256U       /* 字节 */

/* W25QX状态寄存器各位掩码 */
#define STATUS_BUSY 0x01



/* 命令字 */
#define WRITE      0x02U  /* Write to Memory instruction (Page Program) */
#define WRITEQUAD  0x32U  /* Quad page program */
#define WRSR       0x01U  /* Write Status Register instruction */
#define WREN       0x06U  /* Write enable instruction */
#define WRDIS      0x04U  /* Write disable instruction */

#define ERSECTOR   0x20U  /* Sector erase(4KB),段擦除 */
#define ERBLOCK32  0x52U  /* Block Erase(32KB) ，块擦除 */
#define ERPAGE     0xDBU  /* Page Erase，页擦除 */
#define SE         0xD8U  /* Sector Erase instruction (64KB) */
#define BE         0xC7U  /* Bulk Erase instruction, chip Erase */
#define ERSUSPEND  0x75U  /* Erase suspend */
#define ERRESUME   0x7AU  /* Erase resume */
#define PWDOWN     0xB9U  /* Power down */
#define CREAD      0xFFU  /* continuous read mode reset */

#define READ       0x03U  /* Read from Memory instruction */
#define READFAST   0x0BU  /* Fast read */
#define RDSR       0x05U  /* Read Status Register instruction */
#define READSTAT2  0x35U  /* Read status register-2 */
#define RDFRDO     0x3BU  /* Fast read dual output */
#define RDFRDIO    0xBBU  /* Fast read dual I/O */
#define RDFRQO     0x6BU  /* Fast read quad output */
#define RDFRQIO    0xEBU  /* Fast read quad I/O */
#define RDWRQIO    0xE7U  /* Word read quad I/O */
#define RDOWRQIO   0xE3U  /* Qctal word read quad I/O */

#define RPDID      0xABU  /* Release power down/Device ID */
#define MDID       0x90U  /* Manufacturer device id */
#define MDIDDIO    0x92U  /* Manufacturer device id by dual I/O */
#define MDIDQIO    0x94U  /* Manufacturer device id by quad I/O */
#define JEDECID    0x9FU  /* JEDEC ID */
#define RUID       0x4BU  /* Read unique ID */

#define WIP_Flag   0x01U  /* Write In Progress (WIP) flag */
#define DUMMY_BYTE 0xA5U  /* 读取W25Q命令时候，一个01波形的测试命令，无物理意义，用于等待数据从W25Q读出来 */


GPIO_CTL  FLASH_CS;
SPI_TypeDef* W25QX_SPI;

/* I/O管脚操作，用于W25Q的片选 */
#define W25Q_CLR_CS     (GPIO_ResetBits(FLASH_CS.port, FLASH_CS.pin))
#define W25Q_SET_CS     (GPIO_SetBits(FLASH_CS.port, FLASH_CS.pin))


static int32_t w25qx_opened = false;   /* W25Q打开标识 */
static uint32_t lptr = 0U;          /* 全局文件地址指针 */


/* 驱动内部使用函数声明 */
void spi_init(void);
uint8_t spi_send_byte(uint8_t byte);
void spi_send_long(uint32_t quat);
bool w25qx_chk_bsy(void);
void w25qx_cmd(uint8_t cmd);


/*******************************************************************************
 * Function       : spi_init
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 初始化SPI端口和参数,堆栈使用了 32 bytes
 * Calls          : RCC_APB2PeriphClockCmd， SPI_Init，SPI_Cmd
 * Input          : None
 * Output         : None
 * Return         : None
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
void spi_init(void)
{
    SPI_InitTypeDef SPI_InitStructure;


    /*************************SPI初始化设置***************************/
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode      = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize  = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL      = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS       = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit  = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7U;
    SPI_Init(W25QX_SPI, &SPI_InitStructure);

    SPI_Cmd(W25QX_SPI, ENABLE);
}


/*******************************************************************************
 * Function       : spi_send_byte
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : SPI发送一个字节,并返回此间收到的字节,堆栈使用了 16 bytes
 * Calls          : SPI_I2S_GetFlagStatus， SPI_I2S_ReceiveData
 * Input          : byte，需要发送的字节数据
 * Output         : None
 * Return         : 返回SPI所读取的字节数据
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
uint8_t spi_send_byte(uint8_t byte)
{
    uint8_t ch;
    while (SPI_I2S_GetFlagStatus(W25QX_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        ;
    }

    SPI_I2S_SendData(W25QX_SPI, byte);

    while (SPI_I2S_GetFlagStatus(W25QX_SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        ;
    }

    ch = (uint8_t)SPI_I2S_ReceiveData(W25QX_SPI);
    return ch;
}


/*******************************************************************************
 * Function       : spi_send_long
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : SPI发送四个字节(MSB在前)，是由于W25Q读写数据时候发送了一个字
 *                  节的数据3个字节的地址，总共4个字节，且顺序是MSB在前, 堆栈使
 *                  用了 16 bytes
 * Calls          : spi_send_byte
 * Input          : quat，需要发送的4个字节长度的数据
 * Output         : None
 * Return         : None
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
void spi_send_long(uint32_t quat)
{
    uint8_t buf[4];
    if(buf != NULL)
    {
        memcpy(buf, &quat, 4U);

        for (int8_t i = 3; i >= 0; --i)
        {
            (void)spi_send_byte(buf[i]);
        }
    }
}


/*******************************************************************************
 * Function       : w25qx_open
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 打开w25qx_open设备。将设备标识置为“TRUE”，并进行SPI初始化,在
 *                  系统频率为70MHz,函数调用时间为 181us，堆栈使用了 8 bytes
 * Calls          : spi_init
 * Input          : 无具体意义，可都为0
 * Output         : None
 * Return         : DSUCCESS：打开成功
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
int32_t w25qx_open(int32_t flag, int32_t mode)
{
    int32_t ret = OPFAULT;
    if(w25qx_opened != true)
    {
        W25QX_SPI = device_rcc_config(DEVICE_ID_FLASH, TRUE);
        device_gpio_config(DEVICE_ID_FLASH, TRUE);
        FLASH_CS = get_gpio_ctl_attr(DEVICE_ID_FLASH, PIN_GPIO_CS);
        W25Q_SET_CS;
        
        spi_init();
        w25qx_opened = true;
        ret = DSUCCESS;
    }
    return ret;
}


/*******************************************************************************
 * Function       : w25qx_close
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 关闭w25qx_open设备，将设备标识置为“FALSE”，关闭SPI口，在系统
 *                  频率为70MHz,函数调用时间为 25us，堆栈使用了 8 bytes
 * Calls          : SPI_I2S_DeInit
 * Input          : 无具体意义，可都为0
 * Output         : None
 * Return         : DSUCCESS：关闭成功
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
int32_t w25qx_close(void)
{
    if (w25qx_opened == true)
    {
        SPI_I2S_DeInit(W25QX_SPI);
        device_rcc_config(DEVICE_ID_FLASH, FALSE);
        device_gpio_config(DEVICE_ID_FLASH, FALSE);
        w25qx_opened = false;
    }
    return DSUCCESS;
}


/*******************************************************************************
 * Function       : w25qx_chk_bsy
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 判断当前W25Q是否处于忙状态，堆栈使用了 8 bytes
 * Calls          : spi_send_byte
 * Input          : None
 * Output         : None
 * Return         : FALSE: 当前W25Q空闲；TRUE：当前W25Q忙
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
bool w25qx_chk_bsy(void)
{
    uint8_t  sr;
    bool ret = false;
    uint32_t count = 600000U;   /* 超时判断(经测试,这是等整片擦除完成所需的最小计数值600000) */

    W25Q_CLR_CS;
    sr = spi_send_byte(RDSR);

    do
    {
        sr = spi_send_byte(DUMMY_BYTE);
        count--;
    }
    while(((sr & WIP_Flag) != 0U) && (count != 0U));

    W25Q_SET_CS;

    if (count == 0U)
    {
        ret = true;
    }
    return ret;
}


/*******************************************************************************
 * Function       : w25qx_cmd
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 通过SPI口发送一个命令给W25Q，堆栈使用了 8 bytes
 * Calls          : spi_send_byte
 * Input          : cmd: 命令字
 * Output         : None
 * Return         : None
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
void w25qx_cmd(uint8_t cmd)
{
    W25Q_CLR_CS;

    (void)spi_send_byte(cmd);

    W25Q_SET_CS;
}


/*******************************************************************************
 * Function       : w25qx_read
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 读取FLASH内容.最多读到文件末尾结束，在系统频率为70MHz,函数调
 *                  用时间为 114us/字节，堆栈使用了 24 bytes
 * Calls          : w25qx_chk_bsy，spi_send_long，spi_send_byte
 * Input          : buf: 待读取数据存储地址指针；count：待读取数据字节长度
 * Output         : None
 * Return         : EFAULT：读取失败； count：实际所读取到的字节数
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
int32_t w25qx_read(uint8_t buf[], uint32_t count)
{
    int32_t ret = OPFAULT;
    if ((w25qx_opened == true) && (buf != NULL))
    {
        /* 先等待w25X芯片内部操作结束 */
        if (w25qx_chk_bsy() != true)
        {
            /* 判断要读取的内容是否超过了文件末尾 */
            if ((lptr + count) >= (uint32_t)W25Q_FLASH_SIZE)
            {
                count = (uint32_t)W25Q_FLASH_SIZE - 1U - lptr; /* 如果超过,则只读到文件末尾 */
            }

            if(count != 0U)
            {
                /* 1.选中w25X芯片 */
                W25Q_CLR_CS;

                /* 2.发送一字节读命令和三字节地址READ */
                spi_send_long((((uint32_t)READ) << 24) | (lptr & 0x00FFFFFFU));

                /* 3.接收内容 */
                for(uint32_t i = 0U; i < count; ++i)
                {
                    buf[i] = spi_send_byte(DUMMY_BYTE);
                    ++lptr;
                }

                /* 4.取消选中w25X芯片 */
                W25Q_SET_CS;
            }
            ret = (int32_t)count;
        }
    }
    return ret;
}


/*******************************************************************************
 * Function       : w25qx_write
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 向FLASH中写入数据.最多写到文件末尾结束，在系统频率为70MHz,函
 *                  数调用时间为 181us/字节，堆栈使用了 40 bytes
 * Calls          : w25qx_chk_bsy，spi_send_long，spi_send_byte
 * Input          : buf: 待写入数据存储地址指针；count：待写入数据字节长度
 * Output         : None
 * Return         : EFAULT：写入失败； count：实际所写入的字节数
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
int32_t w25qx_write(const uint8_t buf[], uint32_t count)
{
    int32_t ret = OPFAULT;
    uint32_t wrlen;
    uint32_t tl, i, index = 0U;

    if((w25qx_opened == true) && (buf != NULL))
    {
        if (w25qx_chk_bsy() != true)
        {
            if ((lptr + count) >= (uint32_t)W25Q_FLASH_SIZE)
            {
                count = (uint32_t)W25Q_FLASH_SIZE - 1U - lptr;
            }

            wrlen = count;
            /* 发送要写入的内容 */
            while(wrlen > 0U)
            {
                w25qx_cmd(WREN);

                tl = PAGE_SIZE - (lptr % PAGE_SIZE);

                /* 2.选中M25X芯片 */
                W25Q_CLR_CS;

                /* 3.发送一字节写命令和三字节地址 */
                spi_send_long((((uint32_t)WRITE) << 24) | lptr);

                if (tl > wrlen)
                {
                    for(i = 0U; i < wrlen; ++i)
                    {
                    	(void)spi_send_byte(buf[index]);
                        ++index;
                    }
                    lptr += wrlen;
                    wrlen -= wrlen;
                }
                else
                {
                    for(i = 0U; i < tl; ++i)
                    {
                    	(void)spi_send_byte(buf[index]);
                    	++index;
                    }

                    lptr += tl;
                    wrlen -= tl;
                }

                W25Q_SET_CS;

                if(w25qx_chk_bsy() == true)
                {
                    break;
                }
            }
            ret = (int32_t)count;
        }
    }
    return ret;
}


/*******************************************************************************
 * Function       : w25qx_lseek
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 设置全局文件指针，在系统频率为70MHz,函数调用时间为 41us，堆栈
 *                  使用了 8 bytes
 * Calls          : None
 * Input          : offset:指针偏移量，offset>0标识向前偏移，offset<0表示向后偏移
 *                  where: 当前指针指向文件的地方，共有三种取值方法：
 *                  SEEK_SET-文件开始处，SEEK_CUR-当前文件处，SEEK_END-文件结尾处
 * Output         : None
 * Return         : DSUCCESS: 设置成功；EFAULT：设置失败
 ********************************************************************************
 *-------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 ********************************************************************************/
int32_t w25qx_lseek(int64_t offset, int32_t where)
{
    int32_t ret = OPFAULT;
    if(w25qx_opened == true)
    {
        switch(where)
        {
        case SEEK_SET:
            if ((offset >= 0) && (offset < W25Q_FLASH_SIZE))
            {
                lptr = (uint32_t)offset;
                ret = DSUCCESS;
            }
            break;

        case SEEK_END:
            if((offset <= 0) && (offset > -W25Q_FLASH_SIZE))
            {
                lptr = ((uint32_t)W25Q_FLASH_SIZE - 1U) - (uint32_t)offset;
                ret = DSUCCESS;
            }
            break;

        case SEEK_CUR:
            if((((int64_t)lptr + offset) >= 0) && (((int64_t)lptr + offset) < W25Q_FLASH_SIZE))
            {
                if(offset >= 0)
                {
                    lptr += (uint32_t)offset;
                }
                else
                {
                    offset = -offset;
                    lptr -= (uint32_t)offset;
                }
                ret = DSUCCESS;
            }
            break;

        default:
            break;
        }
    }
    return ret;
}


/*************************************************************************************
 * Function       : w25qx_ioctl
 * Author         : Xu Shun'an
 * Date           : 2011.06.04
 * Description    : 擦除W25Q中的历史信息，便于写入新的数据，在系统频率为70MHz,函数
 *                  调用时间为擦除4KB空间 170ms、擦除整片 15.75s，堆栈 使用了16 bytes
 * Calls          : w25qx_chk_bsy，spi_send_long，w25qx_cmd
 * Input          : op：擦除命令，这里只定义了常用的两种:
 *                  W25QX_CMD_SE:一次擦除4KB，W25QX_CMD_BE，一次擦除整片数据
 *                  arg：擦除4KB时候需要设置的起始擦除地址
 * Output         : None
 * Return         : DSUCCESS: 擦除成功；EFAULT：擦除失败
 **************************************************************************************
 *-------------------------------------------------------------------------------------
 * 2011-06-08 : 1.0.1 : xusa
 * Modification   :  整理代码格式
 *-------------------------------------------------------------------------------------
 * 2010-08-03 : 1.0.0 : liulei
 * Modification   : 初始代码编写
 **************************************************************************************/
int32_t w25qx_ioctl(uint32_t op, void *arg)
{
    int32_t ret = OPFAULT;
    if (w25qx_opened == true)
    {
        switch(op)
        {
            /* 擦除 */
        case W25QX_CMD_PE:
        case W25QX_CMD_SSE:
        case W25QX_CMD_SE:
            if (arg != NULL)
            {
                W25QX_IOCTL_ARG *param = (W25QX_IOCTL_ARG *)arg;

                if (w25qx_chk_bsy() != true)
                {
                    w25qx_cmd(WREN);
                    W25Q_CLR_CS;
                    spi_send_long(((uint32_t)ERSECTOR << 24) | ((uint32_t)param->addr & 0x00ffffffU));
                    W25Q_SET_CS;
                    if(w25qx_chk_bsy() != true)
                    {
                        ret = DSUCCESS;
                    }
                }
            }
            break;

            /* 整片擦除 */
        case W25QX_CMD_BE:
            if(w25qx_chk_bsy() != true)
            {
                w25qx_cmd(WREN);
                w25qx_cmd(BE);
                if(w25qx_chk_bsy() != true)
                {
                    ret = DSUCCESS;
                }
            }
            break;
        
        case W25QX_CMD_BE32KB: /* 32KB块擦除 */
            if (arg != NULL)
            {
                W25QX_IOCTL_ARG *param = (W25QX_IOCTL_ARG *)arg;

                if (w25qx_chk_bsy() != true)
                {
                    w25qx_cmd(WREN);
                    W25Q_CLR_CS;
                    spi_send_long(((uint32_t)ERBLOCK32 << 24) | ((uint32_t)param->addr & 0x00ffffffU));
                    W25Q_SET_CS;
                    if(w25qx_chk_bsy() != true)
                    {
                        ret = DSUCCESS;
                    }
                }
            }
            break;

        case W25QX_CMD_BE64KB: /* 64KB块擦除 */
            if (arg != NULL)
            {
                W25QX_IOCTL_ARG *param = (W25QX_IOCTL_ARG *)arg;

                if (w25qx_chk_bsy() != true)
                {
                    w25qx_cmd(WREN);
                    W25Q_CLR_CS;
                    spi_send_long(((uint32_t)SE << 24) | ((uint32_t)param->addr & 0x00ffffffU));
                    W25Q_SET_CS;
                    if(w25qx_chk_bsy() != true)
                    {
                        ret = DSUCCESS;
                    }
                }
            }
            break;

        default:
            break;
        }
    }
    return ret;
}

