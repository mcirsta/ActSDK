#ifndef _EFFECT_H
#define _EFFECT_H

#ifdef WIN32
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#endif


typedef int PFreal;
#define PFREAL_SHIFT 10

#define IANGLE_MAX 1024
#define IANGLE_MASK 1023

#ifndef NULL
#define NULL 0
#endif
//---------- api-----------------
  
typedef enum
{
	TRANS_PAGE,		//��ҳ
	TRANS_SMOOTHMOVE,	//
	TRANS_RANDOMLINE, //�������
	TRANS_LOUVER,		//�촰
	TRANS_ERASE,      //��ȥ
	TRANS_CUT,		//�ָ�
	TRANS_CHECK,		//����
	TRANS_CUBE,	    //������
	TRANS_WAVE,		//����
	TRANS_MOVE,		//����
	TRANS_REVERSE,	//��ת
	TRANS_PPT,		//����Ƭ
	TRANS_FADE,		//���뵭��
	TRANS_ZOOM,		//��ͨ����
	TRANS_FADEZOOM,	//����,�Ŵ�
	TRANS_ROTATIONZOOM,
	TRANS_ROTATION_2D, //other
	TRANS_POPUP,		//��������
	TRANS_POPUPALPHA,
	TRANS_MOVEALPHA		//�����������alpha
}trans_mode_t;

typedef enum
{
	TRANS_UP,			//������
	TRANS_DOWN,       //������
	TRANS_LEFT,		//��������
	TRANS_RIGHT,		//��������
}trans_direct_t;

#ifdef I32
#undef I32
#endif
#ifdef I16
#undef I16
#endif
#ifdef I8
#undef I8
#endif
#ifdef U8
#undef U8
#endif
#ifdef U16
#undef U16
#endif
#ifdef U32
#undef U32
#endif
typedef signed long I32;
typedef short I16;
typedef char I8;
typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int  U32;

typedef struct 
{
	I32  xview;       //���buf��,��ʼλ��Ϊ��Ļ���Ͻ�
	I32  yview;		  //���buf��	
	I32  xpos;        //�����x��ʼλ��
	I32  ypos;        //�����y��ʼλ��
}coordinate_t;

typedef struct 
{
	trans_mode_t mode;			//ģ��trans_mode_t
	U8 *srcbuf;			//Դͼ��
	U8 *desbuf;			//Ŀ��ͼ��
	I32  width;			//��4����
	I32  height;		//��
	I32  totalframes;	//��֡�� 1 - 10			
	I32  currentframe;  //��ǰ֡��
	trans_direct_t  directflag;	//�˶����� trans_direct_t	
	I32 alpha;          //��ת�Ƕȣ���popupʱΪ͸������ 1 - 9
	U8 *bufout;			//���buf
	coordinate_t viewparam;    //���ͼ��λ��coordinate_t
}transval_t;

I32 transimage(transval_t *transval);

//------------------------------------*/
#endif

