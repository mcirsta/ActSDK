/*******************************************************************************
 *                                us213f
 *                            Module: fusion
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       carrie     2009-04-07 15:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     fusion.c
 * \brief    the ui display fusion file
 * \author   carrie
 * \par      GENERAL DESCRIPTION:
 *               the ui display fusion file
 * \par      EXTERNALIZED FUNCTIONS:
 *               transimage.a
 * \version 1.0
 * \date  2009/4/7
 *******************************************************************************/


#include "include_case.h"
#include "transimage.h"
#include "fusion.h"

#define FRAME_DELAY     0
#define MAX_FRAME_NUM   10

typedef struct
{
    fb_info_t       fb_info;    /* fb 信息*/
    unsigned long   fb_addr;    /* framebuffer地址 */
    int             angle;      /* 屏幕角度 */
    bool            inited;     /* 是否初始化 */
    sty_rect_t      rect;       /* 在屏幕中的区域 */
    unsigned char   *pdata;     /* 数据地址 */
    bool            alpha;      /* 数据是否带alpha */
}fusion_info_t;

typedef struct
{
    fusion_effect_t     effect;
    bool                mode_inited;
    transval_t          transval;
    fusion_callback_t   callback;
    fusion_info_t       src_info;
    fusion_info_t       dst_info;
    bool                effect_inited;
}fusion_t;

static fusion_t     s_fusion;
static const clk_domain_t s_clk_domain = {CLK_LOAD_MIN, CLK_LOAD_HIGH_0};

// 随机个数最好为素数
static fusion_mode_e s_random_effect[] =
{
    FUSION_PUSH, FUSION_CHECK, FUSION_CUT, FUSION_ERASE,
    FUSION_LOUVER, FUSION_RANDOMLINE, FUSION_WAVE, FUSION_CUBE,
    FUSION_ROTATE, FUSION_NORMAL, FUSION_PUSH
};

static int s_last_random_index = 0;

#define RANDOM_NUM  (sizeof(s_random_effect)/sizeof(fusion_mode_e))


static void _rotate_src_buf(unsigned char **src_buf, int width, int height, int angle)
{
    int x, y;
    unsigned short *tmpbuf;
    unsigned short *tmpbuf_src;
    bool is_rotate = true;

    if (angle == 0)
    {
        return;
    }

    tmpbuf = (unsigned short *) malloc((unsigned int)(width * height * 2));
    if(tmpbuf == NULL)
    {
        print_err("malloc fail");
        return;
    }
    tmpbuf_src = (unsigned short *) (*src_buf);

    switch (angle)
    {
    case 90:
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                tmpbuf[(y * width) + x] = tmpbuf_src[((width - 1 - x) * height) + y];
            }
        }
        break;

    case 180:
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                tmpbuf[(y * width) + x] = tmpbuf_src[((height - 1 - y) * width) + (width - 1 - x)];
            }
        }
        break;

    case -90:
        for (y = 0; y < height; y++)
        {
            for (x = 0; x < width; x++)
            {
                tmpbuf[(y * width) + x] = tmpbuf_src[(x * height) + (height - 1 - y)];
            }
        }
        break;

    case 0:
    default:
        is_rotate = false;
        break;
    }

    if(true == is_rotate)
    {
        free(*src_buf);
        *src_buf = (unsigned char *) tmpbuf;
    }
    else
    {
        free(tmpbuf);
    }
}

/*
 * 拷贝src到dst的某个位置中去
 * 参数：
 * dst：                     目的首地址指针
 * x, y:        目的区域坐标
 * width_dst:   目的buffer的总宽
 * src:         源地址指针
 * width_src:   源buffer总宽
 * height_src:  源buffer总高
 */
static void _copy_region_buf(unsigned char *dst, int x, int y, int width_dst,
                unsigned char *src, int width_src, int height_src)
{
    int i;
    unsigned int cplen, len_dst;
    unsigned char *tmp_src, *tmp_dst;

    tmp_dst = dst + (((y * width_dst) + x) * 2);
    tmp_src = src;
    cplen = (unsigned int)width_src * 2;
    len_dst = (unsigned int)width_dst * 2;
    for(i = 0; i < height_src; i++)
    {
        memcpy(tmp_dst, tmp_src, cplen);
        tmp_dst += len_dst;
        tmp_src += cplen;
    }
}

