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
 * \file   fm_hw.c
 * \brief
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       In This File
 *       1)using i2c bus to control FM IC.
 *       2)all the code need to rewrite while using different FM IC
 * \par EXTERNALIZED FUNCTIONS:
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2008/7/3
 *******************************************************************************/
#include "fm_hw.h"
#include "fm_config.h"
#include "fm_sys.h"
#include "i2c.h"
#include "radio.h"
#include "fm_inner.h"
#include "ucos/delay.h"
#include "gpio_config.h"


/*!
 *    i2c 设备指针 \n
 */
void * pi2c_dev = NULL;


/*! the following code are used to commnicate with FM IC
 *   what the globale variable really meaning? pls. reference the FM IC DateSheet
 *   different FM IC Has different I2C addrs and different communication mode
 */
/************ Write mode ***********/
int WSET_MUTE = 0; //Bit 7 of first byte of write buffer
int WSET_SM = 0; //Bit 6 of first byte of write buffer
int WSET_PLLHB = 0; //Bit 5 to 0 of first byte of write buffer
int WSET_PLLLB = 0; //Bit 7 to 0 of second byte of write buffer
int WSET_SUD = 0; //Bit 7 of third byte of write buffer(Search up/down)
int WSET_SSL = 0; //Bit 6 to 5 of third byte of write buffer(Search Stop Level)
int WSET_HLSI = 0; //Bit 4 of the third byte of write buffer(HIGH/LOW Side Injection)
int WSET_MS = 0; //Bit 3 of the third byte of write buffer(Mono/Stereo)
int WSET_MR = 0; //Bit 1 of the third byte of write buffer(Mute Right)
int WSET_ML = 0; //Bit 2 of the third byte of write buffer(Mute Left)
int WSET_SWP1 = 0; //Bit 0 of the third byte of write buffer(Software programmable port1)
int WSET_SWP2 = 0; //Bit 7 of the fourth byte of write buffer(Software programmable port2)
int WSET_STBY = 0; //Bit 6 of the fourth byte of write buffer(Standby)
int WSET_BL = 0; //Bit 5 of the fourth byte of write buffer(Band Limits)
int WSET_XTAL = 1; //Bit 4 of the fourth byte of write buffer
int WSET_SMUTE = 0; //Bit 3 of the fourth byte of write buffer(Soft Mute)
int WSET_HCC = 1; //Bit 2 of the fourth byte of write buffer(High Cut Control)
int WSET_SNC = 1; //Bit 1 of the fourth byte of write buffer(Stereo noise cancelling)
int WSET_SI = 0; //Bit 0 of the fourth byte of write buffer(Search indicator)
int WSET_PLLREF = 0; //Bit 7 of the fifth byte of write buffer
int WSET_DTC = 0; //Bit 6 of the fifth byte of write buffer.Bit 5 to 0 is not used,position don't care

/************Read mode************/
int RSET_RF = 0; //Bit 7 of the first byte of read buffer(Ready Flag)
int RSET_BLF = 0; //Bit 6 of the first byte of read buffer(Band Limit Flag)
int RSET_PLLHB = 0; //Bit 5 to 0 of first byte of write buffer
int RSET_PLLLB = 0; //Bit 7 to 0 of second byte of write buffer
int RSET_STEREO = 0; //Bit 7 of the third byte of read buffer(Stereo indication)
int RSET_IFCOUNTER = 0; //Bit 6 to 0 of the third byte of read buffer ，IF Counter result
int RSET_LEV = 0; //Bit 7 to 4 of the fourth byte of read buffer，Level ADC output
int FREQ_RF = 0;

/******************************Local Variable********************************/
int SOFT_LEV;
int SOFT_SUD;
int tempresult_LEV;
int Searchtemp_LEV;
int searchtemp_if;
int searchtemp_stereo;
int VOLUMELEV;
int band_mode;

int READ_Status(void);

