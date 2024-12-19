# 文件介绍
根据做过的项目，将通用的模块提取出来

## 文件说明

| 文件                           | 说明                                                         | 测试结果 |
| ------------------------------ | ------------------------------------------------------------ | -------- |
| [async_work](./async_work)     | 异步作业，移植自[CodeBrick](git@gitee.com:moluo-tech/CodeBrick.git) | PASS     |
| pub_def                        | 定义一些类型别名，以及常见的宏                               |          |
| pub_macro                      | 将一些常见的操作使用宏进行封装                               |          |
| pub_memory                     | 针对RAM的一些操作，如内存赋值，拷贝，清空等                  |          |
| public                         | 常见的函数                                                   |          |
| mf_log                         | 彩色日志打印                                                 |          |
| [设备驱动](./device_driver)    | 根据rt-thread的框架，抽象出软件层面的设备框架，方便接口统一，分离驱动层和应用层 | PASS     |
| [eventos](./eventos_copy)      | 大佬写的事件驱动框架，貌似参考了[state-machine](https://www.state-machine.com/) |          |
| [filter_algo](./filter_algo)   | 常见的滤波算法，中位值，递推平均滤波算法等                   | PASS     |
| [flash](./flash)               | flash擦写均衡框架，参考了[FlashDB](https://github.com/armink/FlashDB) | PASS     |
| [func](./func)                 | 包含了自己写的一些框架，如 fifo、btn、blink、menu            | PASS     |
| [mr_mem](./mr_mem)             | 裸机的动态内存申请与释放，移植自 [mr-library](git@gitee.com:MacRsh/mr-library.git) | PASS     |
| [osal](./osal)                 | 对嵌入式多种rtos API的封装，方便做移植。该文件移植自[CherryUSB](https://github.com/cherry-embedded/CherryUSB) |          |
| [polling](./polling)           | 裸机框架，支持类似于rt-thread的自动初始化的方式。移植自[AT-Command内的none-os](https://gitee.com/moluo-tech/AT-Command/tree/master/samples/none_os) | PASS     |
| [UnityTesting](./UnityTesting) | 单元测试框架，来自[Unity](https://github.com/ThrowTheSwitch/Unity) |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |
|                                |                                                              |          |



## 待办事项

- [ ] 表驱动框架
- [x] 数据队列
- [ ] eventos-copy注释完善

表驱动框架，一般用在通讯协议或者事件驱动

