# pub_function
通用模块

# 文件介绍
1. fifo.* 先进先出模块，暂未实现可变数组扩展
2. log.print.* log模块
3. pub_macro.h 公用宏定义模块，需要继续扩展
4. pub_memory.* 内存操作模块
5. public.h 公用的小函数，需要继续扩展
    5.1 pub_hex2bcd：十六进制BCD码转10进制
    5.2 pub_bcd2hex：十进制转十六进制BCD码
    5.3 pub_sprintf: 格式化字符串
6. memory_operate.* 内存操作模块，支持读flash（可指定输出方式：字符串、字、半字、字节，指定数   量），支持读写ram。内部会校验输入地址是否准确
7. eeprom模块，支持eeprom新增/删除参数（须保证已经存在的参数不变），
8. 状态机模块，实现功能待定