/*
*******************************************************************************
*
*                       NOYA1100 ---style�����
*                (c) Copyright 2002-2008, Actions Co,Ld. 
*                        All Right Reserved 
*
* FileName: global.h     Author:  zhaowenjie        Date:2008/12/16
* Description:    style ������ڲ�ʹ�õ�ȫ����Ϣ��
* Others:         
* History: 
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         �������ļ���
*******************************************************************************
*
*/ 

#ifndef __STYLE_COMMON_H__
#define __STYLE_COMMON_H__

//#define DEBUG	/* �Ƿ����������Ϣ�Ŀ��� */
//	#define WINDOWS

#ifdef WINDOWS

#define TRUE  1
#define FALSE  0
#define NULL 0

#endif  /* WINDOWS  */


#if PRINT_DBG  == 1

#define PRINTF printf

#else

#define PRINTF(...) do {} while (0)

#endif /* DEBUG_PRINT */


#define SUCCESS  0
#define FAILURE    -1

#define KEY_COUNT	  16

#endif /* __STYLE_COMMON_H__ */


