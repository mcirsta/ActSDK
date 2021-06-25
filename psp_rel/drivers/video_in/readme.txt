/*
********************************************************************************
*                   213x --- video in device driver
*                (c) Copyright 2007 - 2010, Actions Co,Ld. 
*                        All Right Reserved 
*
* History:        
*      <author>      <time>       <version >    <desc>
*       houjingkun      2008/06/18     1.0         build this file 
*       houjingkun      2008/08/04     1.1         change some file name, from XXX_internal to XXX_inner.
*                                                  and XXX_param to XXX_module. 
********************************************************************************/ 


the video in device driver's source code structure seems like as this:

                    -------- 
                    vi_drv.h        /* provide api for app and midware. */
                    vi_drv.c
                        |
            ---------------------------------
            |                               |
        sensor_inner.h              tvin_inner.h
        sensor_inner.c              tvin_inner.c
            |                               |
        -------------------          --------------
        /                  \         /             \
       |                    ----------              |
       |                    vi_common.h             |
       |                    vi_common.c             |
       |                    vi_buf.h                |
       |                    vi_buf.c                |
    ---------                                    ---------
    sensor_hw.h                                  tvin_hw.h
    sensor_hw.c                                  tvin_hw.c
    sensor_module.c
 -----------------------------------------------------------
                        vi_sys.h
                        vi_sys.c

