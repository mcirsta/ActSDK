/********************************************************************************
 *                              USDK(1100)
 *                             Module: photo app
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : photo_dec_ctrl.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-4-24          v1.0               build this file
 ********************************************************************************/
/*!
 * \file     photo_dec_ctrl.c
 * \brief    photo decoding control
 * \author   carrie
 *
 * \version 1.0
 * \date  2009/04/24
 *******************************************************************************/
#include "photo_dec_ctrl.h"

static void *s_mmm = NULL;
static void *s_codec_lib = NULL;

bool photo_dec_open(void)
{
    print_dbg();
    if (NULL == s_codec_lib)
    {
        s_codec_lib = dlopen("a_codec.so", RTLD_NOW);
        if (s_codec_lib == NULL)
        {
            print_err("open codec err!\n");
            return false;
        }
    }

    if (NULL == s_mmm)
    {
        s_mmm = mmm_id_open(NULL);
        if (s_mmm == NULL)
        {
            print_err("open mmm err!\n");
            return false;
        }
    }
    return true;
}

void photo_dec_close(void)
{
    print_dbg();

    if (NULL != s_mmm)
    {
        mmm_id_close(s_mmm, NULL);
        s_mmm = NULL;
    }

    if (NULL != s_codec_lib)
    {
        dlclose(s_codec_lib);
        s_codec_lib = NULL;
    }
}

bool photo_dec_control(dec_type_e type, unsigned int param)
{
    int result;
    bool ret = true;

    print_dbg();
    if ((NULL == s_codec_lib) || (NULL == s_mmm))
    {
        if(!photo_dec_open())
        {
            return false;
        }
    }

    switch (type)
    {
    case SET_FILE:
        print_dbg("");
        result = mmm_id_cmd(s_mmm, MMM_SET_FILE, param);
        if (result != 0)
        {
            print_warning("set file err!\n");
            ret = false;
        }
        break;

    case GET_DECODER_STATUS:
        result = mmm_id_cmd(s_mmm, MMM_GET_DECODER_STATUS, param);
        if (result != 0)
        {
            print_warning("get decoder status fail!\n");
            ret = false;
        }
        break;

    case SET_CALLBACK:
        result = mmm_id_cmd(s_mmm, MMM_SET_CALLBACK, param);
        if (result != 0)
        {
            print_warning("set callback err!\n");
            ret = false;
        }
        break;

    case IMG_DECODE:
        print_dbg("");
        result = mmm_id_cmd(s_mmm, MMM_IMG_DECODE, param);
        if (result != 0)
        {
            print_warning("image decode err!\n");
            ret = false;
        }
        break;

    case ZOOMIN:
        result = mmm_id_cmd(s_mmm, MMM_ZOOM, MMM_IMG_ZOOMIN);
        if (result != 0)
        {
            print_warning("zoomin err!\n");
            ret = false;
        }
        break;

    case ZOOMOUT:
        result = mmm_id_cmd(s_mmm, MMM_ZOOM, MMM_IMG_ZOOMOUT);
        if (result != 0)
        {
            print_warning("zoomout err!\n");
            ret = false;
        }
        break;

    case ROTATERIGHT90:
        result = mmm_id_cmd(s_mmm, MMM_ROTATE, MMM_IMG_RIGHT90);
        if (result != 0)
        {
            print_warning("rotate err!\n");
            ret = false;
        }
        break;

    case ROTATELEFT90:
        result = mmm_id_cmd(s_mmm, MMM_ROTATE, MMM_IMG_LEFT90);
        if (result != 0)
        {
            print_warning("rotate err!\n");
            ret = false;
        }
        break;

    case ROTATE180:
        result = mmm_id_cmd(s_mmm, MMM_ROTATE, MMM_IMG_R180);
        if (result != 0)
        {
            print_warning("rotate err!\n");
            ret = false;
        }
        break;

    case RESIZE:
        result = mmm_id_cmd(s_mmm, MMM_RESIZE, 0);
        if (result != 0)
        {
            print_warning("resize err!\n");
            ret = false;
        }
        break;

    case MOVEUP:
        result = mmm_id_cmd(s_mmm, MMM_MOVE, MMM_IMG_UP);
        if (result != 0)
        {
            print_warning("move err!\n");
            ret = false;
        }
        break;

    case MOVEDOWN:
        result = mmm_id_cmd(s_mmm, MMM_MOVE, MMM_IMG_DOWN);
        if (result != 0)
        {
            print_warning("move err!\n");
            ret = false;
        }
        break;

    case MOVELEFT:
        result = mmm_id_cmd(s_mmm, MMM_MOVE, MMM_IMG_LEFT);
        if (result != 0)
        {
            print_warning("move err!\n");
            ret = false;
        }
        break;

    case MOVERIGHT:
        result = mmm_id_cmd(s_mmm, MMM_MOVE, MMM_IMG_RIGHT);
        if (result != 0)
        {
            print_warning("move err!\n");
            ret = false;
        }
        break;

    case IMG_BROWSE:
        print_dbg();
        result = mmm_id_cmd(s_mmm, MMM_IMG_BROWSE, param);
        if (result != 0)
        {
            print_warning("image browse err!\n");
            ret = false;
        }
        break;

    case GET_MEDIA_INFO:
        result = mmm_id_cmd(s_mmm, MMM_GET_MEDIA_INFO, param);
        if (result != 0)
        {
            print_warning("get media info err!\n");
            ret = false;
        }
        break;

    case ABORT_DECODE:
        print_dbg();
        result = mmm_id_cmd(s_mmm, MMM_ABORT_DECODE, 0);
        print_dbg();
        if (result != 0)
        {
            print_warning("abort decode err!\n");
            ret = false;
        }
        break;

    default:
        break;

    }
    return ret;
}

