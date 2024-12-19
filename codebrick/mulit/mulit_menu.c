#include "mulitmenu.h"

/**
 * @brief 多级任务初始化
 * @param p_mulit_menu 多级指针
 * @param init_depth 初始化深度
 */
void mulit_menu_init(mulit_menu_t *p_mulit_menu, unsigned char init_depth)
{
    p_mulit_menu->change_flag = 0;
    
    p_mulit_menu->curr_depth = init_depth;
    p_mulit_menu->last_depth = init_depth;
}

/**
 * @brief 周期执行任务
 * @param p_mulit_menu 多级指针
 */
void mulit_menu_main(mulit_menu_t *p_mulit_menu)
{
    if (p_mulit_menu->change_flag)
    {
        if (p_mulit_menu->curr_depth != p_mulit_menu->last_depth)
        {
            p_mulit_menu->last_depth = p_mulit_menu->curr_depth;
        }        
    }
    p_mulit_menu->menu[p_mulit_menu->last_depth](p_mulit_menu, p_mulit_menu->args[p_mulit_menu->last_depth][0], 
                                                               p_mulit_menu->args[p_mulit_menu->last_depth][1], 
                                                               p_mulit_menu->args[p_mulit_menu->last_depth][2]);
}
