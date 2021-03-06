/****************************************************************************** 
* Copyright (C), 1997-2010, SUNGROW POWER SUPPLY CO., LTD. 
* File name      :main.c
* Author         :llemmx
* Date           :2012-09-29
* Description    :UI进程
* Interface      :无
* Others         :无
*******************************************************************************
* History:        初稿
*------------------------------------------------------------------------------
* 2011-05-03 : 1.0.0 : llemmx
* Modification   :
*------------------------------------------------------------------------------
******************************************************************************/
#include "global.h"
#include "config.h"
#include "assage.h"
#include "fcntl.h"
#include "utils.h"
#include "drivers.h"
//extern void enter_low_power(void);



uint32_t os_tick_test = 0;
uint8_t ads1247_buf[10]= {0};
uint8_t flash_buf[10]={0};
uint8_t tmp_buf[10]={0};
int32_t ret12;
uint8_t dido_buf[32]={0};
uint8_t dsp_wake = 0;

t_assage_msg m_main_msg[3];
assage_handle g_main_mh=0;
//extern assage_handle g_main_mh;//assage机制测试
//extern uint32_t open_assage_flag;
t_gparam setTime;

extern void get_sys_time(void);
uint32_t baud_tmp = 0;
void task_main(void *p_arg)
{

    char buf[64] = {1,2,3};

// 	//波特率更改实验
//     uint32_t baud1 = 57600; 
//     drv_ioctl(gt_glp.fserial1,USART_SET_BAUD,&baud1);

// 	drv_ioctl(gt_glp.fserial1,USART_GET_BAUD,&baud_tmp);



	//波特率更改实验

    int32_t ret_test = 0;
    t_embf_canmsgrx rxmsg;
    uint8_t ret=0;
    uint32_t l_ret_serial1,l_ret_can1,l_wifi_serial2,l_ret_serial3;
    t_assage_msg l_msg;
    
    g_main_mh=assage_create_msg(m_main_msg,3,1);


    flash_buf[0]= 0x05;
    flash_buf[1]= 0x0A;
    flash_buf[2]= 0x05;
    flash_buf[3]= 0x0A;

	//enter_low_power();	   //进入低功耗
//wifi测试
//	io_ctrl(UTILS_IO_WIFI_RESET,UTILS_IO_LOW);
//	io_ctrl(UTILS_IO_WIFI_RESET,UTILS_IO_HIGH);

while(1)
{  
//         OSSchedLock();
//         io_ctrl(UTILS_IO_WIFI_RESET,UTILS_IO_LOW);
// //         os_tick_test = OSTimeGet();
// //         while((OSTimeGet()-os_tick_test)<500);
//         OSTimeSet(0);
//         while(OSTimeGet()<500);
//         io_ctrl(UTILS_IO_WIFI_RESET,UTILS_IO_HIGH);
//         OSSchedUnlock();
    //ads1247_read_period_proc();
  //  dido_demo_test();
    /* demo程序之DIDO  io_ctl  io_get */
    //单次执行,以用户层的形式来调用 ,以打开、操作、关闭的形式来操作
   // dido_demo_test();
   // ret_test = eeprom_demo_test();
  // ret12 = flash_demo_test();
  // r8025_demo_test();
   // adc_demo_test();
  //  ads1247_demo_test();
    usart_can_demo_test();
    //看门狗实验
    task_m.time_cnt = 30;//设置为30*300ms还未喂狗的话视为进程挂掉。
    
    embf_delay(2);
            //ads1247 试验  2012.12.18 
        // ads1247_read_period_proc();
            //ret12 = ads1247_read(ads1247_buf, 8);
           //  drv_read(gt_glp.fads1247,ads1247_buf, 8);


        //    ret12 = write_eeprom(0, flash_buf, 10);  
        //    ret12 = read_eeprom(0,tmp_buf,10);


//            io_ctrl(UTILS_IO_LED_RUN,UTILS_IO_HIGH);
//			embf_delay(250);
//			io_ctrl(UTILS_IO_LED_RUN,UTILS_IO_LOW);
//			embf_delay(250);
//			ret = 0;
//			uint8_t buf[64]={0};
//	drv_lseek(gt_glp.feeprom1,0,0);
//    drv_read(gt_glp.feeprom1,buf,1);

//IO-D板 dido测试 1.读出结果是否正确，是否有延时实现
        //    drv_read(gt_glp.fdido,dido_buf,5);

//		drv_ioctl(gt_glp.fdido,DO_P3|DO_STATE_LOW,0);
//		drv_ioctl(gt_glp.fdido,DIG_OUT4|DO_STATE_HIGH,0);
//		drv_ioctl(gt_glp.fdido,DIG_OUT5|DO_STATE_HIGH,0);
//
//		drv_ioctl(gt_glp.fdido,DIG_OUT4|DO_STATE_LOW,0);
//		drv_ioctl(gt_glp.fdido,DIG_OUT5|DO_STATE_LOW,0);

//IO-D板 dido测试 1.读出结果是否正确，是否有延时实现

//FLASH大容量读写
// for(uint8_t i=0;i<50;i++)
// {
// 	drv_write(gt_glp.fflash1,flash_buf,256);
// }




//FLASH大容量读写


//wifi测试
//		 drv_write(gt_glp.fserial2,"ATH\r\n",5);
//		l_wifi_serial2 = assage_accept(gt_glp.fserial2);
//		if(l_wifi_serial2 == 1 )
//		{
//			drv_read(gt_glp.fserial2,buf,64);	
//		}
//wifi测试
	 //   l_ret_serial3=assage_accept(gt_glp.fserial3);
     //   l_ret_can1   =assage_accept(gt_glp.fdcan1);
     //   assage_get_msg(g_main_mh,ASSAGE_GET_NODELAY,&l_msg);


// 		uint8_t l_buf[7];
// 		l_buf[0]=0x12;
// 		l_buf[1]=0x11;
// 		l_buf[2]=0x30;
// 		l_buf[3]=0x13;
// 		l_buf[4]=0x27;
// 		l_buf[5]=0x05;
// 		drv_write(gt_glp.frtc,l_buf,6);//设置时间函数
 
//     while(true){



//   //  get_sys_time();	//一直执行，对系统压力大





// 		ret = 0; //防止task_main不让出CPU，这样task_net不会进入执行
//       //  l_ret_serial1=assage_accept(gt_glp.fserial1);
//      //   l_ret_can1   =assage_accept(gt_glp.fdcan1);
//     //   assage_get_msg(g_main_mh,0xFF,&l_msg); //阻塞接收，看是否会成功
// //	if(open_assage_flag == 1)
// //	{
// //		    l_msg.msg=3;
// //            assage_post_msg(g_main_mh,&l_msg);
// //	}


//       //  if (l_ret_serial3==1){
// 		//低功耗实验
// 		  //  enter_low_power();	   //进入低功耗
//            // drv_read(gt_glp.fserial3,buf,64);
//            // drv_write(gt_glp.fserial3,buf,64);
//           //  while(1);//看门狗调试

//         if (l_ret_serial1==1){


//             drv_read(gt_glp.fserial1,buf,64);
//             drv_write(gt_glp.fserial1,buf,64);

// //			buf[0] = 0x12;					//时间设置实验，成功
// //			buf[1] =  0x11;
// //			buf[2] =  0x24;
// //			buf[3] = 0x10;
// //			buf[4] =  0x05;
// //			buf[5] = 0x07;
// //		    drv_write(gt_glp.frtc,buf,6);
// //			setTime.sys_time.tm_year = 0x13; //设为13年	   时间设置成功


//             ret=1;

// 		  // uint8_t buf[]={0x12,0x06,0x15,0x19,0x57,0x30}; //rtc write函数增加合法性检查
//           // drv_write(gt_glp.frtc,buf,6);
//       //  }
// //         else if (l_ret_can1==1){
// //             drv_read(gt_glp.fdcan1,&rxmsg,sizeof(rxmsg));
// //         }
// //         else{
// //             ret=0;
// //         }
// //         
// //        if (io_get(UTILS_IO_SLEEPIN)==0 && gt_glp.pwr_mode){
// //            init_run();
// //        }

// 		  // uint8_t buf[]={0x12,0x06,0x15,0x19,0x57,0x30}; //rtc write函数增加合法性检查
//           // drv_write(gt_glp.frtc,buf,6);
//         }
//         else if (l_ret_can1==1){
//             drv_read(gt_glp.fdcan1,&rxmsg,sizeof(rxmsg));
//         }
//         else{
//             ret=0;
//         }

//         

// //         if (l_msg.msg==1){
// //             drv_write(gt_glp.fserial1,"send second message ",20);
// //         }
// //         
// //         if (0==ret){
// //           dsp_wake =  io_get(UTILS_IO_WAKE_HMI_IN);
//              embf_delay(2);
// //         }

// //         if (io_get(UTILS_IO_SLEEPIN)==0 && gt_glp.pwr_mode){
// //             init_run();
// //         }
//         
//         if (l_msg.msg==1){

//             drv_write(gt_glp.fserial1,"send second message ",20);
//         }
//         
//         if (0==ret){


//             embf_delay(2);


//         }

//         }
	}
}