/*
 * 刷屏，特效完成一帧之后调用
 */
static bool _update_framebuffer(int fd, fusion_t *f)
{
    /* 对源、目的都带alpha数据的特效，通常是提供给YUV blending读取，不需要刷屏 */
    if((true == f->src_info.alpha) && (true == f->dst_info.alpha))
    {
        goto out;
    }

    /* 特效输出buffer地址不是framebuffer，拷贝 */
    if(f->transval.bufout != (unsigned char *)f->dst_info.fb_addr)
    {
        _copy_region_buf((unsigned char *)f->dst_info.fb_addr,
                f->dst_info.rect.x, f->dst_info.rect.y,
                f->dst_info.fb_info.xres,
                f->transval.bufout, f->transval.width, f->transval.height);
    }

    out:
    if(NULL != f->callback)
    {
        f->callback((unsigned char *)f->dst_info.fb_addr,
                f->transval.viewparam.xview,
                f->transval.viewparam.yview);
    }

    if (ioctl(fd, FBIOSET_UPDATE_FB, 0) < 0)
    {
        print_err("ioctl device fb failed!");
        return false;
    }

    return true;
}


static bool _display_cube(fusion_t *f)
{
    int i, fd;
    transval_t *tv = &f->transval;

    print_dbg();

    tv->mode = TRANS_CUBE;

    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        return false;
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        tv->currentframe = i + 1;
        if (tv->directflag == TRANS_LEFT)
        {
            tv->alpha = 90 - ((tv->currentframe * 90) / tv->totalframes);
        }
        else
        {
            tv->alpha = (tv->currentframe * 90) / tv->totalframes;
        }

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            close(fd);
            return false;
        }

        if(!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed!");
            close(fd);
            return false;
        }
        mdelay(FRAME_DELAY);
    }
    close(fd);
    return true;
}

static bool _display_rotate(fusion_t *f)
{
    bool ret = false;
    int i, fd;
    unsigned char *tmp_buf, *src_buf, *dst_buf, *frame_buf;
    transval_t *tv = &f->transval;

    fd = -1;
    tmp_buf = NULL;
    src_buf = tv->srcbuf;
    dst_buf = tv->desbuf;
    frame_buf = tv->bufout;

    tmp_buf = (unsigned char *) malloc((unsigned int)(2 * tv->viewparam.xview * tv->viewparam.yview));
    if (!tmp_buf)
    {
        print_err("tmp_buf malloc err!\n");
        goto out;
    }

    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        goto out;
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        print_dbg();
        tv->mode = TRANS_ZOOM;
        tv->currentframe = tv->totalframes - i;
        tv->srcbuf = NULL;
        tv->desbuf = src_buf;
        tv->bufout = tmp_buf;

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            goto out;
        }

        tv->mode = TRANS_ROTATION_2D;
        tv->srcbuf = tmp_buf;
        tv->desbuf = src_buf;
        tv->bufout = frame_buf;

        tv->currentframe = i + 1;
        if (tv->directflag == TRANS_LEFT)
        {
            tv->alpha = (90 * tv->currentframe) / tv->totalframes;
        }
        else
        {
            tv->alpha = 360 - ((90 * tv->currentframe) / tv->totalframes);
        }

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            goto out;
        }

        if (!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed! ret=%d\n");
            goto out;
        }
        mdelay(FRAME_DELAY);
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        print_dbg();
        tv->mode = TRANS_ZOOM;
        tv->currentframe = i + 1;
        tv->srcbuf = NULL;
        tv->desbuf = dst_buf;
        tv->bufout = tmp_buf;

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            goto out;
        }

        tv->mode = TRANS_ROTATION_2D;
        tv->srcbuf = tmp_buf;
        tv->desbuf = dst_buf;
        tv->bufout = (unsigned char *) frame_buf;

        tv->currentframe = tv->totalframes - i;
        if (tv->directflag == TRANS_RIGHT)
        {
            tv->alpha = (90 * tv->currentframe) / tv->totalframes;
        }
        else
        {
            tv->alpha = 360 - ((90 * tv->currentframe) / tv->totalframes);
        }

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            goto out;
        }

        if (!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed!\n");
            goto out;
        }
        mdelay(FRAME_DELAY);
    }

    /* 刷最后一帧到屏幕 */
    tv->bufout = dst_buf;
    if (!_update_framebuffer(fd, f))
    {
        print_err("ioctl device fb failed!\n");
        goto out;
    }
    ret = true;

    out:
    if(NULL != tmp_buf)
    {
        free(tmp_buf);
    }

    if(fd >= 0)
    {
        close(fd);
    }
    tv->srcbuf = src_buf;
    tv->desbuf = dst_buf;
    return ret;
}

