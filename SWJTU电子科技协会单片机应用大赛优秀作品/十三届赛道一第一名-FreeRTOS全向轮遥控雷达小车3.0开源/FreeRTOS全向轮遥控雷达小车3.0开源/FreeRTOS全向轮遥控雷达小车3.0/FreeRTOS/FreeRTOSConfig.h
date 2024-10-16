/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"                  // Device header


//* 1：RTOS使用抢占式调度器；0：RTOS使用协作式调度器（时间片）	
#define configUSE_PREEMPTION                    1	

/*一些 FreeRTOS 移植有两种选择下一个执行任务的方法，
 *0:一种是通用方法， 
 *1:另一种是移植特定的方法(依赖于一种或多种架构特定的汇编指令 （通常是前导零计数 [CLZ] 指令或同等指令） ，
 *因此 仅适用于专为其编写该指令的架构。)。
 */
//#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0

//设置为 1，使用 低功耗无 滴答 模式， 或设置为 0，保持 滴答 中断始终运行。
#define configUSE_TICKLESS_IDLE                 1	

//内核时钟
#define configCPU_CLOCK_HZ                      (SystemCoreClock)

//SysTick 时钟频率  系统已定义，此处不用定义
//#define configSYSTICK_CLOCK_HZ                  configCPU_CLOCK_HZ

//RTOS系统节拍中断的频率 1ms
#define configTICK_RATE_HZ                      ( ( TickType_t ) 20000 )		

//可使用的最大优先级
#define configMAX_PRIORITIES                    (32)	

//空闲任务使用的堆栈大小
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 128 )

//任务名字字符串长度
#define configMAX_TASK_NAME_LEN                 16	

//系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形
#define configUSE_16_BIT_TICKS                  0

// RTOS 内核启动后，滴答中断已经执行的次数。滴答数存放在 滴答Type_t 类型的变量中
//#define configTICK_TYPE_WIDTH_IN_BITS           TICK_TYPE_WIDTH_32_BITS

//空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD                 1

//开启任务通知功能，1开启
//#define configUSE_TASK_NOTIFICATIONS            1	

//每个 RTOS 任务都有一个任务通知数组。 configTASK_NOTIFICATION_ARRAY_ENTRIES 设置数组的索引数。
//#define configTASK_NOTIFICATION_ARRAY_ENTRIES   3	

//1 使用互斥信号量
//#define configUSE_MUTEXES                       0	

//1 使用递归互斥信号量 
//#define configUSE_RECURSIVE_MUTEXES             0

//1 使用计数信号量
//#define configUSE_COUNTING_SEMAPHORES           0	

//#define configUSE_ALTERNATIVE_API               0 /* Deprecated! */

//设置可以注册的信号量和消息队列个数
//#define configQUEUE_REGISTRY_SIZE               10	

//启用队列
//#define configUSE_QUEUE_SETS                    0

//1使能时间片调度
//#define configUSE_TIME_SLICING                  1

//如果 configUSE_NEWLIB_REENTRANT 设置为 1，那么将为每个创建的任务分配一个newlib 重入结构体 。
//#define configUSE_NEWLIB_REENTRANT              0

//1 兼容8.0以前的版本
//#define configENABLE_BACKWARD_COMPATIBILITY     0

//设置每个任务的 线程本地存储数组
//#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 5

//设置为 0 时， MiniListItem_t 和 ListItem_t 保持一致。设置为 1 时，MiniListItem_t 包含的字段比 ListItem_t 少 3 个
//#define configUSE_MINI_LIST_ITEM                1

//设置调用 xTaskCreate() 时用于指定堆栈深度的类型
//#define configSTACK_DEPTH_TYPE                  uint16_t

//消息缓冲区使用 configMESSAGE_BUFFER_LENGTH_TYPE 类型的变量存储 每个消息的长度
//#define configMESSAGE_BUFFER_LENGTH_TYPE        size_t

//
//#define configHEAP_CLEAR_MEMORY_ON_FREE         1



/*****************************************************************
              FreeRTOS与内存申请有关配置选项                                               
*****************************************************************/

//支持静态内存
#define configSUPPORT_STATIC_ALLOCATION             1

//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION            1

//系统所有总的堆大小
#define configTOTAL_HEAP_SIZE                       ( ( size_t ) ( 10 * 1024 ) )

/*默认情况下，FreeRTOS 堆由 FreeRTOS 声明 并由链接器放置在存储器中。 
 *将 configAPPLICATION_ALLOCATED_HEAP 设置为 1，
 *允许应用程序编写者声明堆，这使得 应用程序编写者可以将堆放置在内存的任意位置。
 *如果使用 heap_1.c、heap_2.c 或 heap_4.c，并且将 configAPPLICATION_ALLOCATED_HEAP 设置为 1，
 *那么应用程序编写者必须提供一个 uint8_t 数组， 其确切的名称和大小如下所示。
 *该数组将用作 FreeRTOS 堆。 数组如何放置在内存的特定位置取决于使用的编译器， 请参阅编译器的文档。
 */
//#define configAPPLICATION_ALLOCATED_HEAP            1

