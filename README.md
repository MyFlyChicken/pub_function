# 文件介绍
根据做过的项目，将通用的模块提取出来
## communication


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
9. 写log模块，保存运行中的错误信息，待
