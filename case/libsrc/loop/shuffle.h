/********************************************************************************
 *                              USDK(1100)
 *                             Module: lib 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *                                      v1.0               build this file 
 *      shaofeng    2009-05-17          v1.1               优化loopmode
 *      shaofeng    2009-05-18          v1.2               按照case代码规范整理代码
 ********************************************************************************/
/*!
* \file     shuffle.h
* \brief    
* \author   shaofeng
*               
* \version 1.0
* \date  2009/05/18
*******************************************************************************/

#ifndef __SHUFFLE_H__
#define __SHUFFLE_H__

/*! \cond loop_API*/

//可以完成 3008 首歌的 shuffle
#define      SHUFFLE_BUFFER_MAX       0xBC0


typedef struct
{
    int total_files ;   //shuffle 的最大值，每次输出在 [0，shuffle_range-1]
    int cur_total ;     //当前组的文件总数，最大为SHUFFLE_BUFFER_MAX
    int cur_index ;     //当前shuffle_buf的位置
    int cur_offset ;    //当前组内歌曲的偏移，一般为SHUFFLE_BUFFER_MAX 的整数倍
    int start_offset;   //第一组歌曲的偏移，用于 LOOP_MODE_SHUFFLE_REPEAT 判断一轮shuffle结束
    int per_count ;     //回退的个数，用于回退后继续按之前的顺序播放
    int shuffle_buf[SHUFFLE_BUFFER_MAX];     //存储当前组的歌曲顺序，小于cur_index为已播，大于为未播
    char shuffle_mod;     //分为SHUFFLE_MODE 和 SHUFFLE_REPEAT_MODE，前者只播一遍，后者循环播放
}shuffle_t;




/* shuffle 内部初始化 */
extern bool shuffle_init(void);

/*
 设定shuffle参数，需指定一个当前首(正在播放的歌曲)。
 INT32U total_files, 总文件数，可为任意值，最大为4294967295
 INT32U cur_index,   当前正在播放的歌曲在播放列表的index
 INT8U mode          shuffle 模式
*/
extern bool shuffle(int total_files, int cur_index, char mode);

/* 取得下一首歌索引号*/
extern int shuffle_next(void);

/* 取得上一首歌索引号  */
extern int shuffle_prev(void);

/* 删除播放列表中del_index 的歌曲，shuffle模块不会对播放列表进行任何操作，
只是按新的播放列表重新排列内部数据结构*/
extern int shuffle_del(int del_index);

/*! \endcond*/
#endif  /* __SHUFFLE_H__ */

