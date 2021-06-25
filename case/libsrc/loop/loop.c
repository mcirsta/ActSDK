/********************************************************************************
 *                              USDK(1100)
 *                             Module: lib 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *      xusongzhou  2008-07-16          v1.0               build this file 
 *      shaofeng    2009-05-17          v1.1               优化loopmode
 *      shaofeng    2009-05-18          v1.2               按照case代码规范整理代码
 ********************************************************************************/
 /*!
 * \file   loop.c
 * \brief  
 *       模块接口
 * \author xusongzhou
 * \par GENERAL DESCRIPTION:
 *       详细说明"循环模式库模块"的总体框架和接口定义，介绍该模块的功能和使用方法
 * \par EXTERNALIZED FUNCTIONS:
 *       底层支持：
 *       该模块底层调用了SHUFFLE模块来实现SHUFFLE循环模式（参见参考文献）。
 *        向上提供服务：
 *       向应用提供6种循环模式。当应用需要循环控制播放时，可能包含并调用该循环模式库。
 *
 *
 *  Copyright(c) 2003 - 2008 Actions Semiconductor, All Rights Reserved.
 *
 * \version 1.0
 * \date  2008/07/08
 *******************************************************************************/

#include "include_case.h"


/* 本应用的头文件 */
#include "loop.h"
#include "shuffle.h"



static loop_t loop;

static int _loop_get_next_normal( void );
static int _loop_get_next_forceloop( void );
static int _loop_get_prev_forceloop( void );
static int _loop_delete_current_normal( void );
static int _loop_delete_current_forceloop( loop_direct_e dir );

/* auto next */
static int _loop_get_next_normal( void )
{
    char stop_flag = 0;
    int index_next = 0;
        
    //printf("loop mode is %d\n", loop.loop_mode);
    switch(loop.loop_mode)
    {
    case LOOP_MODE_ONLY_ONE:
        stop_flag = 1;
        break;
        
    case LOOP_MODE_REPEAT_ONE:
        break;
        
    case LOOP_MODE_SEQUENCE:
        if(loop.current >= (loop.total - 1))
        {
            stop_flag = 1;
        }
        else
        {
            loop.current++;
        }
        break;
        
    case LOOP_MODE_REPEAT_ALL:
        if(loop.current >= (loop.total - 1))
        {
            loop.current = 0;
        }
        else
        {
            loop.current++;
        }
        break;
        
    case LOOP_MODE_SHUFFLE:
    case LOOP_MODE_SHUFFLE_REPEAT:
        index_next = shuffle_next();
        if(index_next == -1)
        {
            stop_flag = 1;
        }
        else
        {
            loop.current = index_next;
        }
        break;
        
    default:
        stop_flag = 1;
        break;
    }
    
    if(loop.current >= loop.total)
    {
        loop.current = loop.total -1;
    }
    
    if(stop_flag == 1)
    {
        return -1;
    }
    else
    {
        return loop.current;
    }
}

int _loop_get_next_forceloop( void )
{
    char stop_flag = 0;
    int index_next = 0;
    bool bret = false;
    
    //printf("loop mode is %d\n", loop.loop_mode);
    switch(loop.loop_mode)
    {
    case LOOP_MODE_ONLY_ONE:
    case LOOP_MODE_REPEAT_ONE:
    case LOOP_MODE_SEQUENCE:
    case LOOP_MODE_REPEAT_ALL:
        if(loop.current >= (loop.total - 1))
        {
            loop.current = 0;
        }
        else
        {
            loop.current++;
        }
        break;
        
    case LOOP_MODE_SHUFFLE:
    case LOOP_MODE_SHUFFLE_REPEAT:
        index_next = shuffle_next();
        if(index_next == -1)
        {
            //ignore qac bug
            #pragma __PRQA_IGNORE_START__
            
            //get random index
            index_next = rand() % loop.total;
            
            #pragma __PRQA_IGNORE_END__
            
            //reinit shuffle using random current
            bret = shuffle(loop.total, index_next, (char)loop.loop_mode);
            if(bret == false)
            {
                stop_flag = 1;  //shuffle fail
            }
            else
            {
                loop.current = index_next;
            }
        }
        else
        {
            loop.current = index_next;
        }
        break;
        
    default:
        stop_flag = 1;
        break;
    }
    if(loop.current >= loop.total)
    {
        loop.current = loop.total -1;
    }
    
    if(stop_flag == 1)
    {
        return -1;
    }
    else
    {
        return loop.current;
    }
}

