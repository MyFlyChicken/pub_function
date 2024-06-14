#include "test.h"
#include <stdint.h>
#include <stdio.h>
#include "../device_driver/ee_device.h"

static int ee_sim_init(ee_device_t dev)
{
    (void)dev;
    printf("ee_sim_init\r\n");
    return 0;
}

static int ee_sim_open(ee_device_t dev, uint32_t oflag)
{
    (void)dev;
    (void)oflag;
    printf("ee_sim_open\r\n");
    return 0;
}

static int ee_sim_close(ee_device_t dev)
{
    (void)dev;
    printf("ee_sim_close\r\n");
    return 0;
}

static int ee_sim_read(ee_device_t dev, uint32_t pos, void* buffer, uint32_t size)
{
    (void)dev;
    (void)pos;
    (void)buffer;
    (void)size;
    printf("ee_sim_read\r\n");
    return 0;
}
static int ee_sim_write(ee_device_t dev, uint32_t pos, const void* buffer, uint32_t size)
{
    (void)dev;
    (void)pos;
    (void)buffer;
    (void)size;
    printf("ee_sim_write\r\n");
    return 0;
}

static int ee_sim_control(ee_device_t dev, int cmd, void* args)
{
    (void)dev;
    (void)cmd;
    (void)args;
    printf("ee_sim_control\r\n");
    return 0;
}

struct ee_ops ops = {
    .init    = ee_sim_init,
    .open    = ee_sim_open,
    .close   = ee_sim_close,
    .read    = ee_sim_read,
    .write   = ee_sim_write,
    .control = ee_sim_control,
};

struct ee_obj obj = {
    .name = "ee_test",
};

struct ee_obj obj2 = {
    .name = "ee_test2",
};

void test_ee_device(void)
{
    struct ee_device dev1;
    struct ee_device dev2;
    ee_device_t      dev_find;
    char             buffer[6];

    TEST_FUNCTION_BEGIN();
    ee_device_register(&dev1, &obj, &ops, NULL);
    ee_device_register(&dev2, &obj2, &ops, NULL);

    printf("===>>>ee_test\r\n");
    dev_find = ee_device_find("ee_test");
    printf("%08x, %08x\r\n", (unsigned int)&dev1, (unsigned int)dev_find);
    dev_find->ops->init(dev_find);
    dev_find->ops->open(dev_find, 0);
    dev_find->ops->close(dev_find);
    dev_find->ops->read(dev_find, 0, buffer, 6);
    dev_find->ops->write(dev_find, 0, buffer, 6);
    dev_find->ops->control(dev_find, 0, NULL);

    printf("===>>>ee_test2\r\n");
    dev_find = ee_device_find("ee_test2");
    printf("%08x, %08x\r\n", (unsigned int)&dev2, (unsigned int)dev_find);
    dev_find->ops->init(dev_find);
    dev_find->ops->open(dev_find, 0);
    dev_find->ops->close(dev_find);
    dev_find->ops->read(dev_find, 0, buffer, 6);
    dev_find->ops->write(dev_find, 0, buffer, 6);
    dev_find->ops->control(dev_find, 0, NULL);
    TEST_FUNCTION_END();
}
