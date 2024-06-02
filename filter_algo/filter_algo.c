/* ----------------------- System includes ----------------------------------*/

/* ----------------------- Platform includes --------------------------------*/
#include "filter_algo.h"

#include "../mf_log.h"
#include <stdint.h>

/* ----------------------- Defines ------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif

#define ALGO_ASSERT(x) MF_ASSERT(x)

/* ----------------------- Type definitions ---------------------------------*/
typedef struct
{
    int16_t data[MOVING_AVERAGE_FILTER_SIZE];
    int16_t count; // 记录已填充的数据个数
    int16_t index;
    int16_t sum;
} moving_average_filter;

typedef struct
{
    void* (*get_currval)(unsigned char num);
} filter_inf_t;
/* ----------------------- Static variables ---------------------------------*/
filter_inf_t _filter_inf;

/* ----------------------- Static function ----------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
/**
 * @brief 限幅滤波法
 *       1、方法：
 *       根据经验判断两次采样允许的最大偏差值（设为A）
 *       每次检测到新值时判断：
 *       a. 如果本次值与上次值之差<=A，则本次值有效
 *       b. 如果本次值与上次值之差>A，则本次值无效，放弃本次值，用上次值代替本次值
*
 *       2、优点：
 *       能有效克服因偶然因素引起的脉冲干扰
 *
 *       3、缺点
 *       无法抑制那种周期性的干扰
 *       平滑度差
 *
 * @return char 
 * 
 * @details 
 */
int16_t filter1(void)
{
/* A值根据实际调，value有效值，value_new当前采样值，程序返回有效的实际值 */
#define DELT_VAL 10
    static int16_t value = 0;
    int16_t        value_new;

    ALGO_ASSERT(_filter_inf.get_currval);

    value_new = *(int16_t*)_filter_inf.get_currval(1); // 获取采样值
    if (abs(value_new - value) > DELT_VAL)
        return value; // abs()取绝对值函数

    value = value_new;
    return value_new;
}

/**
 * @brief 中位值滤波法
 *        1、方法：
 *        连续采样N次（N取奇数），把N次采样值按大小排列
 *        取中间值为本次有效值
 *
 *        2、优点：
 *        能有效克服因偶然因素引起的波动干扰
 *        对温度、液位的变化缓慢的被测参数有良好的滤波效果
 *
 *        3、缺点：
 *        对流量、速度等快速变化的参数不宜
 *
 * @param [in] num 设置一次读取原始数据的数量，num必须大于1
 * @return int16_t 返回中间值
 * 
 * @details 
 */
int16_t filter2(int16_t num)
{
    int16_t* value_buf;
    int16_t  i, j, temp;

    ALGO_ASSERT(_filter_inf.get_currval);
    ALGO_ASSERT(num > 1);
    //TODO 直接修改原始数据合理么？
    value_buf = (int16_t*)_filter_inf.get_currval(num);
    for (j = 0; j < (num - 1); j++) {
        for (i = 0; i < (num - j - 1); i++) {
            if (value_buf[i] > value_buf[i + 1]) {
                temp             = value_buf[i];
                value_buf[i]     = value_buf[i + 1];
                value_buf[i + 1] = temp;
            }
        }
    }
    // for (i = 0; i < num; i++)
    // {
    //     printf("%d\r\n", value_buf[i]);
    // }
    return value_buf[(num - 1) / 2];
}

/**
 * @brief 算术平均滤波法
 *        1、方法：
 *       连续取N个采样值进行算术平均运算
 *       N值较大时：信号平滑度较高，但灵敏度较低
 *       N值较小时：信号平滑度较低，但灵敏度较高
 *       N值的选取：一般流量，N=12；压力：N=4
 *       
 *       2、优点：
 *       适用于对一般具有随机干扰的信号进行滤波
 *       这样信号的特点是有一个平均值，信号在某一数值范围附近上下波动
 *       
 *       3、缺点：
 *       对于测量速度较慢或要求数据计算速度较快的实时控制不适用
 *       比较浪费RAM
 * @param [in] num 设置一次读取原始数据的数量，num必须大于2
 * @return int16_t 
 * 
 * @details 
 */
int16_t filter3(int16_t num)
{
    int16_t* value_buf;
    float    average; //避免越界
    ALGO_ASSERT(_filter_inf.get_currval);
    ALGO_ASSERT(num > 2);

    value_buf = (int16_t*)_filter_inf.get_currval(num);

    average = value_buf[0];
    for (int i = 1; i < num; i++) {
        average = average + (value_buf[i] - average) / (i + 1);
    }

    return (int16_t)average;
}

/**
 * @brief 递推平均滤波法
 * @param [in] num 设置计算平均值的数据个数，num必须大于2
 * @return int16_t 
 * 
 * @details 
 *        1、方法：
 *        把连续取N个采样值看成一个队列
 *        队列的长度固定为N
 *        每次采样到一个新数据放入队尾，并扔掉原来队首的一次数据.(先进先出原则)
 *        把队列中的N个数据进行算术平均运算，就可获得新的滤波结果
 *        N值的选取：流量，N=12；压力：N=4；液面，N=4 ~ 12；温度，N=1 ~ 4
 *        
 *        2、优点：
 *        对周期性干扰有良好的抑制作用，平滑度高
 *        适用于高频振荡的系统
 *        
 *        3、缺点：
 *        灵敏度低
 *        对偶然出现的脉冲性干扰的抑制作用较差
 *        不易消除由于脉冲干扰所引起的采样值偏差
 *        不适用于脉冲干扰比较严重的场合
 *        比较浪费RAM
 *        注意，前MOVING_AVERAGE_FILTER_SIZE计算会有误差，可以直接省略
 */
