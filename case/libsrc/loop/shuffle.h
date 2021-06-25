/********************************************************************************
 *                              USDK(1100)
 *                             Module: lib 
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 * History:         
 *      <author>    <time>           <version >             <desc>
 *                                      v1.0               build this file 
 *      shaofeng    2009-05-17          v1.1               �Ż�loopmode
 *      shaofeng    2009-05-18          v1.2               ����case����淶�������
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

//������� 3008 �׸�� shuffle
#define      SHUFFLE_BUFFER_MAX       0xBC0


typedef struct
{
    int total_files ;   //shuffle �����ֵ��ÿ������� [0��shuffle_range-1]
    int cur_total ;     //��ǰ����ļ����������ΪSHUFFLE_BUFFER_MAX
    int cur_index ;     //��ǰshuffle_buf��λ��
    int cur_offset ;    //��ǰ���ڸ�����ƫ�ƣ�һ��ΪSHUFFLE_BUFFER_MAX ��������
    int start_offset;   //��һ�������ƫ�ƣ����� LOOP_MODE_SHUFFLE_REPEAT �ж�һ��shuffle����
    int per_count ;     //���˵ĸ��������ڻ��˺������֮ǰ��˳�򲥷�
    int shuffle_buf[SHUFFLE_BUFFER_MAX];     //�洢��ǰ��ĸ���˳��С��cur_indexΪ�Ѳ�������Ϊδ��
    char shuffle_mod;     //��ΪSHUFFLE_MODE �� SHUFFLE_REPEAT_MODE��ǰ��ֻ��һ�飬����ѭ������
}shuffle_t;




/* shuffle �ڲ���ʼ�� */
extern bool shuffle_init(void);

/*
 �趨shuffle��������ָ��һ����ǰ��(���ڲ��ŵĸ���)��
 INT32U total_files, ���ļ�������Ϊ����ֵ�����Ϊ4294967295
 INT32U cur_index,   ��ǰ���ڲ��ŵĸ����ڲ����б��index
 INT8U mode          shuffle ģʽ
*/
extern bool shuffle(int total_files, int cur_index, char mode);

/* ȡ����һ�׸�������*/
extern int shuffle_next(void);

/* ȡ����һ�׸�������  */
extern int shuffle_prev(void);

/* ɾ�������б���del_index �ĸ�����shuffleģ�鲻��Բ����б�����κβ�����
ֻ�ǰ��µĲ����б����������ڲ����ݽṹ*/
extern int shuffle_del(int del_index);

/*! \endcond*/
#endif  /* __SHUFFLE_H__ */

