/*
 * softFilter.c
 *
 *  Created on: 2013-1-25
 *      Author: wang jing bing
 */

/*
��λֵƽ���˲������ֳƷ��������ƽ���˲�����
A��������
    �൱�ڡ���λֵ�˲�����+������ƽ���˲�����
    ��������N�����ݣ�ȥ��һ�����ֵ��һ����Сֵ
    Ȼ�����N-2�����ݵ�����ƽ��ֵ
        Nֵ��ѡȡ��3~14
B���ŵ㣺
    �ں��������˲������ŵ�
    ����żȻ���ֵ������Ը��ţ������������������������Ĳ���ֵƫ��
C��ȱ�㣺
    �����ٶȽ�����������ƽ���˲���һ��
    �Ƚ��˷�RAM
*/
#include "softFilter.h"

/*
 * ���ܣ��Զ����˲������С
 * ��ڲ�����
 * 	*ptr:ָ��һ��װ�����˲����������ָ��
 * 	n:����Ĵ�С,��ҪȡֵΪ8�ı���
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
    //=====ȡ���м�n/4������������ƽ��============
    for(count=3*n/8;count<n*5/8;count++)
        sum += *(ptr+count);
    return (uint16_t)(sum/(n/4));
}
/*
 * ���ܣ����Ⱥ궨���˲������С
 * ��ڲ�����
 * 	*ptr:ָ��һ��װ�����˲����������ָ��(����Ĵ�С,��ҪȡֵΪ8�ı���)
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
    //=====ȡ���м�N_FT/4������������ƽ��============
    for(count=N_FT*3/8;count<N_FT*5/8;count++)
        sum += *(ptr+count);
    return (uint16_t)(sum/(N_FT/4));

}


