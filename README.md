# pub_function
**通用模块**

# 文件介绍
1. fifo.* 先进先出模块，暂未实现可变数组扩展
2. log_print.* log模块，分等级打印调试信息
3. pub_macro.h 公用宏定义模块，需要继续扩展
4. pub_memory.* 内存操作模块
5. public.h 公用的小函数，需要继续扩展
    5.1 pub_bcd2dec：十六进制BCD码转10进制
    5.2 pub_dec2bcd：十进制转十六进制BCD码
    5.3 pub_sprintf: 格式化字符串
6. memory_operate.* 内存操作模块，支持读flash（可指定输出方式：字符串、字、半字、字节，指定数   量），支持读写ram。内部会校验输入地址是否准确
7. eeprom模块，支持eeprom新增/删除参数（须保证已经存在的参数不变），
8. 状态机模块，实现功能待定
9. 写log模块，保存运行中的错误信息，待定
10. comm_buffer 用于数据缓冲的管理，接收，发送的缓冲区可以用该模块管理
11. mulit_key 多个按键扫描模块，支持ADC采集按键或读取IO口电平按键
12. mulitmenu 多级菜单模块，用户只需关注各级菜单实现的功能即可
13. ring_buf_deal 对环形缓冲区的数据进行断帧
