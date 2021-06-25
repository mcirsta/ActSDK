/*
 ********************************************************************************
 *                   213x --- G Sensor's driver
 *                (c) Copyright 2007 - 2010, Actions Co,Ld.
 *                        All Right Reserved
 *
 * History:
 *      <author>      <time>       <version >    <desc>
 *       houjingkun      2008/06/26     1.0         build this file
 ********************************************************************************/
/*!
 * \file   gsensor_hw.h
 * \brief
 *      gsensor's driver hardware layer's API.
 * \author houjingkun
 * \par GENERAL DESCRIPTION:
 *      realize gsensor driver's hardware layer's API.
 * \par EXTERNALIZED FUNCTIONS:
 *       null
 *
 *     Copyright(c) 2008-2012 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/06/26
 *******************************************************************************/

#include "gsensor_sys.h"
#include "i2c.h"
#include "gsensor_hw.h"

static int _gsensor_ctl_init(void);
static int _gsensor_ic_init(void);

/* gsensor driver's data structure. */
typedef struct
{
    unsigned int buf[6];
    gsensor_data_t ms_data;
    unsigned int old_clk_state; /* reserved clk state before G Sensor device open or install. */
    void *pMs_i2c;
    unsigned int* pMs_i2c_data;
    int ms_init_cnt;
} ms_dev_t;

static ms_dev_t ms_dev;

/* gsensor ic initializing data.
 static unsigned int ms_ic_init_data[] =
 {
 0x20, 0x47,
 0x21, 0x00,
 0x22, 0x04,
 };*/

#ifdef _MS_HW_NO_OS_

/* if system is 120M HZ, and 120 MIPS, 10 instruction one cycle, then udelay's count is 120 / 10 * 1000 = 12000. */
/*static void udelay(unsigned int time)
 {
 volatile unsigned int i, j;

 for (i = 0; i < time; i++)
 {
 for (j = 0; j < 100; j++)
 {
 do {} while (0);
 }
 }
 return;
 }    */
#endif  /* _MS_HW_NO_OS_ */

/* init gsensor related hardware in actions-semi ic.. */
int _gsensor_ctl_init(void)
{
    unsigned int tmp;

    /* module driver related clk enable */
    tmp = act_readl(CMU_DEVCLKEN);
    ms_dev.old_clk_state = tmp;
    tmp |= MS_NEED_CLK; /* enable need clk. */
    act_writel(tmp, CMU_DEVCLKEN);

#pragma __PRQA_IGNORE_START__
    if (I2C_CHAN_NUM == 0)  /*使用I2C1*/
    {
        tmp = act_readl(GPIO_AOUTEN);
        tmp |= (0x3 << 6);
        act_writel(tmp, GPIO_AOUTEN);
    
        tmp = act_readl(GPIO_ADAT);
        tmp |= (0x3 << 6);
        act_writel(tmp, GPIO_ADAT);
    
        tmp = act_readl(GPIO_AOUTEN);
        tmp &= (~(0x3 << 6));
        act_writel(tmp, GPIO_AOUTEN);
        tmp = act_readl(GPIO_AINEN);
        tmp &= (~(0x3 << 6));
        act_writel(tmp, GPIO_AINEN);
    }
    else                    /*使用I2C2*/
    {
        tmp = act_readl(GPIO_BOUTEN);
        tmp |= (0x3 << 14);
        act_writel(tmp, GPIO_BOUTEN);
    
        tmp = act_readl(GPIO_BDAT);
        tmp |= (0x3 << 14);
        act_writel(tmp, GPIO_BDAT);
    
        tmp = act_readl(GPIO_BOUTEN);
        tmp &= (~(0x3 << 14));
        act_writel(tmp, GPIO_BOUTEN);
        tmp = act_readl(GPIO_BINEN);
        tmp &= (~(0x3 << 14));
        act_writel(tmp, GPIO_BINEN);
    }
#pragma __PRQA_IGNORE_END__    

//    /* GPIO Multiple Functions; */
//    tmp = act_readl(GPIO_MFCTL1);
//    tmp |= 0x80000000;
//    act_writel(tmp, GPIO_MFCTL1);

#if 0
    /* GPIO_B0=CS, GPIO_B1=SDO */
    tmp = act_readl(GPIO_BOUTEN);
    tmp |= 0x00000003;
    act_writel(tmp, GPIO_BOUTEN);

    tmp = act_readl(GPIO_BDAT);
    tmp &= ~(0x00000003);
    tmp |= 0x00000001;
    act_writel(tmp, GPIO_BDAT);
#endif

    return 0;
}

