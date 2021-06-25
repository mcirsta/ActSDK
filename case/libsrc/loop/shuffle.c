/*
 ********************************************************************************
 *           NOYA(MAYA)---shuffle
 *        (c) Copyright 2002-2006, Actions Co,Ld.
 *            All Right Reserved
 *
 * FileName: shuffle.c     Author:  yangyongdong        Date:2007/06/14
 * Description:
 1. 定义了shuffle函数的实现
 * Others:     // 其它内容的说明
 * History:    // 修改历史记录列表
 *      <author>    <time>      <version >    <desc>
 *     yyd          2007/12/24     0.1        build this file
 *     yyd          2007/12/24     0.2        修改shuffle_del实现
 *     yyd          2008/02/24     1.0        修改一些隐藏的bug
 *     yyd          2008/08/04     1.1        重写部分代码，通过QAC与PC-lint检查
 *     xsz          2008/09/03     2.0        修改部分代码
 *     shaofeng     2009-05-17     v2.1       优化loopmode
 *     shaofeng     2009-05-18     v2.2       按照case代码规范整理代码
 ********************************************************************************
 */

#include "include_case.h"


/* 本应用的头文件 */
#include "shuffle.h"



static shuffle_t pShuffle;


static void _set_cur_total(int num);
static bool _check_index(void);
static bool _modify_index(int cur_index);

#if 0
#endif
/******************************************************************
**   算法原型
**   int random_shuffle(int first, int last, short *v)
**   {
**       int i, k = 0;
**       short r;
**       for ( i = first ; i != last; i++)
**       {
**           // swap(i, first + rand() % ((i - first) + 1));
**           r = v[i];
**           k = i + rand() % (last - i);
**           v[i] = v[k];
**           v[k] = r;
**           // mDISPLAY("%d ",v[i] + 1);
**       }
**       return 0;
**   }
*****************************************************************/

/******************************************************************************/
/*!
* \par Description:
*        设置当前组的总文件数为num，
*        num必须小于SHUFFLE_BUFFER_MAX，
*        若num与上一组数目不同时，重新置乱当前组
* \param[in]    num -- total number
* \retval       0 为设置成功， -1为失败
*******************************************************************************/
static void _set_cur_total(int num)
{
    int tmp = pShuffle.cur_total;
    
    pShuffle.cur_total = num;
    if(tmp != num)
    {
        if(((num % 7) == 0 ) && (num > 0 ))
        {
            num = num - 1;
            pShuffle.shuffle_buf[num] = num;
        }
        for(tmp = 0; tmp < num; tmp++)
        {
            if ( 0 != num )
            {
                pShuffle.shuffle_buf[tmp] = (tmp * 16807) % num;
            }
        }
    }
}

/*****************************************************************************/
/*!
* \par Description:
*        检查本组是否结束，如果结束则换入下一组
* \param[in]    void  
* \retval       true: 为设置成功， false: 为失败
*****************************************************************************/
static bool _check_index(void)
{
    if(pShuffle.cur_index < pShuffle.cur_total)
    {
        return true;
    }
    else
    {
        if(pShuffle.total_files < 1)
        {
            return false;
        }
        
        pShuffle.cur_offset = pShuffle.cur_offset + pShuffle.cur_total; 
        if(pShuffle.cur_offset >= pShuffle.total_files)
        {
            pShuffle.cur_offset = 0;        
        }
        if((pShuffle.cur_offset == pShuffle.start_offset)&&(pShuffle.shuffle_mod == LOOP_MODE_SHUFFLE))
        {
            pShuffle.cur_index = pShuffle.cur_total -1;
            return false;
        }
        
        if((pShuffle.total_files - pShuffle.cur_offset) > SHUFFLE_BUFFER_MAX)
        {
            _set_cur_total(SHUFFLE_BUFFER_MAX);
        }
        else
        {
            _set_cur_total(pShuffle.total_files - pShuffle.cur_offset);
        }
        
        pShuffle.cur_index = 0;
        pShuffle.per_count = 0;
        
        return true;
    }

}

