/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */

#define GPIO_LED_G    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)

// 静态线程1：红色LED闪烁
/*-----------------------------------Start---------------------------------------*/
rt_align(RT_ALIGN_SIZE)     // 线程栈空间对齐
static char led_red_stack[512];     // 线程栈空间为512字节
static struct rt_thread led_red_thread;    // 线程控制块

// 线程1入口
static void led_red_entry(void* parameter)
{
    while(1)
    {
        rt_pin_write(GPIO_LED_R, PIN_LOW);
        rt_thread_mdelay(500);
        rt_pin_write(GPIO_LED_R, PIN_HIGH);
        rt_thread_mdelay(500);
    }
}

// 创建线程与MSH命令
int led_red(void)
{
    rt_err_t result = RT_EOK;

    rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);

    result = rt_thread_init(&led_red_thread,                 // 线程句柄，由用户提供，并指向对应的线程控制块内存地址
                            "led_red",                // 线程名称
                            led_red_entry,            // 线程入口函数
                            RT_NULL,                  // 入口函数的传入参数
                            led_red_stack,            // 线程栈空间，由用户提供，并指向对应的线程栈内存地址
                            sizeof(led_red_stack),    // 线程栈空间大小，单位为字节
                             20,                      // 线程优先级，数值越小优先级越高
                             20);                     // 线程时间片，单位为系统节拍数，例如20表示20个系统节拍后进行一次时间片轮转

    if(result == RT_EOK)
    {
        rt_thread_startup(&led_red_thread);
        rt_kprintf("LED red thread started successfully!\n");
    }
    return result;
}

MSH_CMD_EXPORT(led_red, led red thread sample);
/*------------------------------------End--------------------------------------*/

// 静态线程2：绿色LED闪烁
/*-----------------------------------Start---------------------------------------*/
rt_align(RT_ALIGN_SIZE)     // 线程栈空间对齐
static char led_green_stack[512];   // 线程栈空间为512字节   
static struct rt_thread led_green_thread;  // 线程控制块

// 线程2入口
static void led_green_entry(void* parameter)
{
    int period = 20;   
    int dir = 1;  
    int duty = 0; 

    while(1)
    {
        for (int i = 0; i < 4; i++)
        { 
            // on_time = duty;
            // off_time = period - duty;
              
            rt_pin_write(GPIO_LED_G, PIN_HIGH); 
            if (duty > 0) rt_thread_mdelay(duty);
            
            rt_pin_write(GPIO_LED_G, PIN_LOW);  
            if (period - duty > 0) rt_thread_mdelay(period - duty);
        }

        duty += dir;
        if (duty >= period)
        {
            duty = period;
            dir = -1;
        }
        else if (duty <= 0)
        {
            duty = 0;
            dir = 1;
        }
    }
}

// 创建线程与MSH命令
int led_green(void)
{
    rt_err_t result = RT_EOK;

    rt_pin_mode(GPIO_LED_G, PIN_MODE_OUTPUT);

    result = rt_thread_init(&led_green_thread,
                            "led_green",
                            led_green_entry,
                            RT_NULL,
                            led_green_stack,
                            sizeof(led_green_stack),
                             20,
                             20);

    if(result == RT_EOK)
    {
        rt_thread_startup(&led_green_thread);
        rt_kprintf("LED green thread started successfully!\n");
    }    
    return result;
}
MSH_CMD_EXPORT(led_green, led green thread sample);
/*------------------------------------End--------------------------------------*/

// 杀死指定线程
/*-----------------------------------Start---------------------------------------*/
static void killerqueen(int argc, char** argv)
{
    if (argc < 2)
    {
        rt_kprintf("Usage: kill_thread <thread_name>\n");
        return;
    }

    const char* name = argv[1];
    rt_thread_t thread = rt_thread_find((char*)name);

    if (thread == RT_NULL)
    {
        rt_kprintf("Thread %s not found.\n", name);
        return;
    }

    /* 在杀死线程前，强制关闭对应的LED设备(灭灯操作) */
    if (rt_strcmp(name, "led_red") == 0)
    {
        rt_pin_write(GPIO_LED_R, PIN_HIGH);
    }
    else if (rt_strcmp(name, "led_green") == 0)
    {
        rt_pin_write(GPIO_LED_G, PIN_HIGH);
    }

    rt_err_t err;
    /* 判断是静态线程还是动态线程，分别调用不同的删除函数 */
    if (rt_object_is_systemobject((rt_object_t)thread))
    {
        err = rt_thread_detach(thread);
    }
    else
    {
        err = rt_thread_delete(thread);
    }

    if (err == RT_EOK)
    {
        rt_kprintf("Thread %s killed successfully.\n", name);
    }
    else
    {
        rt_kprintf("Failed to kill thread %s, error: %d\n", name, err);
    }
}

MSH_CMD_EXPORT(killerqueen, kill thread by name);
/*------------------------------------End--------------------------------------*/

int main()
{
    rt_kprintf("System started successfully!\n");
    rt_kprintf("Welcome aboard, dgzn123\n");

    return RT_EOK;
}
