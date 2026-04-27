/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           MASTER INCLUDES
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : includes.h
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

#ifndef  INCLUDES_MODULES_PRESENT
#define  INCLUDES_MODULES_PRESENT


/*
*********************************************************************************************************
*                                         STANDARD LIBRARIES
*********************************************************************************************************
*/


#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <stdarg.h>
#include  <math.h>


/*
*********************************************************************************************************
*                                                 OS
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                              LIBRARIES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <lib_str.h>

/*
*********************************************************************************************************
*                                              APP / BSP
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <bsp.h>
//#include  <bsp_int.h>

//事件标志组：硬件使用到的标志位
#define FLAG_GRP_RTC_WAKEUP					0x01
#define FLAG_GRP_RTC_ALARM_A				0x02
#define FLAG_GRP_RTC_ALARM_B				0x04

#define FLAG_GRP_KEY0_DOWN					0x10
#define FLAG_GRP_KEY1_DOWN					0x20
#define FLAG_GRP_KEY2_DOWN					0x40
#define FLAG_GRP_WK_UP_DOWN					0x80


#define MAX_CONVERTED_VALUE 	4095  	//最大转换值
#define VREF					3300	//最大电压值
#define QUEUE_NUM		10				//消息队列长度
#define CORE_OBJ_NUM	2				//内核对象个数，一共3个：2个信号量和一个消息队列						


//节点结构体
typedef struct 
{
	char device_id[4];
	char lora_address[4];
	char lora_channel[4];
	char temperature[8];
	char humidity[8];
	char CH4concentration[8];
	char Pitch[8];
	char Roll[8];
	char Yaw[8];
	char light[4];	
	char warning[4];
	char over[4];
} NODE;


extern OS_FLAG_GRP			g_flag_grp;		

extern OS_MUTEX				g_mutex_printf;		//互斥锁的对象


extern OS_Q	 				g_queue_usart1;				//消息队列的对象
extern OS_Q	 				g_queue_usart2;				//消息队列的对象
extern OS_Q	 				g_queue_usart3;				//消息队列的对象
extern OS_Q	 				g_queue_usart4;				//消息队列的对象
extern OS_Q	 				g_queue_usart5;				//消息队列的对象


extern void dgb_printf_safe(const char *format, ...);

//任务1 开始任务
extern OS_TCB StartTaskTCB;

//任务2 DHT11温湿度采集 
extern OS_TCB DHT11_Task_TCB;

//任务3 TDLAS 气体浓度采集	usart2
extern OS_TCB TDLAS_Task_TCB;

//任务4  mq135 气体浓度采集	
extern OS_TCB MQ135_Task_TCB;

//任务5 mq4 气体浓度采集	
extern OS_TCB MQ4_Task_TCB;

//任务6 看门狗 
extern OS_TCB IWG_Task_TCB;

//任务7 等待多个内核对象 消息队列接收数据 以txt格式存入SD卡
extern OS_TCB SAVE_Task_TCB;

//任务8 LORA转发 等待多个内核对象 消息队列接收数据 usart3 发送至上位机
extern OS_TCB LORA_Task_TCB;

//任务9 rtc时间显示	互斥锁 oled显示	
extern OS_TCB RTC_Task_TCB;

//任务10 LED0任务 
extern OS_TCB Led0TaskTCB;

//任务11 LED1任务
extern OS_TCB Led1TaskTCB;

//任务12.BEEP
extern OS_TCB BEEP_Task_TCB;

//任务13.key
extern OS_TCB KEY_Task_TCB;

//任务14 MPU6050
extern OS_TCB	Mpu6050TaskTCB;

//任务15.任务状态
extern OS_TCB	TASK_STA_Task_TCB;

//任务16 任务运行提示
extern OS_TCB	FloatTaskTCB;




#endif



