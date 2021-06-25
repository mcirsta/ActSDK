/*
 ********************************************************************************
 *           NOYA(MAYA)---shuffle
 *        (c) Copyright 2002-2006, Actions Co,Ld.
 *            All Right Reserved
 *
 * FileName: shuffle.c     Author:  yangyongdong        Date:2007/06/14
 * Description:
 1. ������shuffle������ʵ��
 * Others:     // �������ݵ�˵��
 * History:    // �޸���ʷ��¼�б�
 *      <author>    <time>      <version >    <desc>
 *     yyd          2007/12/24     0.1        build this file
 *     yyd          2007/12/24     0.2        �޸�shuffle_delʵ��
 *     yyd          2008/02/24     1.0        �޸�һЩ���ص�bug
 *     yyd          2008/08/04     1.1        ��д���ִ��룬ͨ��QAC��PC-lint���
 *     xsz          2008/09/03     2.0        �޸Ĳ��ִ���
 *     shaofeng     2009-05-17     v2.1       �Ż�loopmode
 *     shaofeng     2009-05-18     v2.2       ����case����淶�������
 ********************************************************************************
 */

#include "include_case.h"


/* ��Ӧ�õ�ͷ�ļ� */
#include "shuffle.h"



static shuffle_t pShuffle;


static void _set_cur_total(int num);
static bool _check_index(void);
static bool _modify_index(int cur_index);

#if 0
#endif
/******************************************************************
**   �㷨ԭ��
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
*        ���õ�ǰ������ļ���Ϊnum��
*        num����С��SHUFFLE_BUFFER_MAX��
*        ��num����һ����Ŀ��ͬʱ���������ҵ�ǰ��
* \param[in]    num -- total number
* \retval       0 Ϊ���óɹ��� -1Ϊʧ��
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
*        ��鱾���Ƿ�������������������һ��
* \param[in]    void  
* \retval       true: Ϊ���óɹ��� false: Ϊʧ��
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
*        �趨��ǰ���Ÿ���index
* \param[in]    cur_index -- Ϊ���ڲ��ŵ�index
* \retval       true: Ϊ���óɹ��� false: Ϊʧ��
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
*        �������������ó�ʼֵ����Shuffle_Init֮����û�������ļ�����(total_files)��
*        ��ǰ������cur_index����Shuffleģʽ��mode������á�
* \param[in]    total_files -- ��ҪShuffle���ļ�����
* \param[in]    cur_index   -- ��ǰ����index
* \param[in]    mode��ShuffleģʽSHUFFLE_MODE��SHUFFLE_REPEAT_MODE
* \retval       ��ȷΪtrue������Ϊfalse
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
*        shuffle_next,���������ƫ��
* \param[in]    void
* \retval       SHUFFLE_MODE�±�����������-1, ������������ƫ��
*******************************************************************************************/
int shuffle_sig_next(void)
{
    int swap_index;
    int tmp;
    int ret;
    /*����л��ˣ��Ȱ��ջ���˳�򲥷�*/
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
    /**���cur_index���ж��Ƿ����������Ҫ����*/
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
*        ȡ��һ��index
* \param[in]    void
* \retval       ��ȷ����0�����󷵻�-1��SHUFFLE_MODE�±�������Ҳ����-1
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
*        ȡ��һ��index
* \param[in]    void
* \retval       ��ȷ����0�����󷵻�-1��SHUFFLE_MODE�±�����������-1�������Լ���3008��
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
            /*����ϱ߽�ֱ�ӷ���-1*/            
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
         ɾ���ĸ���ʱ���ã���֤shuffle���playlist���Ӧ
* \param[in]    del_index -- Ҫɾ���ĸ���index
* \retval       ���ص�ǰindex
*****************************************************************************************/
int shuffle_del(int del_index)
{
    int tmp;
    int retval;
    int idx;
    int del_id = 0;
    
    /*������1*/
    pShuffle.total_files--;
    /*���ɾ����Ŀ�ڱ���֮ǰ����Ȼ��ƫ�Ƽ�1���������������һ��*/
    if(pShuffle.cur_offset > del_index)
    {
        pShuffle.cur_offset--;
    }
    /*���ɾ����Ŀ�ڱ����ڣ���������1����Ȼ��ƫ�Ƽ�1���������������һ��*/
    else if(del_index < (pShuffle.cur_offset + pShuffle.cur_total))
    {
        idx = del_index - pShuffle.cur_offset;    
        for(tmp=0; tmp < pShuffle.cur_total; tmp++)
        {
            //����del_index��Ԫ�ؼ�1, ��֤���ļ���Ӧ
            if(idx < pShuffle.shuffle_buf[tmp])
            {
                pShuffle.shuffle_buf[tmp] = pShuffle.shuffle_buf[tmp] - 1; 
            }
            else if(idx == pShuffle.shuffle_buf[tmp])
            {
                //�ҵ�del_index��Ӧ���ݵ�ƫ��
                del_id = tmp;
            }
            else
            {
            }
        }
        //���������1��Ԫ���滻��ǰԪ��
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

