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
#include <string.h>
#include <stdint.h>

#define MF_ASSERT_EN (1U)
#include "../mf_log.h"

static ee_device_t head = NULL;

void ee_device_register(const ee_device_t dev, const ee_obj_t obj, const ee_ops_t ops, const void* user_data)
{
    ee_device_t tail = head;

    MF_ASSERT(dev);
    MF_ASSERT(obj);
    MF_ASSERT(ops);

    dev->obj       = *obj;
    dev->ops       = (ee_ops_t)ops;
    dev->user_data = (void*)user_data;

    dev->next = NULL;
    if (head == NULL) {
        head = dev;
        return;
    }
    while (tail->next != NULL)
        tail = tail->next;
    tail->next = dev;
}

void ee_device_set_indicate(const ee_device_t dev, int (*rx_indicate)(ee_device_t dev, uint32_t size))
{
    MF_ASSERT(dev);

    dev->rx_indicate = rx_indicate;
}

void ee_device_set_complete(const ee_device_t dev, int (*tx_complete)(ee_device_t dev, void* buffer))
{
    MF_ASSERT(dev);

    dev->tx_complete = tx_complete;
}

ee_device_t ee_device_find(const char* name)
{
    ee_device_t dev = head;

    MF_ASSERT(name);

    if (!dev) {
        return NULL;
    }

    do {
        if (0 == strcmp(dev->obj.name, name)) {
            return dev;
        }
        else {
            dev = dev->next;
        }
    } while (head->next != NULL);

    return NULL;
}

int ee_device_init(ee_device_t dev)
{
    int ret;

    MF_ASSERT(dev);
    ret = dev->ops->init(dev);

    return ret;
}

int ee_device_open(ee_device_t dev, uint32_t oflag)
{
    int ret;

    MF_ASSERT(dev);
    ret = dev->ops->open(dev, oflag);

    return ret;
}

int ee_device_close(ee_device_t dev)
{
    int ret;

    MF_ASSERT(dev);

    ret = dev->ops->close(dev);

    return ret;
}

int ee_device_read(ee_device_t dev, uint32_t pos, void* buffer, uint32_t size)
{
    int ret;

    MF_ASSERT(dev);
    MF_ASSERT(buffer);

    ret = dev->ops->read(dev, pos, buffer, size);

    return ret;
}

int ee_device_write(ee_device_t dev, uint32_t pos, const void* buffer, uint32_t size)
{
    int ret;

    MF_ASSERT(dev);
    MF_ASSERT(buffer);

    ret = dev->ops->write(dev, pos, buffer, size);

    return ret;
}

int ee_device_control(ee_device_t dev, int cmd, void* args)
{
    int ret;

    MF_ASSERT(dev);

    ret = dev->ops->control(dev, cmd, args);

    return ret;
}