/*设置为 1，那么对于任何 使用 xTaskCreate 或 xTaskCreateRestricted API 创建的任务，
 *其堆栈 使用 pvPortMallocStack 函数分配并使用 vPortFreeStack 函数释放。
 *用户需要 实现线程安全的 pvPortMallocStack 和 vPortFreeStack 函数。
 */
//#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP   1

/***************************************************************
             FreeRTOS与钩子函数有关的配置选项                                            
**************************************************************/

/* 置1：使用空闲钩子（Idle Hook类似于回调函数）；置0：忽略空闲钩子
 * 
 * 空闲任务钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationIdleHook(void )，
 * 这个函数在每个空闲任务周期都会被调用
 * 对于已经删除的RTOS任务，空闲任务可以释放分配给它们的堆栈内存。
 * 因此必须保证空闲任务可以被CPU执行
 * 使用空闲钩子函数设置CPU进入省电模式是很常见的
 * 不可以调用会引起空闲任务阻塞的API函数
 */
#define configUSE_IDLE_HOOK                     0

/* 置1：使用时间片钩子（Tick Hook）；置0：忽略时间片钩子
 * 
 * 
 * 时间片钩子是一个函数，这个函数由用户来实现，
 * FreeRTOS规定了函数的名字和参数：void vApplicationTickHook(void )
 * 时间片中断可以周期性的调用
 * 函数必须非常短小，不能大量使用堆栈，
 * 不能调用以”FromISR" 或 "FROM_ISR”结尾的API函数
 */
 /*xTaskIncrementTick函数是在xPortSysTickHandler中断函数中被调用的。因此，vApplicationTickHook()函数执行的时间必须很短才行*/
#define configUSE_TICK_HOOK                     0

/*
 * 大于0时启用堆栈溢出检测功能，如果使用此功能 
 * 用户必须提供一个栈溢出钩子函数，如果使用的话
 * 此值可以为1或者2，因为有两种栈溢出检测方法
 */
//#define configCHECK_FOR_STACK_OVERFLOW          0

//使用内存申请失败钩子函数
#define configUSE_MALLOC_FAILED_HOOK            0


//#define configUSE_DAEMON_TASK_STARTUP_HOOK      0
//#define configUSE_SB_COMPLETED_CALLBACK         0

/********************************************************************
          FreeRTOS与运行时间和任务状态收集有关的配置选项   
**********************************************************************/

//启用运行时间统计功能
//#define configGENERATE_RUN_TIME_STATS           0

//启用可视化跟踪调试
//#define configUSE_TRACE_FACILITY                0

/* 与宏configUSE_TRACE_FACILITY同时为1时会编译下面3个函数
 * prvWriteNameToBuffer()
 * vTaskList(),
 * vTaskGetRunTimeStats()
*/
//#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/********************************************************************
                FreeRTOS与协程有关的配置选项                                                
*********************************************************************/
//启用协程，启用协程以后必须添加文件croutine.c
#define configUSE_CO_ROUTINES                   0

////协程的有效优先级数目
#define configMAX_CO_ROUTINE_PRIORITIES         2

/***********************************************************************
                FreeRTOS与软件定时器有关的配置选项      
**********************************************************************/

//启用软件定时器
#define configUSE_TIMERS                        1

//软件定时器优先级
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES-1)

//软件定时器队列长度
#define configTIMER_QUEUE_LENGTH                10

//软件定时器任务堆栈大小
#define configTIMER_TASK_STACK_DEPTH            (configMINIMAL_STACK_SIZE*2)

/************************************************************
           中断嵌套行为配置                                                    
************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//中断最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//系统可管理的最高中断优先级
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

//#define configMAX_API_CALL_INTERRUPT_PRIORITY   [dependent on processor and application]

/************************************************************
           断言配置                                                    
************************************************************/
//#define configASSERT( ( x ) )  if( ( x ) == 0 ) vAssertCalled( __FILE__, __LINE__ )

/************************************************************
           FreeRTOS MPU的具体定义                                                   
************************************************************/
//#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0
//#define configTOTAL_MPU_REGIONS                                8 /* Default value. */
//#define configTEX_S_C_B_FLASH                                  0x07UL /* Default value. */
//#define configTEX_S_C_B_SRAM                                   0x07UL /* Default value. */
//#define configENFORCE_SYSTEM_CALLS_FROM_KERNEL_ONLY            1
//#define configALLOW_UNPRIVILEGED_CRITICAL_SECTIONS             1
//#define configENABLE_ERRATA_837070_WORKAROUND                  1
//#define configUSE_MPU_WRAPPERS_V1                              0
//#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE                10
//#define configSYSTEM_CALL_STACK_SIZE                           128

///* ARMv8-M secure side port related definitions. */
//#define secureconfigMAX_SECURE_CONTEXTS         5

/************************************************************
            FreeRTOS可选函数配置选项                                                     
************************************************************/
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_uxTaskGetStackHighWaterMark2    0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   0
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          0
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1
#define INCLUDE_vTaskCleanUpResources			0
/* A header file that defines trace macro can be included here. */

#define xPortPendSVHandler 	PendSV_Handler
#define vPortSVCHandler 	SVC_Handler
//#define xPortSysTickHandler SysTick_Handler


#endif /* FREERTOS_CONFIG_H */