int _loop_get_prev_forceloop( void )
{
    char stop_flag = 0;
    int index_prev = 0;
    
    switch(loop.loop_mode)
    {
    case LOOP_MODE_ONLY_ONE:
    case LOOP_MODE_REPEAT_ONE:
    case LOOP_MODE_SEQUENCE:
    case LOOP_MODE_REPEAT_ALL:
        if(loop.current > 0)
        {
            loop.current--;
        }
        else
        {
            loop.current = loop.total - 1;
        }
        break;
        
    case LOOP_MODE_SHUFFLE:
    case LOOP_MODE_SHUFFLE_REPEAT:
        index_prev = shuffle_prev();
        if(index_prev == -1)
        {
            stop_flag = 1;
        }
        else
        {
            loop.current = index_prev;
        }
        break;
        
    default:
        stop_flag = 1;
        break;
    }
    
    if(loop.current >= loop.total)
    {
        loop.current = loop.total -1;
    }
    
    if(stop_flag == 1)
    {
        return -1;
    }
    else
    {
        return loop.current;
    }
}

static int _loop_delete_current_normal( void )
{
    char stop_flag = 0;
    
    switch(loop.loop_mode)
    {
    case LOOP_MODE_ONLY_ONE:
    case LOOP_MODE_SEQUENCE:
        if(loop.current >= loop.total) //delete last one
        {
            stop_flag = 1;  //stop
            loop.current = loop.total -1; //go to new last one
        }
        break;

    case LOOP_MODE_REPEAT_ONE:
    case LOOP_MODE_REPEAT_ALL:
        if(loop.current >= loop.total) //delete last one
        {
            loop.current = 0; //go to first one
        }
        break;
            
    case LOOP_MODE_SHUFFLE:        
    case LOOP_MODE_SHUFFLE_REPEAT:
        stop_flag = 1;
        break;
        
    default:
        stop_flag = 1;
        break;
    }

    if(stop_flag == 1)
    {
        return -1;
    }
    else
    {
        return loop.current;
    }
}

static int _loop_delete_current_forceloop( loop_direct_e dir )
{
    char stop_flag = 0;
    
    switch(loop.loop_mode)
    {
    case LOOP_MODE_ONLY_ONE:
    case LOOP_MODE_SEQUENCE:
    case LOOP_MODE_REPEAT_ONE:
    case LOOP_MODE_REPEAT_ALL:
        switch(dir)
        {            
        case LOOP_DIRECT_NEXT:  
            if(loop.current >= loop.total) //delete last one
            {
                loop.current = 0;  //go to first one
            }
            break;
            
        case LOOP_DIRECT_PREV:
            if(loop.current == 0) //delete frist one
            {
                loop.current = loop.total - 1;  //go to last one
            }
            else
            {
                loop.current--; //
            }
            break;
            
        default:
            break;
        }
        break;
            
    case LOOP_MODE_SHUFFLE:        
    case LOOP_MODE_SHUFFLE_REPEAT:
        stop_flag = 1;
        break;
        
    default:
        stop_flag = 1;
        break;
    }

    if(stop_flag == 1)
    {
        return -1;
    }
    else
    {
        return loop.current;
    }
}

/*! \cond loop_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*        initialize the "loop library"
* \param[in]    void
* \retval       always return true
* \code
Main()
{
    int retv;
    
    // initialization
    loop_init();
    loop_set_mode(LOOP_MODE_SEQUENCE, 5, 10);
}
* \endcode
*******************************************************************************/
bool loop_init(void)
{
    loop.current = 0;
    loop.total = 0;
    loop.loop_mode = LOOP_MODE_REPEAT_ALL;
    
    shuffle_init();
    
    return true;
}
/*! \endcond*/


/*! \cond loop_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*        Set the loop mode. you may call it when you need to set or change the loop mode.
*        It should be called before "loop_get_next", "loop_get_prev" and "loop_delete_index"
* \param[in]    loop_mode_e mode --the loop mode
* \param[in]    int current --the current index
* \param[in]    int total --the total number
* \retval       
* \code
Main()
{
    int retv;
    loop_init();

    // set the mode, index and total files
    loop_set_mode(LOOP_MODE_SEQUENCE, 5, 10);
}
* \endcode
*******************************************************************************/
bool loop_set_mode(loop_mode_e mode, int current, int total)
{
    bool result = true;
    
    if(total <= 0)
    {
        memset((void *)&loop, 0, sizeof(loop_t));
        return false;
    }
    
    if((current >= total)||(current < 0))
    {
        current = total - 1;
    }
    
    loop.loop_mode = mode;
    loop.current = current;
    loop.total = total;
    if((loop.loop_mode == LOOP_MODE_SHUFFLE)||(loop.loop_mode == LOOP_MODE_SHUFFLE_REPEAT))
    {
        result = shuffle(loop.total, loop.current, (char)loop.loop_mode);
    }
    
    return result;
}
/*! \endcond*/

