/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        2009-6-5 9:22   1.0             build this file
 *******************************************************************************/
/*!
 * \file     camera_dsc_control_pcm_data.c
 * \brief    概述
 * \author
 * \par      GENERAL DESCRIPTION:
 *               拍照声从wav转换成pcm到buf
 * \par      EXTERNALIZED FUNCTIONS:
 *               用到外面的模块
 * \version 1.0
 * \date  2009-6-5
 *******************************************************************************/
#include "camera_dsc_control_pcm_data.h"
#include "camera_dsc_config.h"
#include "camera_define.h"

static img_taker_t s_taker;
static bool _get_pcm_data(const char *file_name, pcm_data_t *pcm_data);

/*!
 * Get PCM data.
 *
 * \par Description:
 *	   This function explain the input .wav file, get the formate and the pure PCM data of it.
 *
 * \param[in]   file_name : name of the input .wav file, including full path.
 * \param[out]  pcm_data : the formate and the pure PCM data.
 * \return      sucess or failed
 * \retval      0 successs
 * \retval      -1 failure.
 * \ingroup     pcm_data
 *
 **/
static bool _get_pcm_data(const char *file_name, pcm_data_t *pcm_data)
{
    FILE* fp = NULL;
    wav_riff_type_chunk_t riff;
    wav_format_chunk_t wave;
    int j = 0;

    unsigned char *src_tmp = NULL;
    unsigned char *dst_tmp = NULL;
    unsigned int *dst_int = NULL;
    unsigned char *src_char = NULL;

    int data_found_flag = 0;
    int format_found_flag = 0;

    int max_trunc = 0;
    char chunck_id[4];
    unsigned int len = 0;

    fp = fopen(file_name, "r");
    if (fp == 0)
    {
        print_err("error: wav file open error!\n");
        return false;
    }

    fread(&riff, sizeof(riff), 1, fp);
    
    /*! 下面是搜索fmt，data字段获取相关信息后再取len字节音频数据 */
    while ((!data_found_flag) || (!format_found_flag))
    {
        max_trunc++;
        if (max_trunc > 31)
        {
            fclose(fp);
            if (src_tmp != NULL)
            {
                free(src_tmp);
            }
            return false;
        }

        fread(chunck_id, sizeof(chunck_id), 1, fp);
        fread(&len, sizeof(len), 1, fp);
        /*! klocwork检查需要防止外面获取进来的值过大爆掉 */
        assert(len<0x3fffffff);
    
        if (!memcmp(chunck_id, "fmt ", 4))
        //formate trunck
        {
            format_found_flag = 1;
            fseek(fp, -8, SEEK_CUR);
            fread(&wave, sizeof(wav_format_chunk_t), 1, fp);
            pcm_data->channels = (int) wave.channels;
            pcm_data->bit_per_sample = (int) wave.bits_per_sample;
            switch (wave.samples_per_sec)
            {
                case 8000:
                pcm_data->sample_rate = ACT_SAMPLE_8K;
                break;
                case 11025:
                pcm_data->sample_rate = ACT_SAMPLE_11K;
                break;
                case 22050:
                pcm_data->sample_rate = ACT_SAMPLE_22K;
                break;
                case 16000:
                pcm_data->sample_rate = ACT_SAMPLE_16K;
                break;
                default:
                fclose(fp);
                if (src_tmp != NULL)
                {
                    free(src_tmp);
                }                
                return false;
            }
        }
        else if (!memcmp(chunck_id, "data", 4))
        //data trunck
        {
            data_found_flag = 1;
            if(src_tmp != NULL)
            {
                free(src_tmp);
            }
            src_tmp = (char*) malloc(len);
            if (!src_tmp)
            {
                print_err("not enough memory to load 8bit PCM data!\n");
                fclose(fp);
                return false;
            }

            fread(src_tmp, len, 1, fp);
        }
        else
        {
            fseek(fp, (int) len, SEEK_CUR);
        }
    }

    fclose(fp);

    dst_tmp = (char*) malloc(4 * len);
    if (!dst_tmp)
    {
        print_err("not enough memory to load 32bit PCM data!\n");
        free(src_tmp);
        return false;
    }

    memset(dst_tmp, 0, 4 * len);

    dst_int = (unsigned int*) dst_tmp;
    src_char = (unsigned char*) src_tmp;

    while (j < (int) len)
    {
        j++;
        *dst_int = (unsigned int) ((*src_char) - 0x80) << 24;
        src_char++;
        dst_int++;
    }

    free(src_tmp);
    /*! 这里malloc的空间要在外面消亡是否有些不妥,中间件很多是这样做的，
     注意要封装好成对出现，以避免内存泄漏 */
    pcm_data->data = dst_tmp;
    pcm_data->data_len = (int) len * 4;
    pcm_data->has_inited = 1;

    print_dbg("pcm data address:0x%x\n", pcm_data->data);
    print_dbg("pcm data len:0x%x\n", pcm_data->data_len);
    print_dbg("sample rate:%d, precision:%d channel:%d\n", pcm_data->sample_rate,
            pcm_data->bit_per_sample, pcm_data->channels);
    if ((pcm_data->channels != 2) && (pcm_data->channels != 1))
    {
        print_err();
        return false;
    }
    return true;
}

