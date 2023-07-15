#ifndef __MULIT_MENU_H
#define __MULIT_MENU_H


#include <stdint.h>


typedef int (* menu_func)(void *argv1, void *argv2, void *argv3, void *argv4);

#define MENU_DEPTH 5
typedef struct 
{
    unsigned char curr_depth;/* 当前深度 */
    unsigned char last_depth;/* 上一次深度 */
    unsigned char change_flag;/* 切换标志, 1可以切换 0不可以切换 */
    //void * lock;             /* 用户执行菜单根据lock判断是否可以执行当前深度任务 */
    //void * psd;              /* 用户密码 */
    void * args[MENU_DEPTH][4];/* 参数 */
    menu_func menu[MENU_DEPTH];
}mulit_menu_t;














#endif