int16_t filter4(void)
{
    static moving_average_filter move_filt = {0};
    int16_t                      new_value;

    ALGO_ASSERT(_filter_inf.get_currval);

    new_value = *(int16_t*)_filter_inf.get_currval(1);
    // 移出旧值
    move_filt.sum -= move_filt.data[move_filt.index];

    // 添加新值
    move_filt.data[move_filt.index]  = new_value;
    move_filt.sum                   += new_value;

    // 更新索引（循环使用队列）
    move_filt.index = (move_filt.index + 1) % MOVING_AVERAGE_FILTER_SIZE;

    // 返回当前平均值
    return move_filt.sum / MOVING_AVERAGE_FILTER_SIZE;
}

/**
 * @brief 中位值平均滤波法
 *        1、方法：
 *        相当于“中位值滤波法”+“算术平均滤波法”
 *        连续采样N个数据，去掉一个最大值和一个最小值
 *        然后计算N-2个数据的算术平均值
 *        N值的选取：3~14
 *        
 *        2、优点：
 *        融合了两种滤波法的优点
 *        对于偶然出现的脉冲性干扰，可消除由于脉冲干扰所引起的采样值偏差
 *        
 *        3、缺点：
 *        测量速度较慢，和算术平均滤波法一样
 *        比较浪费RAM

 * @param [in] num 设置一次读取原始数据的数量，num必须大于2
 * @return int16_t 
 * 
 * @details 
 */
int16_t filter5(int16_t num)
{
    uint8_t  i, j;
    int16_t* value_buf;
    int32_t  tmp = 0;
    float    average;
    ALGO_ASSERT(_filter_inf.get_currval);
    ALGO_ASSERT(num > 2);

    value_buf = (int16_t*)_filter_inf.get_currval(num);

    //TODO 直接修改原始数据合理么？
    for (j = 0; j < (num - 1); j++) {
        for (i = 0; i < (num - j - 1); i++) {
            if (value_buf[i] > value_buf[i + 1]) {
                tmp              = value_buf[i];
                value_buf[i]     = value_buf[i + 1];
                value_buf[i + 1] = tmp;
            }
        }
    }

    average = value_buf[1];
    for (i = 2; i < num - 1; i++) {
        //printf("%f\r\n", average);
        average = average + (value_buf[i] - average) / (i + 1);
    }

    return (int16_t)(average);
}

/**
 * @brief 一阶滞后滤波算法
 *        1、方法：
 *        取ratio=0~1
 *        本次滤波结果=（1-ratio）本次采样值+ratio上次滤波结果
 *        
 *        2、优点：
 *        对周期性干扰具有良好的抑制作用
 *        适用于波动频率较高的场合
 *        
 *        3、缺点：
 *        相位滞后，灵敏度低
 *        滞后程度取决于a值大小
 *        不能消除滤波频率高于采样频率的1/2的干扰信号

 * @param [in] prev_value 上一次采样值
 * @param [in] curr_value 当前采样值
 * @param [in] ratio 滞后程度取决于ratio值大小，取值（0~100）
 * @return int16_t 
 * 
 * @details 
 */
int16_t filter6(int16_t prev_value, int16_t curr_value, int16_t ratio)
{
    ALGO_ASSERT(_filter_inf.get_currval);
    ALGO_ASSERT(ratio <= 100);

    return ((100 - ratio) * prev_value + ratio * curr_value) / 100;
}

/**
 * @brief 二阶滞后滤波算法
 *        
 * @return int16_t 
 * 
 * @details 
 */
int16_t filter7(int16_t prev_value, uint8_t ratio1, uint8_t ratio2)
{
    return 0;
}

/**
 * @brief 加权平均滤波算法
 *        1、方法：
 *        不同时刻的数据加以不同的权
 *        通常是，越接近现时刻的数据，权取得越大。
 *        给予新采样值的权系数越大，则灵敏度越高，但信号平滑度越低
 *        
 *        2、优点：
 *        适用于有较大纯滞后时间常数的对象
 *        和采样周期较短的系统
 *        
 *        3、缺点：
 *        对于纯滞后时间常数较小，采样周期较长，变化缓慢的信号
 *        不能迅速反应交易系统当前所受干扰的严重程度，滤波效果差
 *        
 * @param [in] weights 权重原始数据，数据应为0~100，实际为0.00~1.00，扩大了100倍方便计算
 * @param [in] weights_num 权重个数
 * @param [in] sa_num 采样数，取值（>2）
 * @return int16_t 
 * 
 * @details 
 */
int16_t filter8(int16_t* weights, int16_t weights_num, int16_t sa_num)
{
    int32_t  sum = 0;
    int16_t  i;
    int16_t* value_buf;

    ALGO_ASSERT(_filter_inf.get_currval);
    ALGO_ASSERT(weights_num == sa_num);
    ALGO_ASSERT(sa_num > 2);

    value_buf = (int16_t*)_filter_inf.get_currval(sa_num);

    // 计算加权平均值
    i    = 0;
    sum += value_buf[i] * weights[i];
    while (--sa_num) {
        sum  += value_buf[i] * weights[i];
        sum >>= 1;
    }

    return (int16_t)(sum / 100);
}

int16_t filter_inf_register(void* (*get_currval)(unsigned char num))
{
    ALGO_ASSERT(get_currval);

    _filter_inf.get_currval = get_currval;

    return 0;
}
