/**
 * @file ee_device.c
 * @author yuyf ()
 * @brief 
 * @version 0.1
 * @date 2024-05-29
 * 
 * @copyright Copyright (c) 2024 常山赵子龙
 * 
 * @par 修改日志:
 * <table>
 * <caption id="multi_row">$</caption>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2024-05-29 <td>v1.0     <td>chen     <td>内容
 * </table>
 * @section 
 * @code 
 * @endcode
 */
#include "ee_device.h"
//ee_device_creat()

void ee_device_register(const ee_device_t dev, const ee_obj_t parent, const ee_ops_t ops, const void* user_data)
{
    dev->parent    = *parent;
    dev->ops       = (ee_ops_t)ops;
    dev->user_data = (void*)user_data;
}

void ee_device_set_indicate(const ee_device_t dev, int (*rx_indicate)(ee_device_t dev, uint32_t size))
{
    dev->rx_indicate = rx_indicate;
}

void ee_device_set_complete(const ee_device_t dev, int (*tx_complete)(ee_device_t dev, void* buffer))
{
    dev->tx_complete = tx_complete;
}

int ee_device_open(ee_device_t dev)
{
}

int ee_device_close(ee_device_t dev)
{
}
int ee_device_read(ee_device_t dev, uint32_t pos, void* buffer, uint32_t size)
{
}

int ee_device_write(ee_device_t dev, uint32_t pos, const void* buffer, uint32_t size)
{
}

int ee_device_control(ee_device_t dev, int cmd, void* args)
{
}