/*********************************************************************************************/
/*!
* \par Description:
*        设定当前播放歌曲index
* \param[in]    cur_index -- 为正在播放的index
* \retval       true: 为设置成功， false: 为失败
**********************************************************************************************/
static bool _modify_index(int cur_index)
{
    int counta;
    int tmp;
    int index_tmp = cur_index - pShuffle.cur_offset;
    
    pShuffle.cur_index = 0;
    pShuffle.per_count = 0;
    if(index_tmp < SHUFFLE_BUFFER_MAX)
    {
        for(counta = 0; counta < pShuffle.cur_total; counta++)
        {
            if(index_tmp == pShuffle.shuffle_buf[counta])
            {
                tmp = pShuffle.shuffle_buf[0];
                pShuffle.shuffle_buf[0] = pShuffle.shuffle_buf[counta];
                pShuffle.shuffle_buf[counta] = tmp;
                
                return true;
            }
        }
    }
    
    return false;
}

/*******************************************************************************************/
/*!
* \par Description:
*        功能描述：设置初始值，在Shuffle_Init之后调用或调整了文件总数(total_files)，
*        当前歌曲（cur_index）或Shuffle模式（mode）后调用。
* \param[in]    total_files -- 需要Shuffle的文件总数
* \param[in]    cur_index   -- 当前歌曲index
* \param[in]    mode，Shuffle模式SHUFFLE_MODE或SHUFFLE_REPEAT_MODE
* \retval       正确为true，否则为false
*******************************************************************************************/
bool shuffle(int total_files, int cur_index, char mode)
{
    pShuffle.total_files = total_files;
    pShuffle.start_offset = (cur_index / SHUFFLE_BUFFER_MAX) * SHUFFLE_BUFFER_MAX;
       
    if((total_files - pShuffle.start_offset) > SHUFFLE_BUFFER_MAX)
    {
        _set_cur_total(SHUFFLE_BUFFER_MAX);
    }
    else
    {
        _set_cur_total(total_files - pShuffle.start_offset);
    }
 
    pShuffle.cur_offset = pShuffle.start_offset;
  
    pShuffle.shuffle_mod = mode;
    
    return _modify_index(cur_index);
}


/******************************************************************************/
/*!
* \par Description:
*        Initialization data structure
* \param[in]    void
* \retval       always return true
*******************************************************************************/
bool shuffle_init(void)
{
    memset( &pShuffle, 0, sizeof(shuffle_t) );
    
    return true;
}


/******************************************************************************************/
/*!
* \par Description:
*        shuffle_next,不考虑组的偏移
* \param[in]    void
* \retval       SHUFFLE_MODE下遍历结束返回-1, 其它返回组内偏移
*******************************************************************************************/
int shuffle_sig_next(void)
{
    int swap_index;
    int tmp;
    int ret;
    /*如果有回退，先按照回退顺序播放*/
    if((pShuffle.per_count > 0)&&(pShuffle.cur_index >= pShuffle.per_count))
    {
        pShuffle.per_count--;
        return pShuffle.shuffle_buf[pShuffle.cur_index - pShuffle.per_count];
    }
    else
    {
        pShuffle.per_count = 0;
        pShuffle.cur_index++;
    }
    /**检查cur_index，判断是否结束或者需要分组*/
    if( false == _check_index() )
    {
        return -1;
    }
    
    tmp = pShuffle.cur_total - pShuffle.cur_index - 1;
    if( tmp > 0)
    {
        swap_index = pShuffle.cur_index + 1 + ((int)rand() % tmp);

        tmp = pShuffle.shuffle_buf[pShuffle.cur_index];
        pShuffle.shuffle_buf[pShuffle.cur_index] = pShuffle.shuffle_buf[swap_index];
        pShuffle.shuffle_buf[swap_index] = tmp;
    }
    //printf("pShuffle.cur_index is %d \n", pShuffle.cur_index);
    ret = pShuffle.shuffle_buf[pShuffle.cur_index];
    //printf("ret is %d \n", ret);
    return ret;
}