static bool _display_reverse(fusion_t *f)
{
    int i, fd;
    transval_t *tv = &f->transval;

    print_dbg();

    tv->totalframes = MAX_FRAME_NUM * 2;
    tv->mode = TRANS_REVERSE;

    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        return false;
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        tv->currentframe = i + 1;
        if (tv->directflag == TRANS_RIGHT)
        {
            tv->alpha = (180 * tv->currentframe) / tv->totalframes;
        }
        else
        {
            tv->alpha = 360 - ((180 * tv->currentframe) / tv->totalframes);
        }

        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            close(fd);
            return false;
        }

        if (!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed!");
            close(fd);
            return false;
        }
        mdelay(FRAME_DELAY);
    }
    close(fd);
    return true;
}

static bool _display_page(fusion_t *f)
{
    bool ret = false;
    int i, fd = -1;
    unsigned char *src_buf, *dst_buf;
    transval_t *tv = &f->transval;
    int cur_dir;

    print_dbg();

    tv->mode = TRANS_PAGE;

    src_buf = tv->srcbuf;
    dst_buf = tv->desbuf;
    cur_dir = tv->directflag;

    /* 应用传递的参数虽然有左右之分，但是对于特效库只有向左，因此要把src和dst buff互换 */
    if(tv->directflag == TRANS_RIGHT)
    {
        tv->srcbuf = dst_buf;
        tv->desbuf = src_buf;
        tv->directflag = TRANS_LEFT;
    }

    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        goto out;
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        if(cur_dir == TRANS_RIGHT)
        {
            /*
             * 这里是从9->0, 如果是10->1，最后一帧（dst_buff)没有刷到
             * 只有这个特效有这个情况，应用传递的参数虽然有左右之分，但是对于特效库只有向左
             * 因此有特殊处理，见前面。
             */
            tv->currentframe = tv->totalframes - i - 1;
        }
        else
        {
            tv->currentframe = i + 1;
        }
        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            goto out;
        }

        if (!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed!");
            goto out;
        }
        mdelay(FRAME_DELAY);
    }
    ret = true;

    out:
    tv->srcbuf = src_buf;
    tv->desbuf = dst_buf;
    if(fd >= 0)
    {
        close(fd);
    }
    return ret;
}

static bool _display_other(fusion_t *f, int mode)
{
    int i, fd;
    transval_t *tv = &f->transval;

    print_dbg();

    tv->mode = mode;

    fd = open(DEVICE_FB, O_RDWR, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        return false;
    }

    for (i = 0; i < tv->totalframes; i++)
    {
        tv->currentframe = i + 1;
        if (transimage(tv) < 0)
        {
            print_err("transimage err!");
            close(fd);
            return false;
        }

        if (!_update_framebuffer(fd, f))
        {
            print_err("ioctl device fb failed!");
            close(fd);
            return false;
        }
        mdelay(FRAME_DELAY);
    }
    close(fd);
    return true;
}

