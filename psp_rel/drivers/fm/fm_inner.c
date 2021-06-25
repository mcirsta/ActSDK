/********************************************************************************
 *                              这里填写项目名
 *                            Module: FM Drvier
 *                 Copyright(c) 2001-2007 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xkping     2008-07-03 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file   fm_inner.c
 * \brief  inner functions to implemet user ioctl
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File
 *       1) implement functions for really funtions
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/

#include "fm_inner.h"
#include "fm_hw.h"
#include "fm_sys.h"
#include "radio.h"
#include "gpio_config.h"
/*!
 *    MODULE_OPEN_COUNT ,count how many times does the applications had opened the drivers\n
 */
static int MODULE_OPEN_COUNT = 0;

#pragma __PRQA_IGNORE_START__
struct radio_info_t
radio_info =
{
    .current_freq = 87500,
    .stereo_status = 1, //mono mode
    .band_mode = US, //band info
    .intensity =5,
    .seek_status = -999
};
#pragma __PRQA_IGNORE_END__
/*!
 *  \brief
 *  threshold,判断是否是台的信号强度门槛值
 *   \li 5: default intensity    \n
 */
int threshold = 5;

//static void _udelay(unsigned int time)
//{
//    volatile unsigned int i, j;
//
//    for (i = 0; i < time; i++)
//    {
//        for (j = 0; j < 600; j++)
//        {
//            do
//            {
//            } while (0);
//        }
//    }
//}

/******************************************************************************/
/*!
 * \par  Description:
 *     init host hardware for using fm ,
 *       open vccout,
 *       enable i2c clock
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
void host_hardware_init(void)
{
    #if IF_HAVE_NO_OSCILLATION
    int i = 2000;
    /*set the FM clk*/
    act_writel( ( (act_readl(CMU_FMCLK)| 0x1) &  (~(0x2)) ), CMU_FMCLK); 
    while(i--); //mdelay(500);
    #endif
    /*! open vcc out */
    act_writel(act_readl(PMU_CTL) | (1 << 14), PMU_CTL);

//    /*! open I2C CLK */
//    act_writel(((0x1 << 23) | act_readl(CMU_DEVCLKEN)), CMU_DEVCLKEN);
}

/******************************************************************************/
/*!
 * \par  Description:
 *     restore host hardware have been changed by this drvier  ,
 *     inculde vccout, i2c clock.
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
void host_hardware_exit(void)
{
    unsigned int tmp;
    
    /*fm clk output disable的时候，会产生一个方波。
        若fm clk output的时候，刚好是高电平，则会产生一个从高到底的跳变，
        而该方波会导致RTC复位，所以系统时间会出错
        故注释下面部分*/
//    #if IF_HAVE_NO_OSCILLATION
//    int i = 2000;
//    /*disable the FM clk*/
//    act_writel( ( act_readl(CMU_FMCLK) &  (~(0x1)) ), CMU_FMCLK); 
//    while(i--); //delay
//    #endif

    /*! close vccout */
    tmp = act_readl(PMU_CTL);
    tmp &= ~(1 << 14);
    act_writel(tmp, PMU_CTL);

    /*! I2C clk disable*/
//    tmp = act_readl(CMU_DEVCLKEN);
//    tmp &= ~(0x1 << 23);
//    act_writel(tmp, CMU_DEVCLKEN);
}

/******************************************************************************/
/*!
 * \par  Description:
 *     init host hardware register;
 *     init fm hardware register;
 *     init some global value;
 * \param[in]   none
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      other failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int act_radio_open(void)
{
    int ret = 0;

    MODULE_OPEN_COUNT++;

    if (MODULE_OPEN_COUNT == 1)
    {
        host_hardware_init();
        ret = fm_init(0);
    }
    return ret;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   none
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int act_radio_release(void)
{
    int ret = 0;

    MODULE_OPEN_COUNT--;

    if (MODULE_OPEN_COUNT == 0)
    {
        ret = fm_exit();
        host_hardware_exit();

    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   a  para1
 * \param[in]   b  para2
 * \param[in]   c  para3
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
ssize_t act_radio_read(void)
{
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   a  para1
 * \param[in]   b  para2
 * \param[in]   c  para3
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
ssize_t act_radio_write(void)
{
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   param
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess ,yes, the earphone is inserted
 * \retval      -1 failed, the earphone is not inserted
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int is_earphone_inserted(unsigned long param)
{

    act_writel(act_readl(SPEAKER_GPIO_OUTEN) & ~(0x1 << SPEAKER_GPIO_NUM), SPEAKER_GPIO_OUTEN);
    act_writel(act_readl(SPEAKER_GPIO_INEN) | (0x1 << SPEAKER_GPIO_NUM), SPEAKER_GPIO_INEN);

    //mdelay(160);

    if (((act_readl(SPEAKER_GPIO_DATA) >> SPEAKER_GPIO_NUM )&0x1) !=0)
    {
        *((unsigned int *)param) = 0;//the earphone is not inserted
        //return -1; 
    }
    else
    {
        *((unsigned int *)param) = 1;//the earphone is inserted	
    }

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *   much work is done in this func:
 *   1 test whether the current freq is valid station
 *   2 get info about this station and save in the global var fm_info
 * \param[in]   int freq, set freq
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int is_freq_valid_station(int freq)
{
    int ret = -1;
    int is_valid_station = 0;

#if 0
    if( (radio_info.bandinfo == US_BAND) && IS_INVALID_US_FREQ(freq))
    return -1;

    if( (radio_info.bandinfo == JAPAN_BAND) && IS_INVALID_JPAN_FREQ(freq))
    return -1;
#endif

    //fm_set_mute(1);
    /* set to mute */
    fm_ioctl(SET_FMHW_MUTE, 1);
    ret = fm_ioctl(SET_FMHW_FREQ, (unsigned long) freq);//fm_judge_station(freq);
    //printk("return value of fm_ioctl is %d\n", ret);
    if (ret == 0)
    {
        is_valid_station = 1;
    }

    //fm_get_intensity(&radio_info.intensity);
    //fm_get_stereo_mode(&radio_info.stereo_mode);
    fm_ioctl(GET_FMHW_INTENSITY, (unsigned long) &radio_info.intensity);
    fm_ioctl(GET_FMHW_STEREO_MODE, (unsigned long) &radio_info.stereo_status);

    /*!
     *   yes we searched a valid station, but we need to test the threshold
     */
    if (is_valid_station != 0)
    {
        if (radio_info.intensity < (unsigned int)threshold)
        {
            return -1;
        }

        //fm_set_mute(0);
        fm_ioctl(SET_FMHW_MUTE, 0); /* begin to play fm */
        return 0; //success
    }

    return -1; //failure
}