/******************************************************************************************/
/*!
* \par Description:
*        取下一个index
* \param[in]    void
* \retval       正确返回0，错误返回-1，SHUFFLE_MODE下遍历结束也返回-1
*******************************************************************************************/
int shuffle_next(void)
{
    int ret = 0;
    
    ret = shuffle_sig_next();
    if(ret < 0)
    {
        return -1;
    }
    //printf("pShuffle.cur_offset is %d \n", pShuffle.cur_offset);
    return pShuffle.cur_offset + ret;
}

/*****************************************************************************************/
/*!
* \par Description:
*        取上一个index
* \param[in]    void
* \retval       正确返回0，错误返回-1，SHUFFLE_MODE下遍历结束返回-1，最多可以记忆3008个
******************************************************************************************/
int shuffle_prev(void)
{
    int tmp = 0;
    if(pShuffle.cur_index > pShuffle.per_count)
    {
        pShuffle.per_count++;
        return pShuffle.cur_offset + pShuffle.shuffle_buf[pShuffle.cur_index - pShuffle.per_count];
    }
    else
    {

        if(pShuffle.shuffle_mod == LOOP_MODE_SHUFFLE)
        {
            /*随机上边界直接返回-1*/            
            tmp = -1;
        }
        else if(pShuffle.shuffle_mod == LOOP_MODE_SHUFFLE_REPEAT)
        {
            //ignore qac bug
            #pragma __PRQA_IGNORE_START__
            
            tmp = rand() % pShuffle.total_files;
            
            #pragma __PRQA_IGNORE_END__
        }
        else
        {
        }
        
        return tmp;
    }
}

/*****************************************************************************************/
/*!
* \par Description:
         删除的歌曲时调用，保证shuffle表和playlist相对应
* \param[in]    del_index -- 要删除的歌曲index
* \retval       返回当前index
*****************************************************************************************/
int shuffle_del(int del_index)
{
    int tmp;
    int retval;
    int idx;
    int del_id = 0;
    
    /*总数减1*/
    pShuffle.total_files--;
    /*如果删除曲目在本组之前，当然组偏移减1，即本组歌曲左移一首*/
    if(pShuffle.cur_offset > del_index)
    {
        pShuffle.cur_offset--;
    }
    /*如果删除曲目在本组内，则总数减1，当然组偏移减1，即本组歌曲左移一首*/
    else if(del_index < (pShuffle.cur_offset + pShuffle.cur_total))
    {
        idx = del_index - pShuffle.cur_offset;    
        for(tmp=0; tmp < pShuffle.cur_total; tmp++)
        {
            //大于del_index的元素减1, 保证和文件对应
            if(idx < pShuffle.shuffle_buf[tmp])
            {
                pShuffle.shuffle_buf[tmp] = pShuffle.shuffle_buf[tmp] - 1; 
            }
            else if(idx == pShuffle.shuffle_buf[tmp])
            {
                //找到del_index对应数据的偏移
                del_id = tmp;
            }
            else
            {
            }
        }
        //将数组最后1个元素替换当前元素
        pShuffle.shuffle_buf[del_id] = pShuffle.shuffle_buf[pShuffle.cur_total - 1];

        if(pShuffle.cur_total > 1)
        {
            pShuffle.cur_total--;            
            if(pShuffle.cur_index >= pShuffle.cur_total)
            {
                pShuffle.cur_index = pShuffle.cur_total -1;
                if(pShuffle.per_count > 0)
                {
                    pShuffle.per_count--;
                }
            }
        }
        else
        {
            pShuffle.cur_total = 0;
            pShuffle.cur_index = 0;
            pShuffle.per_count = 0;
            return pShuffle.cur_offset;
        }
            
    }
    else
    {
    }
    retval = pShuffle.cur_offset + pShuffle.shuffle_buf[pShuffle.cur_index - pShuffle.per_count];
    
    return retval;
}