static bool _get_lcd_info(fusion_info_t *fusion_fb)
{
    int fd, ret;

    fd = open(DEVICE_FB, O_RDONLY, 0);
    if (fd < 0)
    {
        print_err("open device fb failed!\n");
        return false;
    }

    /* 获取屏幕参数 */
    ret = ioctl(fd, FBIOGET_FB_PARAM, (int) &fusion_fb->fb_info);
    if (ret < 0)
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return false;
    }

    /* 获取framebuffer地址 */
    ret = ioctl(fd, FBIOGET_FB_ADDR, (int) &fusion_fb->fb_addr);
    if (ret < 0)
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return false;
    }

    /* 获取fb角度*/
    ret = ioctl(fd, FBIOGET_SCREEN_ROTATE, (int) &fusion_fb->angle);
    if (ret < 0)
    {
        print_err("ioctl device fb failed! ret=%d\n", ret);
        close( fd );
        return false;
    }

    close(fd);

    return true;
}


static bool _get_cur_framebuffer_data(fusion_frame_t *frame, bool update)
{
    int len, result;
    gui_fb_area_t fb_area;

    len = 2 * frame->rect.width * frame->rect.height;
    if(NULL != frame->pdata)
    {
        free(frame->pdata);
    }
    frame->pdata = (unsigned char *) malloc((unsigned int)len);
    if(NULL == frame->pdata)
    {
        print_err("mallo error!");
        return false;
    }

    if(true == update)
    {
        gui_framebuffer_update();
    }
    fb_area.x = frame->rect.x;
    fb_area.y = frame->rect.y;
    fb_area.width = frame->rect.width;
    fb_area.height = frame->rect.height;
    fb_area.pdata = (unsigned short *) frame->pdata;
    result = gui_get_area(&fb_area);
    if (result != 0)
    {
        print_warning("gui_get_area err! result = %d \n", result);
        return false;
    }

    return true;
}

/*
 * 获取特效帧信息
 */
static bool _get_fusion_info(fusion_info_t *info, fusion_frame_t *input_frame, bool update)
{
    unsigned int len;
    int bpp;
    fusion_frame_t tmp_frame;

    _get_lcd_info(info);

    if((NULL != input_frame)
        && (((input_frame->rect.x + input_frame->rect.width) > info->fb_info.xres)
            || ((input_frame->rect.y + input_frame->rect.height) > info->fb_info.yres)
            || ((true == input_frame->alpha) && (NULL == input_frame->pdata))))
    {
        print_warning("rect.x:%d, rect.y:%d, rect.width:%d, rect.height:%d",
                input_frame->rect.x, input_frame->rect.y, input_frame->rect.width,
                input_frame->rect.height);
        print_warning("fb.xres:%d, fb.yres:%d", info->fb_info.xres, info->fb_info.yres);
        return false;
    }

    memset(&tmp_frame, 0, sizeof(fusion_frame_t));
    tmp_frame.alpha = false;

    if(NULL == input_frame)
    {
        tmp_frame.rect.width = (short)info->fb_info.xres;
        tmp_frame.rect.height = (short)info->fb_info.yres;
        _get_cur_framebuffer_data(&tmp_frame, update);
        memcpy(&info->rect, &tmp_frame.rect, sizeof(sty_rect_t));
    }
    else if(NULL == input_frame->pdata)
    {
        memcpy(&tmp_frame, input_frame, sizeof(fusion_frame_t));
        _get_cur_framebuffer_data(&tmp_frame, update);
        memcpy(&info->rect, &tmp_frame.rect, sizeof(sty_rect_t));
    }
    else
    {
        bpp = 2;
        if(true == input_frame->alpha)
        {
            tmp_frame.alpha = true;
            bpp = 3;
        }
        len = (unsigned int)(bpp * input_frame->rect.width * input_frame->rect.height);
        tmp_frame.pdata = (unsigned char *) malloc(len);
        if(NULL == tmp_frame.pdata)
        {
            return false;
        }
        memcpy(tmp_frame.pdata, input_frame->pdata, len);
        memcpy(&info->rect, &input_frame->rect, sizeof(sty_rect_t));
    }

    info->alpha = tmp_frame.alpha;
    info->pdata = tmp_frame.pdata;

    return true;
}

/*
 *  设置特效参数
 */
bool fusion_set_effect(fusion_effect_t *effect)
{
    if(false == s_fusion.effect_inited)
    {
        memset(&s_fusion, 0, sizeof(fusion_t));
        s_fusion.effect_inited = true;
    }

    s_fusion.effect = *effect;
    s_fusion.mode_inited = true;
    return true;
}