/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   unsigned int cmd, define in fm_drv.h
 * \param[in]   unsigned long arg, para for cmd
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      other failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int act_radio_ioctl(unsigned int cmd, unsigned long param)
{
    int ret = 0;
    //printk("act radio cmd =%x\n", cmd);
    switch (cmd)
    {
        case RADIO_GET_STEREO_MODE:
        //put_user((unsigned long)radio_info.stereo_status, (unsigned long *)param);
        fm_ioctl(GET_FMHW_STEREO_MODE, (unsigned long) &radio_info.stereo_status);
        *((int *) param) = (int)radio_info.stereo_status;
        break;

        case RADIO_GET_INFO:
        ret = check_seek_status(&radio_info);
        copy_to_user((unsigned long *)param , (unsigned long *) (&radio_info), sizeof(struct radio_info_t));
        break;

        case RADIO_GET_INTENSITY:
        // put_user((unsigned long)radio_info.intensity, (unsigned long *)param);
        fm_ioctl(GET_FMHW_INTENSITY, (unsigned long) &radio_info.intensity);
        *((int *) param) = (int)radio_info.intensity;
        break;

        case RADIO_GET_BAND_MODE:
        // put_user((unsigned long)radio_info.intensity, (unsigned long *)param);
        //fm_ioctl(GET_FMHW_BAND_MODE, (unsigned long) &radio_info.band_mode);
        *((int *) param) = (int)radio_info.band_mode;
        break;

        case RADIO_GET_FREQ:
        //put_user((unsigned long)radio_info.current_freq, (unsigned long *)param);
        fm_get_freq(&radio_info);
        *((int *) param) = (int)radio_info.current_freq;
        break;

        case RADIO_GET_ANTENNA:
        ret = is_earphone_inserted(param);
        break;

        case RADIO_SET_BAND_MODE:
        //printf("set band %d\n", param);
        ret = fm_ioctl(SET_FMHW_BAND_MODE, param);
        if (ret == 0)
        {
            //printf("set band %d\n", param);
            radio_info.band_mode = param;
        }
        break;

        case RADIO_SET_FREQ:
        ret = fm_set_freq((int) param);
        if (ret != -EIO)
        {
            radio_info.current_freq = param;
        }
        break;

        case RADIO_SET_PLAY:
        ret = fm_ioctl(SET_FMHW_MUTE, 0);//fm_set_mute(0);
        break;

        case RADIO_SET_STOP:
        ret = fm_ioctl(SET_FMHW_MUTE, 1);//fm_set_mute(1);
        break;

        case RADIO_SET_THRESHOLD:
        threshold = (int) param;
        break;

        case RADIO_SET_ENTER_STANDBY:
        fm_ioctl(SET_FMHW_ENTER_STANDBY, param);
        break;

        case RADIO_SET_AUTO_SEEK:
        ret = fm_ioctl(RADIO_SET_AUTO_SEEK, param);
        break;
        
        case RADIO_SET_AUTO_SEEK_BREAK:
        ret = fm_ioctl(RADIO_SET_AUTO_SEEK_BREAK, param);	
        break;
        
        case RADIO_SET_FREQ_NO_CHECK:
        ret = fm_set_freq_no_check((int) param);	
        break;
        
        
        case RADIO_CHECK_LINEIN:
        ret = check_linein(param);
        break;
        default:
        ret = -1;
        break;
    }

    return ret;

}

