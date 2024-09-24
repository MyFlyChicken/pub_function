/**
 * @file poll_module.h
 * @author yuyf ()
 * @brief 模块需要搭配链接文件才可实现自动初始化，否则用used修饰的变量，依然会被优化掉
 * @version 0.1
 * @date 2024-06-11
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-06-11 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#ifndef __poll_module_H_ //shift+U转换为大写
#define __poll_module_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "poll_def.h"

/*模块初始化项*/
typedef struct {
    const char* name;   //模块名称
    void (*init)(void); //初始化接口
} poll_item_t;

/*任务处理项*/
typedef struct {
    const char* name;       //模块名称
    void (*handle)(void);   //初始化接口
    unsigned int  interval; //轮询间隔
    unsigned int* timer;    //指向定时器指针
} poll_task_t;

#define __poll_module_initialize(name, func, level)     \
    USED ANONY_TYPE(const poll_item_t, poll_tbl_##func) \
        SECTION("poll.item." level) = {name, func}

/*
 * @brief       任务注册
 * @param[in]   name    - 任务名称 
 * @param[in]   handle  - 初始化处理(void func(void){...})
  * @param[in]  interval- 轮询间隔(ms)
 */
#define poll_task_register(name, handle, interval)                        \
    static unsigned int __task_timer_##handle;                            \
    USED                ANONY_TYPE(const poll_task_t, poll_task_##handle) \
        SECTION("poll.task.1") =                                          \
            {name, handle, interval, &__task_timer_##handle}

/*
 * @brief       模块初始化注册
 * @param[in]   name    - 模块名称 
 * @param[in]   func    - 初始化入口函数(void func(void){...})
 */
#define system_init(name, func) __poll_module_initialize(name, func, "1")
#define driver_init(name, func) __poll_module_initialize(name, func, "2")
#define module_init(name, func) __poll_module_initialize(name, func, "3")

#ifdef __cplusplus
}
#endif

#endif