/*
 * 设置特效的第一帧
 * 参数：
 *    frame -- 包含该帧区域及rgb数据的结构体指针，如为NULL，则取当前全屏的framebuffer数据
 */
bool fusion_set_first_frame(fusion_frame_t *frame)
{
    /* 已经调用过fusion_set_effect */
    if((true == s_fusion.mode_inited) && (FUSION_NORMAL == s_fusion.effect.mode))
    {
        return true;
    }

    if(false == s_fusion.effect_inited)
    {
        memset(&s_fusion, 0, sizeof(fusion_t));
        s_fusion.effect_inited = true;
    }

    if(!_get_fusion_info(&s_fusion.src_info, frame, false))
    {
        print_warning("_get_fusion_info fail!");
        return false;
    }

    if(NULL != s_fusion.transval.srcbuf)
    {
        free(s_fusion.transval.srcbuf);
    }

    s_fusion.transval.srcbuf = s_fusion.src_info.pdata;
    s_fusion.src_info.inited = true;

    return true;
}

/*
 * 设置特效的最后一帧
 * 参数：
 *    frame -- 包含该帧区域及rgb数据的结构体指针，如为NULL，则取当前全屏的framebuffer数据
 */
bool fusion_set_last_frame(fusion_frame_t *frame)
{
    /* 已经调用过fusion_set_effect */
    if((true == s_fusion.mode_inited) && (FUSION_NORMAL == s_fusion.effect.mode))
    {
        return true;
    }

    if(!_get_fusion_info(&s_fusion.dst_info, frame, true))
    {
        print_warning("_get_fusion_info fail!");
        return false;
    }

    if(NULL != s_fusion.transval.desbuf)
    {
        free(s_fusion.transval.desbuf);
    }

    s_fusion.transval.desbuf = s_fusion.dst_info.pdata;
    s_fusion.transval.width = s_fusion.dst_info.rect.width;
    s_fusion.transval.height = s_fusion.dst_info.rect.height;

    if(!s_fusion.dst_info.alpha)
    {
        s_fusion.transval.bufout = (unsigned char *)s_fusion.dst_info.fb_addr;
        s_fusion.transval.viewparam.xpos = s_fusion.dst_info.rect.x;
        s_fusion.transval.viewparam.ypos = s_fusion.dst_info.rect.y;
        s_fusion.transval.viewparam.xview = s_fusion.dst_info.fb_info.xres;
        s_fusion.transval.viewparam.yview = s_fusion.dst_info.fb_info.yres;
    }
    else
    {
        assert(frame != NULL);
        assert(frame->pdata != NULL);
        s_fusion.transval.bufout = (unsigned char *)frame->pdata;   /* 使用应用传递进来的地址 */
        s_fusion.transval.viewparam.xpos = 0;
        s_fusion.transval.viewparam.ypos = 0;
        s_fusion.transval.viewparam.xview = s_fusion.dst_info.rect.width;
        s_fusion.transval.viewparam.yview = s_fusion.dst_info.rect.height;
    }

    s_fusion.dst_info.inited = true;

    return true;
}

static int _get_rotation_angle(fusion_t *f)
{
    int rotate = 0;

    if((false == f->src_info.inited) || (false == f->dst_info.inited))
    {
        /* 第一帧或最后一帧未初始化 */
        return 0;
    }

    if(((f->src_info.angle == ROTATION_0) && (f->dst_info.angle == ROTATION_90))
            || ((f->src_info.angle == ROTATION_90) && (f->dst_info.angle == ROTATION_180))
            || ((f->src_info.angle == ROTATION_180) && (f->dst_info.angle == ROTATION_270))
            || ((f->src_info.angle == ROTATION_270) && (f->dst_info.angle == ROTATION_0)))
    {
        rotate = 90;
    }
    else if(((f->src_info.angle == ROTATION_90) && (f->dst_info.angle == ROTATION_0))
            || ((f->src_info.angle == ROTATION_180) && (f->dst_info.angle == ROTATION_90))
            || ((f->src_info.angle == ROTATION_270) && (f->dst_info.angle == ROTATION_180))
            || ((f->src_info.angle == ROTATION_0) && (f->dst_info.angle == ROTATION_270)))
    {
        rotate = -90;
    }
    else if(((f->src_info.angle == ROTATION_0) && (f->dst_info.angle == ROTATION_180))
            || ((f->src_info.angle == ROTATION_90) && (f->dst_info.angle == ROTATION_270))
            || ((f->src_info.angle == ROTATION_180) && (f->dst_info.angle == ROTATION_0))
            || ((f->src_info.angle == ROTATION_270) && (f->dst_info.angle == ROTATION_90)))
    {
        rotate = 180;
    }
    else
    {
        rotate = 0;
    }

    return rotate;
}