/*! \cond loop_API*/
/********************************************************************************/
/*!
* \par  Description: Get the current index.
* \param[in]  none
* \retval   loop.current --the next index
* \Others    :
*********************************************************************************/
int loop_get_current(void)
{
    return loop.current;
}
/*! \endcond*/

/*! \cond loop_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*        get the next index.
* \param[in]    loop_switch_e smode 
* \retval       -1 --loop come to the end
* \retval       loop.current --the next index
* \code
Main()
{
    int index;
    loop_init();

    // set the mode, index and total files
    loop_set_mode(LOOP_MODE_SEQUENCE, 5, 10);
        
    // get the next index
    index = loop_get_next();
    if(index == (INT32U)(-1))
    {
        // reset loop
    }
}
* \endcode
*******************************************************************************/
int loop_get_next( loop_switch_e smode )
{
    int result;
    
    if(loop.total <= 0)
    {
        return -1;
    }
    
    switch(smode)
    {
    case LOOP_SWITCH_NORMAL:
        result = _loop_get_next_normal();
        break;
        
    case LOOP_SWITCH_FORCELOOP:
        result = _loop_get_next_forceloop();
        break;
        
    default:
        result = -1;
        break;
    }

    return result;
}
/*! \endcond*/


/*! \cond loop_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*        get the previous index.
* \param[in]    loop_switch_e smode
* \retval       -1 --loop come to the end
* \retval       loop.current --the previous index
* \code
Main()
{
    int index;
    loop_init();

    // set the mode, index and total files
    loop_set_mode(LOOP_MODE_SEQUENCE, 5, 10);
        
    // get the previous index
    index = loop_get_prev();
    if(index == (INT32U)(-1))
    {
        // reset loop
    }
}
* \endcode
*******************************************************************************/
int loop_get_prev(loop_switch_e smode)
{
    int result;
    
    if(loop.total <= 0)
    {
        return -1;
    }
    
    switch(smode)
    {
    case LOOP_SWITCH_NORMAL:
        result = -1;
        break;
        
    case LOOP_SWITCH_FORCELOOP:
        result = _loop_get_prev_forceloop();
        break;
        
    default:
        result = -1;
        break;
    }

    return result;    
}
/*! \endcond*/


/*! \cond loop_API*/
/******************************************************************************/
/*!                    
* \par  Description:
*        delete the index.
* \param[in]    del_index -- the "delete" index
* \param[in]    loop_switch_e smode
* \param[in]    bool is_prev -- true: play prev file after delete; false: play next file
* \retval       -1 -- the total number is zero
* \retval       loop.current -- current index
* \code
Main()
{
    INT32U index;
    loop_init();

    // set the mode, index and total files
    loop_set_mode(LOOP_MODE_SEQUENCE, 5, 10);
        
    // delete index
    index = loop_delete_index(4);
    if(index == (INT32U)(-1))
    {
        // reset loop
    }
}
* \endcode
*******************************************************************************/
int loop_delete_index( int del_index, loop_switch_e smode, loop_direct_e dir )
{
    int result;
    
    if(loop.total <= 1)
    {
        loop.total = 0;
        return -1;
    }
    
    if(del_index >= loop.total)
    {
        return -1;
    }

    //total dec
    loop.total--;

    //shuffle delete
    if((loop.loop_mode == LOOP_MODE_SHUFFLE)||(loop.loop_mode == LOOP_MODE_SHUFFLE_REPEAT))
    {
        loop.current = shuffle_del(del_index);
        return loop.current;
    }
    
    //other delete
    
    //delete item is not current item
    if(del_index != loop.current)
    {    
        if(del_index < loop.current)
        {
            loop.current--;
        }
        
        return loop.current;
    }
    else
    {    
        //delete item is current item
        switch(smode)
        {
        case LOOP_SWITCH_NORMAL:
            result = _loop_delete_current_normal();
            break;
            
        case LOOP_SWITCH_FORCELOOP:
            result = _loop_delete_current_forceloop( dir );
            break;
            
        default:
            result = -1;
            break;
        }

        return result; 
    }
}
/*! \endcond*/