int Write_Buffer[5] =
{ 0x00000080, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

int Read_Buffer[5] =
{ 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 };

/******************************************************************************/
/*!
 * \par  Description:
 *     Function is to read one frame.
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
int READ_ONEFRAME(void)
{
    int flag;
    flag = i2c_read(pi2c_dev, 0xc1, Read_Buffer, 5);
    return (flag);
}

int check_linein(unsigned int *state)
{
    unsigned int tmp; 
    
    tmp = act_readl(LINEIN_GPIO_OUTEN);
    tmp &= (~(0x1<<LINEIN_GPIO_NUM));
    act_writel(tmp, LINEIN_GPIO_OUTEN);
    // printk("*********GPIO_BOUTEN:%x\n*******",act_readl(GPIO_BOUTEN));
    
    tmp = act_readl(LINEIN_GPIO_INEN);
    tmp |= (0x1<<LINEIN_GPIO_NUM);
    act_writel(tmp, LINEIN_GPIO_INEN);
    // printk("*********GPIO_BINEN:%x\n*******",act_readl(GPIO_BINEN));
    
    mdelay(1);
    tmp = act_readl(LINEIN_GPIO_DATA);
    // printk("*********GPIO_BDAT:%x\n*******",tmp);
   
    tmp = (tmp>>LINEIN_GPIO_NUM) & (0x1);
    
    if (tmp == 0)
    {
        *state = LINEIN_IN;
    }
    else 
    {
        *state = LINEIN_OUT;
    }
    
    return 0;
     
}


/******************************************************************************/
/*!
 * \par  Description:
 *     Function is to write one frame.
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
int WRITE_ONEFRAME(void)
{
    //    printk("********* WRITE_ONEFRAME********\n");
    int flag;
    flag = i2c_write(pi2c_dev, 0xc0, Write_Buffer, 5);
    return (flag);
}
/********************************************************************************************
 Function is to assemble write buffer.
 Input parameter:         no
 Output parameter:        no
 ********************************************************************************************/
void ASSEMBLE_WB(void)
{
    int temp = 0;
    temp = WSET_MUTE;
    if (temp != 0)
    {
        WSET_PLLHB = WSET_PLLHB | 0x80;
    }
    if (WSET_SM != 0)
    {
        WSET_PLLHB = WSET_PLLHB | 0x40;
    }

    Write_Buffer[0] = WSET_PLLHB;
    Write_Buffer[1] = WSET_PLLLB;
    temp = (WSET_SUD & 0x01) << 7;
    temp = temp | ((WSET_SSL & 0x03) << 5);
    temp = temp | ((WSET_HLSI & 0x01) << 4);
    temp = temp | ((WSET_MS & 0x01) << 3);
    temp = temp | ((WSET_MR & 0x01) << 2);
    temp = temp | ((WSET_ML & 0x01) << 1);
    temp = temp | WSET_SWP1;
    Write_Buffer[2] = temp;
    //    printk("***********WSET_MS is %d************\n",WSET_MS);
    temp = (WSET_SWP2 & 0x01) << 7;
    temp = temp | ((WSET_STBY & 0x01) << 6);
    temp = temp | ((WSET_BL & 0x01) << 5);
    temp = temp | ((WSET_XTAL & 0x01) << 4);
    temp = temp | ((WSET_SMUTE & 0x01) << 3);
    temp = temp | ((WSET_HCC & 0x01) << 2);
    temp = temp | ((WSET_SNC & 0x01) << 1);
    temp = temp | WSET_SI;
    Write_Buffer[3] = temp;
    temp = (WSET_PLLREF & 0x01) << 7;
    temp = temp | ((WSET_DTC & 0x01) << 6);
    Write_Buffer[4] = temp;
    //    printk("*********************write buffer 3 is %x\n", Write_Buffer[2]);
    return;
}

/********************************************************************************************
 Function is to disassemble read buffer.
 Input parameter:         no
 Output parameter:        no
 ********************************************************************************************/
void DISASSEMBLE_RB(void)
{
    int temp;

    READ_Status();
    //    printk("*******  read_buffer 0 is %X\n",Read_Buffer[0] );
    //    printk("*******  read_buffer 1 is %X\n",Read_Buffer[1] );
    //    printk("*******  read_buffer 2 is %X\n",Read_Buffer[2] );
    //    printk("*******  read_buffer 3 is %X\n",Read_Buffer[3] );

    temp = Read_Buffer[0];
    RSET_RF = ((temp & 0x80) >> 7) & 0x1;
    RSET_BLF = ((temp & 0x40) >> 6) & 0x1;
    //    printk("*******  RSET_BLF is %X\n***********",RSET_BLF );
    temp = Read_Buffer[2];
    RSET_STEREO = ((temp & 0x80) >> 7) & 0x1;
    //    printk("*******  RSET_STEREO is %X\n***********",RSET_STEREO );
    RSET_IFCOUNTER = (temp & 0x7f);
    temp = Read_Buffer[3];
    //    printk("*******  read_buffer 3 is %X\n",Read_Buffer[3] );
    RSET_LEV = ((temp & 0xf0) >> 4) & 0xf;
    // printk("*******  level adc is %d\n",RSET_LEV );

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
int fm_hw_init(int init_band_mode)
{
    WSET_MUTE = 0;
    WSET_SM = 0;
    WSET_PLLHB = 0;
    WSET_PLLLB = 0;
    WSET_SUD = 0;
    WSET_SSL = 0;
    WSET_HLSI = 0;
    WSET_MS = 0;
    WSET_MR = 0;
    WSET_ML = 0;
    WSET_SWP1 = 0;
    WSET_SWP2 = 0;
    WSET_STBY = 0;
    WSET_BL = 0;
    WSET_XTAL = 1;
    WSET_SMUTE = 0;
    WSET_HCC = 1;
    WSET_SNC = 1;
    WSET_SI = 0;
    WSET_PLLREF = 0;
    WSET_DTC = 0;
    RSET_RF = 0;
    RSET_BLF = 0;
    RSET_PLLHB = 0;
    RSET_PLLLB = 0;
    RSET_STEREO = 0;
    RSET_IFCOUNTER = 0;
    RSET_LEV = 0;
    FREQ_RF = 0;
    WSET_SM = 0; //no search
    WSET_STBY = 0; //no standby
    WSET_XTAL = 1; //XTAL f=32.768kHz
    WSET_BL = init_band_mode;
    SOFT_LEV = 5;

    pi2c_dev = i2c_open(NORMAL_SPEED_I2C, I2C_ChAN_NUM);

    //INIT_ADDA();
    //i2c_get_gpio();
    //I2C_Init();
    //fm_enter_standby(0);
    //READ_Status();
    //i2c_release_gpio();

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   int stanb: 0 not standby; 1 standby
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      other failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int fm_enter_standby(int stanb)
{
    int flag = -1;
    static int c;
    Write_Buffer[3] = Write_Buffer[3] & 0xbf; //no standby
    WSET_STBY = stanb;
    if (WSET_STBY != 0)
    {
        Write_Buffer[3] = Write_Buffer[3] | 0x40; //standby
    }
    
    c = 3;
    do
    {
        c = c - 1;
        if (c == 0)
        {
            break;
        }
        flag = WRITE_ONEFRAME();
    } while (flag != 0);
    
    return (flag);
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
int fm_hw_exit(void)
{
    DEBUG_PRINT("fm exit  \n");
    fm_enter_standby(1); //enter standby
    i2c_close(pi2c_dev);
    //I2C_Exit();
    DEBUG_PRINT("fm exit out\n");
    //ada_set_mute(0);
    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     set mute of fm ic
 * \param[in]   int mute_type ,1 mute ;0 not mute
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int fm_set_mute(int mute)
{
    int flag = 0;
    int m;
    Write_Buffer[0] = Write_Buffer[0] & 0x7f; //not mute
    WSET_MUTE = mute;
    if (WSET_MUTE != 0)
    {
        Write_Buffer[0] = Write_Buffer[0] | 0x80; //mute
    }

    for (m = 3; m > 0; m--)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        {

            break;
            //            if (mute == 0)
            //            {
            //                   // setSrc_mute(0x4,1);
            //               // setMute(0x4);
            //            }
            //            else
            //            {
            //                //setSrc_mute(0x4,0);
            //               // setMute(0x0);
            //            }
        }
    }
    return flag;
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

int fm_set_band_mode(int band)
{
    int flag = -1;
    int c;
    if (band == JAPAN)
    {
        band_mode = JAPAN;
        WSET_BL = 1;
        Write_Buffer[3] = Write_Buffer[3] | 0x20; //JAP FM band

    }
    else
    {
        if (band == EURO)
        {
            band_mode = EURO;
            WSET_BL = 0;
            Write_Buffer[3] = Write_Buffer[3] & 0xdf; //US.EURO FM band
        }
        else
        {
            band_mode = US;
            WSET_BL = 0;
            Write_Buffer[3] = Write_Buffer[3] & 0xdf; //US.EURO FM band
        }
    }
    for (c = 4; c > 0; c--)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        {
            break;
        }
    }
    return (flag);
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
int fm_get_intensity(int* pIntensity)
{
    int rssi = 0;

    DISASSEMBLE_RB();
    rssi = RSET_LEV;
    *((int *) pIntensity) = rssi;

    // printf("rssi %d \n",rssi);
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
int fm_get_stereo_mode(int* pMode)
{
    int track = 0;
    DISASSEMBLE_RB();
    track = RSET_STEREO;
    *((int *) pMode) = track;
    //DEBUG_PRINT("sound mode =%d\n",track);
    return (0);
}

/********************************************************************************************
 Function is to wait 50ms.
 Input parameter:         no
 Output parameter:        no
 ********************************************************************************************/
void FMwait50ms(void)
{
#ifdef __DSP_RAM_FLAG__
    {
        volatile int i;
        // 在32.768KHz频率下运行时,要延时50ms只需空等0.05 / ( 1 / 32768)  条指令即可
        for(i = 0; i <= 1638; i++)
        {
            __asm__ __volatile__ ("nop");
        }
    }
#else
    {
        volatile int i, j, c;
        int wait_times;
        wait_times = 6000;
        for (i = 0; i < wait_times; i++)
        {
            for (j = 0; j < 120; j++)
            {
                c = 0;
            }
        }
    }
#endif

    return;
}
/********************************************************************************************
 Function is to calculate pll.
 Input parameter:
 freq(wanted frequency)
 Output parameter:
 flag(1:success  0:failure)
 ********************************************************************************************/
void CALCULATE_Pll(int freq)
{
    int PLL;
    if (WSET_HLSI == 0)
    {
        PLL = (125 * (freq - 225)) / 1024;
    }
    else
    {
        PLL = (125 * (freq + 225)) / 1024;
    }
    WSET_PLLLB = PLL & 0xff;
    WSET_PLLHB = (PLL & 0xff00) >> 8;

    return;
}

/********************************************************************************************
 Function is to read status.
 Input parameter:         no
 Output parameter:
 flag(0:success  -1:failure)
 ********************************************************************************************/
int READ_Status(void)
{
    int flag = -1;
    int c;

    //FMwait50ms();
    mdelay(35);
    for (c = 4; c > 0; c--)
    {
        flag = READ_ONEFRAME();
        if (flag == 0)
        {
            break;
        }
    }

    if (flag == -1)
    {
        printk("************* I2C read error!***********\n");
    }
    return (flag);
}

/********************************************************************************************
 Function is to check FM station.
 Input parameter:         no
 Output parameter:
 flag(0:success  -1:failure)
 ********************************************************************************************/
int CHECK_Station(void)
{
    int flag = -1;
    int c;

    c = 3;

    loop1: c = c - 1;
    if (c == 0)
    {
        goto FMerrstop;
    }
    flag = WRITE_ONEFRAME();
    if (flag == -1)
    {
        goto loop1;
    }
    flag = READ_Status(); //judge if any station has been found
    if (flag == -1)
    {
        goto loop1;
    }
    DISASSEMBLE_RB();
    searchtemp_if = RSET_IFCOUNTER;
    searchtemp_stereo = RSET_STEREO;
    Searchtemp_LEV = RSET_LEV;
    //  if (Searchtemp_LEV < temp)
    //      goto FMerrstop;
    if (searchtemp_if >= IFHIGHLIMIT)
    {
        goto FMerrstop;
    }
    if (searchtemp_if <= IFLOWLIMIT)
    {
        goto FMerrstop;
    }
    //  if (searchtemp_stereo == 0)
    //      goto FMerrstop;
    tempresult_LEV = Searchtemp_LEV;
    flag = 0;
    goto exit;

    FMerrstop: flag = -1;
    exit: return (flag);
}

/********************************************************************************************
 Function is to test FM station.
 Input parameter:         no
 Output parameter:
 flag(1:success  0:failure)
 ********************************************************************************************/
int Teststation(void)
{
    int flag;
    static int c;

    for (c = 3; c > 0; c--)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        { 
            DISASSEMBLE_RB();
            Searchtemp_LEV = RSET_LEV;
            break;
            
        }
    }
    return (flag);
}


int fm_set_freq(int freq)
{
    int flag, FM_result, FMadclevel1, FMadclevel2;
    int temp;
    int i;
    
    
    
    int band_limit_low;
    int band_limit_high;
    
    if (band_mode == JAPAN)
    {
        band_limit_low = 76000;
        band_limit_high = 90000;
        //printk("JAPAN band mode\n");
    }
    else
    {
        band_limit_low = 87500;
        band_limit_high = 108000;
        //printk("US band mode\n");
    }

    if ((freq < band_limit_low) || (freq > band_limit_high))
    {
        return -EINVAL;
    }
    
    
    

    //  WSET_MUTE=1;
//    WSET_MUTE = 0;
    WSET_HLSI = 1;
    WSET_SM = 0;
    CALCULATE_Pll(freq);
    ASSEMBLE_WB();

    temp = WSET_PLLHB;
    if (WSET_PLLLB > 0xc8)
    {
        temp = temp + 1;
    }
    Write_Buffer[0] = (temp | 0x80);
    Write_Buffer[1] = ((WSET_PLLLB + 0x37) & 0xff);
    FM_result = Teststation();

    if (FM_result == 0)
    {
        FMadclevel1 = Searchtemp_LEV;
        temp = WSET_PLLHB;
        if (WSET_PLLLB < 0x37)
        {
            temp = temp - 1;
        }
        Write_Buffer[0] = (temp | 0x80);
        Write_Buffer[1] = (((WSET_PLLLB + 0xff) - 0x37) & 0xff);
        FM_result = Teststation();

        if (FM_result == 0)
        {
            FMadclevel2 = Searchtemp_LEV;

            if (FMadclevel1 >= FMadclevel2)
            {
                WSET_HLSI = 0x00;
                CALCULATE_Pll(freq);
            }
        }
        else
        {
            return -EIO;
        }
    }
    else
    {
        return -EIO;
    }

    ASSEMBLE_WB();
    flag = -1;
    for (i = 0; i < 3; i++)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        {
            break;
        }
    }
    if (flag == -1)
    {
        return -EIO;
    }
    
    //mdelay(50);
    DISASSEMBLE_RB();
    if ((RSET_RF == 1) && (RSET_IFCOUNTER > 0x31) && (RSET_IFCOUNTER < 0x3e) && (RSET_LEV == 10) && (RSET_STEREO == 1))
    {
        return 0;
    }
    else
    {
        return -1;
    }   
}




int fm_set_freq_no_check(int freq)
{   
    int flag;
    int i;
  
    int band_limit_low;
    int band_limit_high;
    
    if (band_mode == JAPAN)
    {
        band_limit_low = 76000;
        band_limit_high = 90000;
        //printk("JAPAN band mode\n");
    }
    else
    {
        band_limit_low = 87500;
        band_limit_high = 108000;
        //printk("US band mode\n");
    }

    if ((freq < band_limit_low) || (freq > band_limit_high))
    {
        return -EINVAL;
    }
    
    
    //  WSET_MUTE=1;
//    WSET_MUTE = 0;
    WSET_HLSI = 1;
    WSET_SM = 0;
    CALCULATE_Pll(freq);
    ASSEMBLE_WB();

    
    for (i = 0; i < 3; i++)
    {

        

        flag = WRITE_ONEFRAME();
       
        
        if (flag == 0)
        {
            break;
        }
    }
    if (flag == -1)
    {
        return -EIO;
    }
    return 0;
    
}






/******************************************************************************/
/*!
 * \par  Description:
 *     description the function  in here
 * \param[in]   int band_mode, 0: US EURO Mode ; 1: Japan Mode
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      other failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int fm_init(int band_mode_init)
{
    fm_hw_init(band_mode_init);
    return 0;
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
int fm_exit(void)
{
    fm_hw_exit();
    return 0;
}
#pragma __PRQA_IGNORE_START__
/******************************************************************************/
/*!
 * \par  Description:
 *     fm_seek_stations
 * \param[in]   seek_info
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      <0 fail
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
 
 
 
 
 
 
 






int fm_seek_stations(struct radio_seek_info_t * seek_info)
{
    int flag = -1;
    int band_limit_low;
    int band_limit_high;
    int freq;
    int c;
    

    freq = seek_info->search_freq;
    SOFT_SUD = (int)seek_info->search_direction;
    WSET_SUD = SOFT_SUD;
    WSET_SM = 1;

    if (WSET_SUD == 1)
    {
        WSET_HLSI = 0;
    }
    else
    {
        WSET_HLSI = 1;
    }

    if (threshold <= 7)
    {
        WSET_SSL = 0x01;
    }
    if ((threshold > 7) && (threshold < 10))
    {
        WSET_SSL = 0x02;
    }
    if (threshold >= 10)
    {
        WSET_SSL = 0x03;
    }

    if (band_mode == JAPAN)
    {
        band_limit_low = 76000;
        band_limit_high = 90000;
        //printk("JAPAN band mode\n");
    }
    else
    {
        band_limit_low = 87500;
        band_limit_high = 108000;
        //printk("US band mode\n");
    }

    if ((freq < band_limit_low) || (freq > band_limit_high))
    {
        return -EINVAL;
    }

    CALCULATE_Pll(freq);
    ASSEMBLE_WB();

    for (c = 0; c < 3; c++)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        {
            return 0;
        }
    }
    if (flag == -1)
    {
        printk("I2C write error!\n");
        return -EIO;
    }
    
    
}





int check_seek_status(struct radio_info_t * radio_info_check)
{
    int error = 0;
    int flag = -1;
    int freq_found;
    int freq_print;
    int band_limit_low;
    int band_limit_high;

    int value_pll = 0;
    int value_pllh = 0;
    int value_pllb = 0;
    int level_1 = 0;
    int level_2 = 0;
    int diff;
    int c;
    
     if (band_mode == JAPAN)
    {
        band_limit_low = 76000;
        band_limit_high = 90000;
        //printk("JAPAN band mode\n");
    }
    else
    {
        band_limit_low = 87500;
        band_limit_high = 108000;
        //printk("US band mode\n");
    }
    
    DISASSEMBLE_RB();
    value_pllh = Read_Buffer[0] & 0x3f;
    value_pllb = Read_Buffer[1];
    value_pll = (value_pllh << 8) | value_pllb;
    if (WSET_HLSI == 0)
    {
        freq_found = (((32768 * value_pll) / 4) + 225000) / 1000;
    } //opposition HLSI, test
    else
    {
        freq_found = (((32768 * value_pll) / 4) - 225000) / 1000;
    }

    freq_print = freq_found + 50;
    freq_print = freq_print / 100;
    freq_print = freq_print * 100;
    
    radio_info_check->current_freq = freq_print;
    radio_info_check->intensity = RSET_LEV;
    radio_info_check->stereo_status = RSET_STEREO; 
    if (freq_print < band_limit_low)
    {
        radio_info_check->current_freq = band_limit_low;
        radio_info_check->seek_status = REACH_BANDLIMIT;
        return 0;
    }
    if (freq_print > band_limit_high)
    {
        radio_info_check->current_freq = band_limit_high;
        radio_info_check->seek_status = REACH_BANDLIMIT;
        return 0;
    }
    
    if (RSET_BLF == 1)
    {
        //printk("reach band limit!\n");
        radio_info_check->seek_status = REACH_BANDLIMIT;
        return 0;
    }
    else
    {
    
        if (RSET_RF == 1)
        {
            //printk("RSET_RF is 1\n");
            level_1 = RSET_LEV;
            if (WSET_HLSI == 0)
            {
//                freq_found = (((32768 * value_pll) / 4) + 225000) / 1000;
                WSET_HLSI = 1;
            } //opposition HLSI, test
            else
            {
//                freq_found = (((32768 * value_pll) / 4) - 225000) / 1000;
                WSET_HLSI = 0;
            }
            if ((RSET_IFCOUNTER > 0x31) && (RSET_IFCOUNTER < 0x3e))
            {
                CALCULATE_Pll(freq_found);
                WSET_SM = 0;
//                WSET_MUTE = 0;
                ASSEMBLE_WB();

                for (c = 0; c < 3; c++)
                {
                    flag = WRITE_ONEFRAME();
                    if (flag == 0)
                    {
                        break;
                    }
                }
                if (flag == -1)
                {
                    //printk("I2C write error!\n");
                    return -EIO;
                }

                for (c = 3; c > 0; c--)
                {
                    DISASSEMBLE_RB();
                    if (RSET_RF == 1)
                    {
                        break;
                    }
                }

                if ((RSET_RF == 1) && (RSET_IFCOUNTER > 0x31) && (RSET_IFCOUNTER < 0x3e) && (RSET_STEREO == 1))
                {
                    level_2 = RSET_LEV;
                    diff = level_1 - level_2;
                    if ((diff <= 2) && (diff >= -2))
                    {
                          radio_info_check->seek_status = FOUND_STATION;
                          return 0;
                    }
                    else
                    {
                        error = 1;
//                        DISASSEMBLE_RB();
                       // printk("*** diff wrong.\n");
                    }
                }
                else
                {
                    error = 1;
                   // printk("RF and intermediate frequency wrong.\n");
                }

            }
            else
            {
                error = 1;
                //printk("***intermediate frequency wrong.\n");
            }
            if (error == 1)
            {
                radio_info_check->seek_status = INVALID_STATION;
                return 0;   
            }

        }
        else
        {
            radio_info_check->seek_status = FOUND_NOSTAION;
            return 0;
           // printk("station has not found yeat.\n");
        }
      }  
    
}







void fm_get_freq(struct radio_info_t * radio_info_getfreq)
{
    int freq_found;
    int freq_print;
    int band_limit_low;
    int band_limit_high;

    int value_pll = 0;
    int value_pllh = 0;
    int value_pllb = 0;
    if (band_mode == JAPAN)
    {
        band_limit_low = 76000;
        band_limit_high = 90000;
        //printk("JAPAN band mode\n");
    }
    else
    {
        band_limit_low = 87500;
        band_limit_high = 108000;
        //printk("US band mode\n");
    }

    DISASSEMBLE_RB();
    value_pllh = Read_Buffer[0] & 0x3f;
    value_pllb = Read_Buffer[1];
    
    
    
    
    
    value_pll = (value_pllh << 8) | value_pllb;
    if (WSET_HLSI == 0)
    {
        freq_found = (((32768 * value_pll) / 4) + 225000) / 1000;
    } //opposition HLSI, test
    else
    {
        freq_found = (((32768 * value_pll) / 4) - 225000) / 1000;
    }

    if (radio_info.band_mode == EURO)
    {
        freq_print = freq_found + 10;
        freq_print = freq_print / 50;
        freq_print = freq_print * 50;
    }
    else
    {
        freq_print = freq_found + 50;
        freq_print = freq_print / 100;
        freq_print = freq_print * 100;
    }
    
    radio_info_getfreq->current_freq = freq_print;      
    if (freq_print < band_limit_low)
    {
        radio_info_getfreq->current_freq = band_limit_low;
    }
    if (freq_print > band_limit_high)
    {
        radio_info_getfreq->current_freq = band_limit_high;
    }
    
}






 
 
 
 
 
 
 
 
 
 
 
 
 
//int fm_seek_stations(struct radio_seek_info_t * seek_info)
//{
//    int flag = -1;
//    int error = 0;
//    int band_limit_low;
//    int band_limit_high;
//    int freq_found;
//    int freq;
//
//    int value_pll = 0;
//    int value_pllh = 0;
//    int value_pllb = 0;
//    int level_1 = 0;
//    int level_2 = 0;
//    int diff;
//    int c;
//    
//    unsigned long *  fm_giffies;
//    fm_giffies = kernel_sym("jiffies");
//    
//
//    freq = seek_info->search_freq;
//    SOFT_SUD = (int)seek_info->search_direction;
//    WSET_SUD = SOFT_SUD;
//    WSET_SM = 1;
//
//    if (WSET_SUD == 1)
//    {
//        WSET_HLSI = 0;
//    }
//    else
//    {
//        WSET_HLSI = 1;
//    }
//
//    if (threshold <= 7)
//    {
//        WSET_SSL = 0x01;
//    }
//    if ((threshold > 7) && (threshold < 10))
//    {
//        WSET_SSL = 0x02;
//    }
//    if (threshold >= 10)
//    {
//        WSET_SSL = 0x03;
//    }
//
//    if (band_mode == JAPAN)
//    {
//        band_limit_low = 76000;
//        band_limit_high = 90000;
//        printk("JAPAN band mode\n");
//    }
//    else
//    {
//        band_limit_low = 87500;
//        band_limit_high = 108000;
//        printk("US band mode\n");
//    }
//
//    if ((freq < band_limit_low) || (freq > band_limit_high))
//    {
//        return -EINVAL;
//    }
//
//    CALCULATE_Pll(freq);
//    ASSEMBLE_WB();
//
//    for (c = 0; c < 3; c++)
//    {
//        flag = WRITE_ONEFRAME();
//        if (flag == 0)
//        {
//
//            break;
//        }
//    }
//    if (flag == -1)
//    {
//        printk("I2C write error!\n");
//        return -EIO;
//    }
//    
//    printk("**********fm_giffies1 is : %d\n",*fm_giffies);
//    
//    DISASSEMBLE_RB();
//
//    while (RSET_BLF != 1)
//    {
//        if (RSET_RF == 1)
//        {
//            printk("**********fm_giffiesRF is : %d\n",*fm_giffies);
//            printk("RSET_RF is 1\n");
//            level_1 = RSET_LEV;
//            value_pllh = Read_Buffer[0] & 0x3f;
//            value_pllb = Read_Buffer[1];
//            value_pll = (value_pllh << 8) | value_pllb;
//            if (WSET_HLSI == 0)
//            {
//                freq_found = (((32768 * value_pll) / 4) + 225000) / 1000;
//                WSET_HLSI = 1;
//            } //opposition HLSI, test
//            else
//            {
//                freq_found = (((32768 * value_pll) / 4) - 225000) / 1000;
//                WSET_HLSI = 0;
//            }
//            if ((RSET_IFCOUNTER > 0x31) && (RSET_IFCOUNTER < 0x3e))
//            {
//                //                    level_1 = RSET_LEV;
//                //                    value_pllh = Read_Buffer[0] & 0x3f;
//                //                  value_pllb = Read_Buffer[1];
//                //                  value_pll = (value_pllh << 8) | value_pllb;
//
//                CALCULATE_Pll(freq_found);
//                WSET_SM = 0;
//                WSET_MUTE = 0;
//                ASSEMBLE_WB();
//
//                for (c = 0; c < 3; c++)
//                {
//                    flag = WRITE_ONEFRAME();
//                    if (flag == 0)
//                    {
//                        break;
//                    }
//                }
//                if (flag == -1)
//                {
//                    //printk("I2C write error!\n");
//                    return -EIO;
//                }
//
//                for (c = 5; c > 0; c--)
//                {
//                    DISASSEMBLE_RB();
//                    if (RSET_RF == 1)
//                    {
//                        break;
//                    }
//                }
//
//                if ((RSET_RF == 1) && (RSET_IFCOUNTER > 0x31) && (RSET_IFCOUNTER < 0x3e) && (RSET_STEREO == 1))
//                {
//                    level_2 = RSET_LEV;
//                    diff = level_1 - level_2;
//                    if ((diff <= 2) && (diff >= -2))
//                    {
//                        int freq_print;
//                        freq_print = freq_found + 50;
//                        freq_print = freq_print / 100;
//                        freq_print = freq_print * 100;
//                        radio_info.current_freq = freq_print;
//                        radio_info.intensity = RSET_LEV;
//                        
//                        printk("**********fm_giffies2 is : %d\n",*fm_giffies);
//                       // printk("**** A station is found!! \n");
//                       // printk("******** freq_found is %d \n", freq_print);
//                        return 0;
//                    }
//                    else
//                    {
//                        error = 1;
//                        DISASSEMBLE_RB();
//                       // printk("*** diff wrong.\n");
//                    }
//                }
//                else
//                {
//                    error = 1;
//                   // printk("RF and intermediate frequency wrong.\n");
//                }
//
//            }
//            else
//            {
//                error = 1;
//                //printk("***intermediate frequency wrong.\n");
//            }
//
//            if (WSET_HLSI == 0)
//            {
//                WSET_HLSI = 1;
//            } //opposition HLSI, test
//            else
//            {
//                WSET_HLSI = 0;
//            }
//            WSET_SM = 1;
//            CALCULATE_Pll(freq_found);
//            ASSEMBLE_WB();
//            for (c = 0; c < 3; c++)
//            {
//                flag = WRITE_ONEFRAME();
//                if (flag == 0)
//                {
//                    break;
//                }
//            }
//            if (flag == -1)
//            {
//               // printk("I2C write error!\n");
//                return -EIO;
//            }
//            DISASSEMBLE_RB();
//
//        }
//        else
//        {
//            DISASSEMBLE_RB();
//           // printk("station has not found yeat.\n");
//        }
//
//    }
//    printk("reach band limit!\n");
//    return -ENODATA;
//}




int fm_seek_break(unsigned int param)
{
    int flag = -1;
    int freq_found;
    int freq_print;

    int value_pll = 0;
    int value_pllh = 0;
    int value_pllb = 0;
    int c;
    
    
    DISASSEMBLE_RB();
    value_pllh = Read_Buffer[0] & 0x3f;
    value_pllb = Read_Buffer[1];
    value_pll = (value_pllh << 8) | value_pllb;
    if (WSET_HLSI == 0)
    {
        freq_found = (((32768 * value_pll) / 4) + 225000) / 1000;
    } //opposition HLSI, test
    else
    {
        freq_found = (((32768 * value_pll) / 4) - 225000) / 1000;
    }

    freq_print = freq_found + 50;
    freq_print = freq_print / 100;
    freq_print = freq_print * 100;
    radio_info.current_freq = freq_print;
    
    WSET_PLLHB = value_pllh;
    WSET_PLLLB = value_pllb;
    WSET_SM = 0;
    ASSEMBLE_WB();
    for (c = 0; c < 3; c++)
    {
        flag = WRITE_ONEFRAME();
        if (flag == 0)
        {
            break;
        }
    }
    if (flag == -1)
    {
        return -EIO;
    }
    else
    {
        return 0;
    }
        
}








#pragma __PRQA_IGNORE_END__
/******************************************************************************/
/*!
 * \par  Description:
 *     fm_ioctl, provice interface for fm_inner.c to access FM IC INFO
 * \param[in]   unsigned int cmd, commad for IOCTL
 * \param[in]   unsigned long param,
 * \param[out]  none
 * \return      the result
 * \retval      0 sucess
 * \retval      1 failed
 * \par      exmaple code
 * \code
 *           list example code in here
 * \endcode
 *******************************************************************************/
int fm_ioctl(unsigned int cmd, unsigned long param)
{
    int ret = 0;
    switch (cmd)
    {
        case GET_FMHW_STEREO_MODE:
        fm_get_stereo_mode((int *) param);
        break;

        case GET_FMHW_INTENSITY:
        fm_get_intensity((int *) param);
        break;

        case SET_FMHW_BAND_MODE:
        ret = fm_set_band_mode((int) param);
        break;

        case SET_FMHW_FREQ:
        ret = fm_set_freq((int) param);
        break;

        case SET_FMHW_MUTE:
        ret = fm_set_mute((int) param);
        break;

        case SET_FMHW_ENTER_STANDBY:
        fm_enter_standby((int) param);
        break;

        case RADIO_SET_AUTO_SEEK:
        ret = fm_seek_stations(param);
        break;
        
        case RADIO_SET_AUTO_SEEK_BREAK:
        ret = fm_seek_break(param);
        break;

        default:
        ret = -1;
        break;
    }
    return ret;
}