/* initialize gsensor ic. */
int _gsensor_ic_init(void)
{
    void *pI2c;
    //unsigned int pVal[5];
    // int i;
    int ret = 0;

    ms_dev.pMs_i2c = i2c_open(0, 0, 0x2b, 0x2a);
    pI2c = ms_dev.pMs_i2c;

    ret = i2c_write_reg(pI2c, 0x0, 0x0);
    if (ret != 0)
    {
        return ret;
    }

#if 0
    ms_dev.pMs_i2c_data = (unsigned int*)ms_ic_init_data;
    ms_dev.ms_init_cnt = sizeof(ms_ic_init_data) / sizeof(int);

    ms_dev.pMs_i2c = i2c_open(0, I2C_CHAN_NUM, 0x39, 0x38);

    pI2c = ms_dev.pMs_i2c;
    udelay(10);

    pVal = ms_dev.pMs_i2c_data;
    for (i = 0; i < ms_dev.ms_init_cnt; i += 2)
    {
        if (ret = i2c_write(pI2c, pVal, 2))
        {
            return ret;
        }
        pVal++;
        pVal++;
    }
    udelay(100);
#endif

    return 0;
}

/* exit gsensor ic, maybe set it into standby. */
//static int _gsensor_module_exit(void)
//{
//    i2c_close(ms_dev.pMs_i2c);
//
//    return 0;
//}

/* 1g = 1000 */
int read_gsensor_data(gsensor_data_t *p_data)
{
    unsigned int *buf = ms_dev.buf;
    int ret;
    ret = i2c_read_regs(ms_dev.pMs_i2c, 0x1, buf, 4);

    ms_dev.ms_data.acc_x = (int) (((ms_dev.buf[0] & 0xff) << 8) | (ms_dev.buf[1] & 0xff));
    ms_dev.ms_data.acc_x -= 2048;
    ms_dev.ms_data.acc_y = (int) (((ms_dev.buf[2] & 0xff) << 8) | (ms_dev.buf[3] & 0xff));
    ms_dev.ms_data.acc_y -= 2048;
    ms_dev.ms_data.acc_z = 0;
    /* change the unit of acc_x acc_y acc_z to mg*/
    p_data->acc_x = ms_dev.ms_data.acc_x * 2;
    p_data->acc_y = ms_dev.ms_data.acc_y * 2;
    p_data->acc_z = ms_dev.ms_data.acc_z * 2;

    return ret;
}

/******************************************************************************
 * Function:       gsensor_hw_init
 * par  Description:
 *       gsensor related hardware's initialization.
 * param[in]    null
 * retval       0 sucess, else failed.
 *******************************************************************************/
int gsensor_hw_init(void)
{

    int c;
    int ret = 0;
    ret = _gsensor_ctl_init();
    if (ret != 0)
    {
        goto exit;
    }
    for (c = 0; c < 4; c++)
    {
        ret = _gsensor_ic_init(); 
        if (ret == 0)
        {
            break;
        }
    }
    
    if (ret != 0)
    {
        goto exit;
    }

    exit: return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *       release gsensor related hardware.
 * \param[in]    null
 * \retval       0 sucess, else failed.
 *******************************************************************************/
int gsensor_hw_exit(void)
{
    unsigned int tmp;

    i2c_close(ms_dev.pMs_i2c);

    /* Clock disable */
    tmp = act_readl(CMU_DEVCLKEN) & (~(MS_NEED_CLK));
    tmp |= ms_dev.old_clk_state & MS_NEED_CLK;
    act_writel(tmp, CMU_DEVCLKEN);

    return 0;
}

