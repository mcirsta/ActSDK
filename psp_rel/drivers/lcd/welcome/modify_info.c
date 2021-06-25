#include "lcd_config.h"
#include "kmod_calls.h"

#define MAX_LEVEL 6        //应用程序所允许的最大分级数

typedef struct
{
    int max_back_light_level;       //背光的最大物理级数
    int max_level;                             //当前的分级数<=MAX_LEVEL
    unsigned char  level[MAX_LEVEL];	    //背光的物理级数，物理级数的最大值由工具调用检测程序得到
    unsigned int  resolution;                          //LCM的分辨率
    unsigned int  identification;
}light_level_t;

//修改act_light_level的第三个成员变量里的数组元素，可调节LCD背光的默认级别值
#ifdef BL_PWM
light_level_t  act_light_level =
{
    31,                //该屏的可设背光的最大物理级数
     5,                 //应用用于显示的最大级数
#ifdef IC_DS9300      //不同的背光控制IC，需要输入PWM的占空比不一样。
                                       //主流的IC有9300和5121，在“lcd_config.h”中配置（前提是先配置为PWM方式）
    {0,6,12,18,24,30},  //应用用于显示的最大级数+1个元素，第零个元素为关背光时的亮度，
                                     //从第一个元素开始为设置中的级数对应的物理级数
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
    5,                //该屏的可设背光的最大物理级数
    5,                 //应用用于显示的最大级数
    {0, 1, 2, 3, 4, 5},  //应用用于显示的最大级数+1个元素，第零个元素为关背光时的亮度，
                                 //从第一个元素开始为设置中的级数对应的物理级数
    0x014000f0,             //320  240
   MODIFY_MATCH_ID
};
#endif

#ifdef BL_GPIO
light_level_t  act_light_level =
{
     15,                //该屏的可设背光的最大物理级数
     5,                 //应用用于显示的最大级数
    {0, 2, 5, 8, 11, 15},  //应用用于显示的最大级数+1个元素，第零个元素为关背光时的亮度，
                                       //从第一个元素开始为设置中的级数对应的物理级数
    0x014000f0,             //320  240
    MODIFY_MATCH_ID	
};
#endif
