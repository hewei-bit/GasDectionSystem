# GasDectionSystem
输气站场气体泄漏检测系统（基于STM32f103)

名    称:输气站场气体泄漏检测系统

作    者:何蔚
创建日期:2020/10/14 建立系统框架
当前日期:2020/11/16 完成V1.0
当前日期:2020/12/24 完成V2.0
当前日期:2021/01/08 完成V3.0
当前日期:2021/01/18 完成V4.0
当前日期:2021/02/16 未完成V4.0
任务：
	1.开始任务 创建信号量 消息队列 互斥锁 事件标志组 任务
	2.DHT11温湿度采集,单总线采集，  	互斥锁 oled显示		数据通过消息队列发送数据到线程存储、转发任务
	3.TDLAS气体浓度采集，串口接收浓度数据，互斥锁 oled显示	数据通过消息队列发送数据到线程存储、转发任务        
	4.MQ135 浓度采集
	5.MQ4 浓度采集
	6.看门狗
	7.本地存储任务 等待多个内核对象 消息队列接收数据 以txt格式存入SD卡
	8.LORA转发 等待多个内核对象 消息队列接收数据 usart3 发送至上位机
	9.RTC时间显示 互斥锁 oled显示	
	10.LED0 信号量接受数据 报警
	11.LED1 系统运行提示
	12.BEEP 信号量接受数据 报警
	13.KEY 
	14.任务状态
	
	
说  明:		
	当前代码尽可能实现了模块化编程，一个任务管理一个硬件。最简单的
	led、蜂鸣器都由单独任务管理。


//V1.0 完成任务和内核创建 传感器数据采集 json数据封装
//V2.0 完成OLED显示 串口响应  LORA任务中dht11和TDLAS的消息队列传输
//V3.0 完成上位机对下位机的数据问询
//V4.0 完成lora信息传输 





