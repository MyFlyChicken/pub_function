/**
 * @file ee_device.h
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
#ifndef __ee_device_H_ //shift+U转换为大写
#define __ee_device_H_

#include <cstdint>
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define EE_NAME_MAX (12U)

enum ee_device_class_type {
    Device_Class_Char = 0, /**< character device */
};

//typedef struct ee_obj* ee_obj_t;
typedef struct {
    char                      name[EE_NAME_MAX]; /**< name of kernel object */
    enum ee_device_class_type type;              /**< device type */
    uint16_t                  flag;              /**< flag of kernel object */
    uint16_t                  open_flag;         /**< device open flag */
    uint8_t                   ref_count;         /**< reference count */
} ee_obj, *ee_obj_t;

//typedef struct ee_device* ee_device_t;
typedef struct {
    struct ee_obj parent;

    ee_ops_t ops;

    uint8_t device_id; /**< 保留 0 - 255 */

    /* device call back */
    int (*rx_indicate)(ee_device_t dev, uint32_t size);
    int (*tx_complete)(ee_device_t dev, void* buffer);

    void* user_data; /**< device private data 一般用于存放配置文件的指针 */
} ee_device, *ee_device_t;

//typedef struct ee_ops* ee_ops_t;
typedef struct {
    /* common device interface */
    int (*init)(ee_device_t dev);
    int (*open)(ee_device_t dev, uint32_t oflag);
    int (*close)(ee_device_t dev);
    int (*read)(ee_device_t dev, uint32_t pos, void* buffer, uint32_t size);
    int (*write)(ee_device_t dev, uint32_t pos, const void* buffer, uint32_t size);
    int (*control)(ee_device_t dev, int cmd, void* args);
} ee_ops, *ee_ops_t;

void ee_device_register(const ee_device_t dev, const ee_obj_t parent, const ee_ops_t ops, const void* user_data);
void ee_device_set_indicate(const ee_device_t dev, int (*rx_indicate)(ee_device_t dev, uint32_t size));
void ee_device_set_complete(const ee_device_t dev, int (*tx_complete)(ee_device_t dev, void* buffer));

#ifdef __cplusplus
}
#endif

#endif