bool shutter_sound_play(void)
{
    int fd;

    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value == 0xffff)
    {
        return true;
    }
    if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value <= 0)
    {
        return false;
    }
    fd = sys_open("/dev/ada", O_RDWR, 0);
    sys_ioctl((unsigned int) fd, DACIO_SET_VOLUME, 0x1f); //old is 10
    sys_ioctl((unsigned int) fd, DACIO_SET_PCM_RATE, (unsigned int) s_taker.pcm_data.sample_rate);
    /*! set stearo or mono voice; set 0 is stearo, set 1 is mono;	  */
    sys_ioctl((unsigned int) fd, DACIO_SET_PCM_CHANNEL, (unsigned int)((s_taker.pcm_data.channels == 2) ? 0 : 1));
    sys_ioctl((unsigned int) fd, DACIO_SET_PCM_BITS, (unsigned int) s_taker.pcm_data.bit_per_sample);

    sys_ioctl((unsigned int) fd, DACIO_GET_PCM_SYNC, 0);
    sys_ioctl((unsigned int) fd, DACIO_PCM_PLAY_ON, 1);

    sys_write((unsigned int) fd, s_taker.pcm_data.data, (unsigned int)s_taker.pcm_data.data_len);

    sys_ioctl((unsigned int) fd, DACIO_PCM_PLAY_ON, 0);
    sys_close((unsigned int) fd);
    return true;
}

void shutter_sound_init(void)
{
    memset(&s_taker, 0, sizeof(s_taker));
}

void shutter_sound_deinit(void)
{
    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value == 0xffff)
    {
        return ;
    }
    if (s_taker.pcm_data.data != NULL)
    {
        free(s_taker.pcm_data.data);
        s_taker.pcm_data.data = NULL;
    }
    memset(&s_taker, 0, sizeof(s_taker));
}

bool shutter_sound_change(void)
{
    char param[PATH_MAX] = {0};

    if (g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value == 0xffff)
    {
        return true;
    }
// TODO:别的地方是否也要这样判断？
    if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value > \
        g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value)
    {
        g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value = \
            g_camera_dsc_option_visible_cfgs[CAM_DSC_CFG_VISIBLE_TYPE_SHUTTER_SOUND].value;
    }
    switch (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].value)
    {
        case 0:
        if (g_camera_dsc_cfgs[CAM_DSC_CFG_TYPE_SHUTTER_SOUND].changed == true)
        {
            if (s_taker.pcm_data.data != NULL)
            {
                free(s_taker.pcm_data.data);
                s_taker.pcm_data.data = NULL;
            }
        }
        memset(&s_taker, 0, sizeof(s_taker));
        return true;

        case 1:
        snprintf(param, sizeof(param), g_camera_dsc_shutter_sound_cfgs[0].text);
        break;
        case 2:
        snprintf(param, sizeof(param), g_camera_dsc_shutter_sound_cfgs[1].text);
        break;
        case 3:
        snprintf(param, sizeof(param), g_camera_dsc_shutter_sound_cfgs[2].text);
        break;
        default:
        print_err();
        snprintf(param, sizeof(param), g_camera_dsc_shutter_sound_cfgs[0].text);
        break;
    }

    if (strcmp(s_taker.shutter_sound, param) != 0)
    {
        if (s_taker.pcm_data.data != NULL)
        {
            free(s_taker.pcm_data.data);
            s_taker.pcm_data.data = NULL;
        }
        s_taker.pcm_data.has_inited = 0;
        memset(&s_taker.shutter_sound, 0, PATH_MAX);
        memcpy(&s_taker.shutter_sound, param, strlen(param));
    }

    if (s_taker.pcm_data.has_inited == 0)
    {
        s_taker.result = _get_pcm_data((const char*) param, &s_taker.pcm_data);
        if (s_taker.result == false)
        {
            print_err();
            return false;
        }
    }
    return true;
}

