/*
 * softFilter.c
 *
 *  Created on: 2013-1-25
 *      Author: wang jing bing
 */

/*
中位值平均滤波法（又称防脉冲干扰平均滤波法）
A、方法：
    相当于“中位值滤波法”+“算术平均滤波法”
    连续采样N个数据，去掉一个最大值和一个最小值
    然后计算N-2个数据的算术平均值
        N值的选取：3~14
B、优点：
    融合了两种滤波法的优点
    对于偶然出现的脉冲性干扰，可消除由于脉冲干扰所引起的采样值偏差
C、缺点：
    测量速度较慢，和算术平均滤波法一样
    比较浪费RAM
*/
#include "softFilter.h"

/*
 * 功能：自定义滤波数组大小
 * 入口参数：
 * 	*ptr:指向一个装满待滤波数据数组的指针
 * 	n:数组的大小,需要取值为8的倍数
 *
 */
uint16_t filter(uint16_t *ptr,uint8_t n)
{
    int8_t count=0,i=0,j=0;
    uint16_t temp = 0;
    uint32_t sum=0;

    for (j=0;j<n-1;j++){
        for (i=0;i<n-1-j;i++){
            if (*(ptr+i) > *(ptr+i+1))
            {
                temp = *(ptr+i);
                *(ptr+i) = *(ptr+i+1);
                *(ptr+i+1) = temp;
            }
        }
    }
    //=====取出中间n/4个数进行算术平均============
    for(count=3*n/8;count<n*5/8;count++)
        sum += *(ptr+count);
    return (uint16_t)(sum/(n/4));
}
/*
 * 功能：事先宏定义滤波数组大小
 * 入口参数：
 * 	*ptr:指向一个装满待滤波数据数组的指针(数组的大小,需要取值为8的倍数)
 *
 */
uint16_t filter_N(uint16_t *ptr)
{
    int8_t count=0,i=0,j=0;
    uint16_t temp = 0;
    uint32_t sum=0;

    for (j=0;j<N_FT-1;j++){
        for (i=0;i<N_FT-1-j;i++){
            if (*(ptr+i) > *(ptr+i+1))
            {
                temp = *(ptr+i);
                *(ptr+i) = *(ptr+i+1);
                *(ptr+i+1) = temp;
            }
        }
    }
    //=====取出中间N_FT/4个数进行算术平均============
    for(count=N_FT*3/8;count<N_FT*5/8;count++)
        sum += *(ptr+count);
    return (uint16_t)(sum/(N_FT/4));

}


