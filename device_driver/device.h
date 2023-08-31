/**
 * ************************************************************************
 * 
 * @file device.h
 * @author yuyf (123.com)
 * @brief 
 * 
 * ************************************************************************
 * @copyright Copyright (c) 2023 XXX 
 * For study and research only, no reprinting
 * ************************************************************************
 */
#ifndef __DEVICE_H__
#define __DEVICE_H__
#include <stdint.h>

/**
 * ************************************************************************
 * @brief 设备类型
 * 
 * 
 * ************************************************************************
 */
enum device_class_type
{
    Device_Class_Char = 0,                           /**< character device */
    Device_Class_Block,                              /**< block device */
    Device_Class_NetIf,                              /**< net interface */
    Device_Class_MTD,                                /**< memory device */
    Device_Class_CAN,                                /**< CAN device */
    Device_Class_RTC,                                /**< RTC device */
    Device_Class_Sound,                              /**< Sound device */
    Device_Class_Graphic,                            /**< Graphic device */
    Device_Class_I2CBUS,                             /**< I2C bus device */
    Device_Class_USBDevice,                          /**< USB slave device */
    Device_Class_USBHost,                            /**< USB host bus */
    Device_Class_USBOTG,                             /**< USB OTG bus */
    Device_Class_SPIBUS,                             /**< SPI bus device */
    Device_Class_SPIDevice,                          /**< SPI device */
    Device_Class_SDIO,                               /**< SDIO bus device */
    Device_Class_PM,                                 /**< PM pseudo device */
    Device_Class_Pipe,                               /**< Pipe device */
    Device_Class_Portal,                             /**< Portal device */
    Device_Class_Timer,                              /**< Timer device */
    Device_Class_Miscellaneous,                      /**< Miscellaneous device */
    Device_Class_Sensor,                             /**< Sensor device */
    Device_Class_Touch,                              /**< Touch device */
    Device_Class_PHY,                                /**< PHY device */
    Device_Class_Security,                           /**< Security device */
    Device_Class_Unknown                             /**< unknown device */
};

/**
 * ************************************************************************
 * @brief 设备操作结构体
 * 
 * 
 * ************************************************************************
 */
typedef struct 
{
    //struct rt_object          parent;                   /**< inherit from rt_object */

    enum device_class_type type;                     /**< device type */
    uint16_t               flag;                     /**< device flag */
    uint16_t               open_flag;                /**< device open flag */

    uint8_t                ref_count;                /**< reference count */
    uint8_t                device_id;                /**< 0 - 255 */

    /* device call back */
    int (*rx_indicate)(device_t dev, uint32_t size);
    int (*tx_complete)(device_t dev, void *buffer);

    /**
     * ************************************************************************
     * @brief 用于存放对设备操作的函数
     * 
     * 
     * ************************************************************************
     */
    struct 
    {
        /* common device interface */
        int (*init)   (device_t dev);
        int (*open)   (device_t dev, uint32_t oflag);
        int (*close)  (device_t dev);
        int (*read)   (device_t dev, uint32_t pos, void *buffer, uint32_t size);
        int (*write)  (device_t dev, uint32_t pos, const void *buffer, uint32_t size);
        int (*control)(device_t dev, int cmd, void *args);
    }ops;

    void                     *user_data;                /**< device private data 一般用于存放配置文件的指针 */
}device, * device_t;