bool _get_rtc_stdsec(unsigned long * stdsec)
{
    rtc_date_time_t date_time;
    int fd = -1;

    fd = open(DEVICE_RTC, O_RDWR);
    if(fd < 0)
    {
        print_err("open device rtc fail");
        return false;
    }
    if(ioctl(fd, RTC_GET_DATE_TIME, (int)(&date_time)) != 0)
    {
        print_err("get rtc date and time failed!");
        close(fd);
        return false;
    }

    if(ioctl(fd, RTC_CHANGE_TO_STDSEC, (int)(&date_time)) != 0)
    {
        print_err("change to standard second failed!");
        close(fd);
        return false;
    }

    *stdsec = date_time.stdsec;
    close(fd);
    return true;
}


static bool fusion_display_custom(fusion_effect_t *effect, fusion_callback_t callback)
{
    bool ret = true;
    unsigned int len;
    unsigned long temp;
    bool is_random = false;

    if((false == s_fusion.src_info.inited) || (false == s_fusion.dst_info.inited)
        || (NULL == effect)
#pragma __PRQA_IGNORE_START__
        || (effect->direct < TRANS_UP)
        || (effect->direct > TRANS_RIGHT)
#pragma __PRQA_IGNORE_END__
        )
    {
        /* 第一帧或最后一帧未初始化 */
        ret = false;
        goto end_display;
    }

    /* 如果first和last的屏幕角度不一致，则旋转buf */
    if(s_fusion.dst_info.angle != s_fusion.src_info.angle)
    {
        _rotate_src_buf(&s_fusion.transval.srcbuf, s_fusion.transval.width,
                    s_fusion.transval.height, _get_rotation_angle(&s_fusion));
    }

    s_fusion.transval.totalframes = MAX_FRAME_NUM;
    s_fusion.transval.directflag = effect->direct;
    s_fusion.callback = callback;

    if(effect->mode == FUSION_RANDOM)
    {
        is_random = true;
        _get_rtc_stdsec(&temp); // 用当前时间做为随机种子
        srandom(temp);
        temp = (unsigned long)random();
        temp  = temp % RANDOM_NUM;
        effect->mode = s_random_effect[temp];
        //printf("%s---%d: random index:%d\n", __FILE__, __LINE__, temp);
        
        s_last_random_index = (int)temp;
        if(s_last_random_index >= (int)(RANDOM_NUM - 1))
        {
            s_last_random_index = 0;
        }
        s_random_effect[RANDOM_NUM - 1] = s_random_effect[s_last_random_index];
    }

    if (effect->mode != FUSION_NORMAL)
    {
        sys_pm_set_clk_domain(&s_clk_domain);

        switch (effect->mode)
        {
        case FUSION_POPUPALPHA: /*弹出透明子菜单(RGB)，支持上、下、左、右*/
            s_fusion.transval.alpha = 9;
            ret = _display_other(&s_fusion, TRANS_POPUPALPHA);
            assert(true == s_fusion.dst_info.alpha);
            len = (unsigned int)(3 * s_fusion.dst_info.rect.width * s_fusion.dst_info.rect.height);
            memcpy(s_fusion.transval.bufout, s_fusion.dst_info.pdata, len); /* 目的数据拷贝回去 */
            break;

        case FUSION_MOVEALPHA: /*弹出透明子菜单(YUV)，支持上、下、左、右*/
            ret = _display_other(&s_fusion, TRANS_MOVEALPHA);
            break;

        case FUSION_CUBE: /*立方体，支持左、右*/
            ret = _display_cube(&s_fusion);
            break;

        case FUSION_REVERSE: /*翻转，支持左、右*/
            ret = _display_reverse(&s_fusion);
            break;

        case FUSION_PUSH: /*推送，支持左、右*/
            ret = _display_other(&s_fusion, TRANS_MOVE);
            break;

        case FUSION_ROTATE: /*旋转，支持左、右*/
            ret = _display_rotate(&s_fusion);
            break;

        case FUSION_POPUP: /*弹出or ppt，支持左、右*/
            ret = _display_other(&s_fusion, TRANS_POPUP);
            break;

        case FUSION_FADE:
            ret = _display_other(&s_fusion, TRANS_FADE);
            break;

        case FUSION_WAVE:
            ret = _display_other(&s_fusion, TRANS_WAVE);
            break;

        case FUSION_CHECK:
            ret = _display_other(&s_fusion, TRANS_CHECK);
            break;

        case FUSION_LOUVER:
            ret = _display_other(&s_fusion, TRANS_LOUVER);
            break;

        case FUSION_PAGE:
            ret = _display_page(&s_fusion);
            break;

        case FUSION_RANDOMLINE:
            ret = _display_other(&s_fusion, TRANS_RANDOMLINE);
            break;

        case FUSION_CUT:
            ret = _display_other(&s_fusion, TRANS_CUT);
            break;

        case FUSION_ERASE:
            ret = _display_other(&s_fusion, TRANS_ERASE);
            break;

        default:
            if(NULL != s_fusion.callback)
            {
                s_fusion.callback(s_fusion.transval.bufout,
                        s_fusion.transval.viewparam.xview,
                        s_fusion.transval.viewparam.yview);
            }
            break;
        }

        sys_pm_clear_clk_domain(&s_clk_domain);
    }
    else
    {
        if(true == is_random)   // 刷屏
        {
            int fd;
            if(NULL != s_fusion.callback)
            {
                s_fusion.callback(s_fusion.transval.bufout,
                        s_fusion.transval.viewparam.xview,
                        s_fusion.transval.viewparam.yview);
            }
            fd = open(DEVICE_FB, O_RDWR, 0);
            ioctl(fd, FBIOSET_UPDATE_FB, 0);
            close(fd);
        }
    }

    end_display:
    if(s_fusion.transval.srcbuf != NULL)
    {
        free(s_fusion.transval.srcbuf);
        s_fusion.transval.srcbuf = NULL;
    }

    if(s_fusion.transval.desbuf != NULL)
    {
        free(s_fusion.transval.desbuf);
        s_fusion.transval.desbuf = NULL;
    }

    s_fusion.src_info.inited = false;
    s_fusion.dst_info.inited = false;
    s_fusion.effect_inited = false;

    return ret;
}


bool fusion_display(fusion_callback_t callback)
{
    if(false == s_fusion.mode_inited)
    {
        s_fusion.effect.mode = FUSION_NORMAL;
        s_fusion.effect.direct = TRANS_LEFT;
        s_fusion.mode_inited = true;
    }
    return fusion_display_custom(&s_fusion.effect, callback);
}


static const unsigned int fusion_apitbl[]=
{
    (unsigned int)fusion_set_effect,
    (unsigned int)fusion_set_first_frame,
    (unsigned int)fusion_set_last_frame,
    (unsigned int)fusion_display,
    (unsigned int)fusion_set_config,
    (unsigned int)fusion_get_config,
    (unsigned int)fusion_update_config,
};                
                  
/******************************************************************************/
/*!                    
* \par  Description:
*      init函数
* \note
* 使用宏__init确保函数定位在.ctors段
*******************************************************************************/
int __attribute__((constructor)) fusion_api_init(void)
{
    api_install(APINO_FUSION, (unsigned)fusion_apitbl);
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      exit函数
* \note
* 使用宏__exit确保函数定位在.dtors段
*******************************************************************************/
void __attribute__((destructor)) fusion_api_exit(void)
{
    api_uninstall(APINO_FUSION);
    return;
}

