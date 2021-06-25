#include "lcd_config.h"
#include "kmod_calls.h"

#define MAX_LEVEL 6        //Ӧ�ó�������������ּ���

typedef struct
{
    int max_back_light_level;       //��������������
    int max_level;                             //��ǰ�ķּ���<=MAX_LEVEL
    unsigned char  level[MAX_LEVEL];	    //������������������������ֵ�ɹ��ߵ��ü�����õ�
    unsigned int  resolution;                          //LCM�ķֱ���
    unsigned int  identification;
}light_level_t;

//�޸�act_light_level�ĵ�������Ա�����������Ԫ�أ��ɵ���LCD�����Ĭ�ϼ���ֵ
#ifdef BL_PWM
light_level_t  act_light_level =
{
    31,                //�����Ŀ��豳������������
     5,                 //Ӧ��������ʾ�������
#ifdef IC_DS9300      //��ͬ�ı������IC����Ҫ����PWM��ռ�ձȲ�һ����
                                       //������IC��9300��5121���ڡ�lcd_config.h�������ã�ǰ����������ΪPWM��ʽ��
    {0,6,12,18,24,30},  //Ӧ��������ʾ�������+1��Ԫ�أ������Ԫ��Ϊ�ر���ʱ�����ȣ�
                                     //�ӵ�һ��Ԫ�ؿ�ʼΪ�����еļ�����Ӧ��������
#elif defined(IC_G5121)
    {0,5,7,9,11,13}, 
#else
    {0,6,12,18,24,30}, 
#endif
    0x014000f0,             //320  240
    MODIFY_MATCH_ID	
};
#endif

#ifdef BL_DC2DC
light_level_t  act_light_level =
{
    5,                //�����Ŀ��豳������������
    5,                 //Ӧ��������ʾ�������
    {0, 1, 2, 3, 4, 5},  //Ӧ��������ʾ�������+1��Ԫ�أ������Ԫ��Ϊ�ر���ʱ�����ȣ�
                                 //�ӵ�һ��Ԫ�ؿ�ʼΪ�����еļ�����Ӧ��������
    0x014000f0,             //320  240
   MODIFY_MATCH_ID
};
#endif

#ifdef BL_GPIO
light_level_t  act_light_level =
{
     15,                //�����Ŀ��豳������������
     5,                 //Ӧ��������ʾ�������
    {0, 2, 5, 8, 11, 15},  //Ӧ��������ʾ�������+1��Ԫ�أ������Ԫ��Ϊ�ر���ʱ�����ȣ�
                                       //�ӵ�һ��Ԫ�ؿ�ʼΪ�����еļ�����Ӧ��������
    0x014000f0,             //320  240
    MODIFY_MATCH_ID	
};
#endif
