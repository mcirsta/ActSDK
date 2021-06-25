/********************************************************************************
*                              usdk130
*                            Module: key driver
*                 Copyright(c) 2001-2007 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*       xkping     2008-02-01 15:00     1.0             build this file 
********************************************************************************/
/*!
 * \file   key_buf_queue.c
 * \brief  key scan support parallel and searial mode,but,in this driver,we only support parallel mode 4*4
 * \author xkping
 * \par GENERAL DESCRIPTION:
 *       This file has follwing functions: 
 *      1) maintain the buffer of key,FIFO    
 * \par EXTERNALIZED FUNCTIONS:
 *       no
 *
 *     Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 *  \version 1.0
 *  \date  2007/5/4
 *******************************************************************************/


#include "key_buf_queue.h"
#include "key_drv.h"
#include "key_sys.h"



unsigned int key_value[KEY_MESSAGE_BUF_SIZE]={0};
unsigned int spos = 0;
unsigned int rpos = 0;
unsigned int cnt = 0;


/* Store key value in the queue. */

/******************************************************************************/
/*!                    
* \par  Description:
*     Store ints in the queue.
* \param[in]   unsigned int q  ,a key output value
* \param[out]  none       
* \return      none 
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/
void qstore( unsigned int q )
{   
    if ( 1 == is_full() )
    {
        return;
    }        
    key_value[spos] = q;
  
    spos++;
    if( spos==KEY_MESSAGE_BUF_SIZE ) 
    {
        spos = 0; /* loop back */
    }
    cnt++;
    return ;    

}

/******************************************************************************/
/*!                    
* \par  Description:
*     Store up value in the queue.
* \param[in]   unsigned int q  ,a key output value
* \param[out]  none       
* \return      none 
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/



void qstore_up( unsigned int q )
{   
    if ( 1 == is_full() )
    {
        
        if (0 == spos)
        {
            key_value[KEY_MESSAGE_BUF_SIZE - 1] = q;   
        }
        else
        {
            key_value[spos - 1] = q;
        }
    }        
    else
    {
        key_value[spos] = q;
        spos++;
        if( spos==KEY_MESSAGE_BUF_SIZE ) 
        {
            spos = 0; /* loop back */
        }
        cnt++;
    }    

}



/******************************************************************************/
/*!                    
* \par  Description:
*     Retrieve an element.
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      0  ,buffer is empty ,do not need to Retrieve,
* \retval      other, value of Retrieved element
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/
unsigned int  qretrieve(void)
{
    unsigned int value;
  
    if( is_empty() ==1 ) 
    {
        return 0;
    }
    value =  key_value[rpos];
    rpos++;
    if(rpos == KEY_MESSAGE_BUF_SIZE)  
    {
        rpos = 0; /* loop back */  
    }
    cnt--;
  
    
    return value;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     judge the buffer is empty
* \param[in]   none
* \param[out]  none       
* \return      rezult 
* \retval      0 not empty 
* \retval      1 is emtpy 
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/
int is_empty(void)
{
    if( cnt == 0 ) 
    {
        spos = 0;
        rpos = 0;
        return 1;
    }

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     judge the buffer is full
* \param[in]   none
* \param[out]  none       
* \return      rezult 
* \retval      0 not full 
* \retval      1 is full 
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/

int is_full(void)
{
    if (cnt >= KEY_MESSAGE_BUF_SIZE ) 
    {
        return 1;       
    }
    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      return the element number of buffer
* \param[in]   none
* \param[out]  none       
* \return      the result  
* \retval      the number of buffer
* \par      exmaple code
* \code 
*           list example code in here
* \endcode
*******************************************************************************/

unsigned int sizeof_key_buf(void)
{
    return cnt;
}

