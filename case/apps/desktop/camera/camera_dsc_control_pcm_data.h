/*******************************************************************************
 *                              USDK(1100)
 *                            Module: camera_dsc
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    Albert_lyu 2009-6-5  1.0             build this file
 *******************************************************************************/
/*!
 * \file    camera_dsc_control_pcm_data.h
 * \brief    wav转换为pcm到buf头文件
 * \author   Albert_lyu
 * \version 1.0
 * \date  2009-6-5
 *******************************************************************************/

#ifndef __CAMERA_DSC_CONTROL_PCM_DATA_H__
#define __CAMERA_DSC_CONTROL_PCM_DATA_H__
#include "include_case.h"
#include "include_psp.h"

/*!
 * \defgroup  pcm_data
 */

typedef struct
{
    unsigned char *data;
    int data_len;
    int sample_rate;
    int channels;
    int bit_per_sample;
    int has_inited;

} pcm_data_t;

/*!
 *    The riff chunk of .wav file,
 *    always at the beginning of the file,
 *    the size is 12 bytes.
 */
typedef struct
{
    unsigned int chunk_id;
    int chunk_size;
    unsigned int riff_type;

} wav_riff_type_chunk_t;

/*!
 *    The formate chunk of .wav file,
 *    the size is 24 bytes.
 */
typedef struct
{
    unsigned int chunk_id;
    int chunk_size;
    short format_tag;
    unsigned short channels;
    unsigned int samples_per_sec;
    unsigned int avg_bytes_per_sec;
    unsigned short block_align;
    unsigned short bits_per_sample;

} wav_format_chunk_t;

/*!
 *    The data chunk of .wav file,
 *    the size is 8 bytes.
 */
typedef struct
{
    unsigned int chunk_id;
    int chunk_size;

} wav_data_chunk_t;

typedef struct
{
    pcm_data_t pcm_data; 

    char shutter_sound[PATH_MAX];
    bool result;

} img_taker_t;
typedef struct
{
    unsigned int sample_rate; //set sample rate, set 0:96k; 1:48k; 2:32k; ...... 6:8k; refer to upside;
    unsigned int channel; //set stearo or mono voice; set 0 is stearo, set 1 is mono;
    unsigned int precision; //set sample precision, reserved;
} dac_para_t;

extern bool shutter_sound_play(void);
extern void shutter_sound_init(void);
extern void shutter_sound_deinit(void);
extern bool shutter_sound_change(void);

#endif   /* __CAMERA_DSC_CONTROL_PCM_DATA_H__ */

