
/*
*******************************************************************************
*
*                       NOYA1100 ---style�����
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* FileName: style.h     Author:  zhaowenjie        Date:2008/12/16
* Description:    style ������ṩ���ⲿʹ�õĽӿڡ�
* Others:
* History:
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         �������ļ���
*         zhaowenjie      2008/12/16     1.1         �������Ϊ����1100 ���������
*******************************************************************************
*
*/

/********************************************************************************
*                              NOYA1100 ---style�����
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         �������ļ���
*         zhaowenjie      2008/12/16     1.1         �������Ϊ����1100 ���������
********************************************************************************/
/*!
* \file     style.h
* \brief    ��style ������ṩ��Ӧ�ó���ʹ�õĽӿڡ�
* \author   zhaowenjie
*               
* \version 1.0
* \date  2009/1/14
*******************************************************************************/

#ifndef __STYLE_H__
#define __STYLE_H__

#ifdef _MSC_VER
#define PACK  // 1��ʾ1�ֽڶ���
#else
#define PACK __attribute__ ((packed))
#endif

/*��Դ������*/
/*! ͼƬ*/
#define	RESOURCE_PICREGION 1
/*! �ַ���*/
#define	RESOURCE_STRING 2
/*!  ��Դ��*/
#define RESOURCE_RESGROUP 3
/*!  ��Դ��*/
#define RESOURCE_PICTURE 4

#ifdef _MSC_VER
#pragma pack(1)
#endif

typedef struct style_s
{
    int file;
    /*! scene ������ */
    unsigned int sum; 
    /*! scene ������ */
    int* scenes;
}PACK
style_t;


/*
  * �ڳ�����ʹ�õ�resource ��ʽ����������Դ�ṹ�Ĺ��в���
  * Ϊ���ܰѲ�ͬ����Դ�ṹ����һ������
*/

/*!
*\brief
	��Դ�Ļ����ṹ
*/
typedef struct resource_s
{
    /*!��Դ������*/
    unsigned char type;		
    /*! ����Դ��ID */
    unsigned int  	 id;						
    /*! ָ����һ����Դ*/
    unsigned int* offset;		
}PACK
resource_t;

/*!
*\brief
	��Դ��Ľṹ
*/
typedef struct
{
    /*!��Դ�Ļ�����Ϣ*/
    resource_t   	 resource;

    /*! ����Ļ�ľ��Ժ�����  */
    short  absolute_x;
    /*! ����Ļ�ľ���������  */
    short  absolute_y;
    /*! �ڸ����еĺ�����  */
    short  x;
    /*! �ڸ����е�������  */
    short  y;
    
    /*! ���  */
    short  width;
    /*! �߶�  */
    short  height;
    /*! ����ɫ  */
    unsigned int	 background;
        /*! �Ƿ�ɼ�  */
    unsigned char  visible;
    /*! �Ƿ�͸�� */
    unsigned char   opaque;
    /*! ͸����  */
    unsigned short  transparence;

    /*! ��resgroup ����Դ����Ŀ*/
    unsigned int  	 resource_sum;
    /*! ��resgroup ����Դ��ƫ��*/
    unsigned int child_offset;	
}PACK
resgroup_resource_t;


/*!
*\brief
	ͼƬ������Դ�� �ṹ
*/
typedef struct
{
    /*! ��Դ�Ļ����ṹ*/
    resource_t   	 resource;
    /*! �ڸ����еĺ�����*/
    short  x;
    /*! �ڸ����е�������*/
    short  y;
    /*! ���*/
    short  width;
    /*! �߶�*/
    short  height;
    /*! �Ƿ�ɼ�*/
    unsigned char   visible;
    /*! ��ͼƬ������Դӵ�е�ͼ����*/
    unsigned short 	 frames;		

    /*! ��¼��ͼƬ������Դ������ͼ��ͼƬ��ID */
    unsigned int pic_offset;
}PACK
picregion_resource_t;

/*!
*\brief
	ͼƬ������ĳһ������� �ṹ
*/
typedef struct
{
    /*! ��ͼ���ͼƬ������Դ��ID ��*/
    unsigned int  	 id;   
    /*! ��ͼ��Ĳ���*/
    unsigned short  frame;
    /*! ͼ������ͼƬ����ĺ�����*/
    short  x;
    /*!  ͼ������ͼƬ�����������*/
    short  y;
    /*! ͼ������ͼƬ����Ŀ��*/
    short  width;
     /*! ͼ������ͼƬ����ĸ߶�*/
    short  height;
    /*! ��ͼ����ʹ�õ�ͼƬID */
    unsigned short  pic_id;
}picregion_frame_t;



/*!
*\brief
	�ַ�����Դ�� �ṹ
*/
typedef struct
{
    /*! ��Դ�Ļ����ṹ*/
    resource_t   	 resource;
    /*! ���ο��x ���� */
    short  x;     
    /*! ���ο��y ���� */
    short  y;            		
    /*! ���ο�Ŀ�� */
    short  width;	
    /*! ���ο�ĸ߶� */
    short  height;	
    /*! �������ɫ */
    unsigned int  	 foreground;	
    /*! ���ο�ı���ɫ*/
    unsigned int	 background;
    /*! �Ƿ�ɼ� */
    unsigned char   visible;
    /*! �����ھ��ο��еĶ��뷽ʽ */
    unsigned char   text_align;
    /*! �����ģʽ */
    unsigned char   mode;		
    /*!����Ĵ�С*/
    unsigned char 	 font_height;	
    	
    		
    /*! �ַ�������ԴID  */
    unsigned short str_id;			
    /*! �Ƿ����: 1 ��ʾ������0 ��ʾ������ */
    unsigned char scroll;		
    /*! �����ķ���: -1 ��ʾ���ҵ���0 ��ʾ������ */
    char direction;	
    /*! ���������򣬵�λ������ */
    unsigned short space;			
    /*! �����Ĳ���,��λ������ */
    unsigned short pixel;			
}PACK
string_resource_t;


/*!
*\brief
	ͼƬ��Դ�� �ṹ
*/
typedef struct
{
     /*! ��Դ�Ļ����ṹ*/
    resource_t   	 resource;
    short  x;
    short  y;
    short  width;
    short  height;
    unsigned char  visible;
    unsigned short    pic_id;
}PACK
picture_resource_t;

/*!
*\brief
	������ �ṹ
*/
typedef struct
{	      
    /*! ������  */
    short  x;
    /*! ������  */
    short  y;
    /*! ���  */
    short  width;
    /*! �߶�  */
    short  height;
    /*! ����ɫ  */
    unsigned int background;     
    /*! scene �Ƿ�ɼ�  */
    unsigned char  visible;
    /*! scene �Ƿ�͸�� */
    unsigned char   opaque;
    /*! scene ��͸����  */
    unsigned short  transparence;
    
    /*! ��scene �к�����Դ����Ŀ*/
    unsigned int resource_sum;
    /*! ���ӵ�ƫ���� */
    unsigned int child_offset;
    /*! ��������*/
    unsigned char direction;
    
    unsigned char keys[ 16 ];
}PACK
scene_t;

#ifdef _MSC_VER
#pragma pack()
#endif

/******************************************************************************/
/*!                    
* \par  Description:
*      �� sty �ļ���
* \param[in]    filepath -- ��sty �ļ���·����
* \retval       ��style ��ָ���0( û�ҵ� )��
*\par example code
*\code
*	style_t* style = open_style_file( "music.sty" );
* \endcode
*******************************************************************************/
style_t* open_style_file( const unsigned char *filepath );

/******************************************************************************/
/*!                    
* \par  Description:
*      �ر� sty �ļ���
* \param[in]    style -- �����ļ�����Ľṹ��
*\par example code
*\code
*	close_style_file( style );
* \endcode
*******************************************************************************/
void close_style_file( style_t* style );

/******************************************************************************/
/*!                    
* \par  Description:
*      ��sty �ļ���ȡ��ָ��ID ��scene  ��
* \param[in]    filepath -- ��sty �ļ���·����
* \param[in]    id -- ��scene ��ID��
* \retval       ��scene ��ָ���0( û�ҵ� )��
*\par example code
*\code
*	scene_t* scene = load_scene( "music.sty" , 1 );
* \endcode
*******************************************************************************/
scene_t* load_scene( style_t* style, const unsigned id );

/******************************************************************************/
/*!                    
* \par  Description:
*      �ͷ� scene ��Դ  ��
* \param[in]    scene -- Ҫ�ͷŵ� scene ��
*\par example code
*\code
*	free_scene( scene );
* \endcode
*******************************************************************************/
void free_scene( scene_t* scene );

/******************************************************************************/
/*!                    
* \par  Description:
*      ��sty �ļ���ȡ��ָ��ID ��scene  ��
* \param[in]    filepath -- ��sty �ļ���·����
* \param[in]    id -- ��scene ��ID��
* \retval       ��scene ��ָ���0( û�ҵ� )��
*\par example code
*\code
*	scene_t* scene = load_scene( "music.sty" , 1 );
* \endcode
*******************************************************************************/
scene_t* load_scene( style_t* style, const unsigned id );

/******************************************************************************/
/*!                    
* \par  Description:
*      ��ָ����scene �в���ָ�� �ֲ�ID ����Դ��
* \param[in]    scene -- ��scene ��ָ�롣
* \param[in]    id -- Ҫ���ҵ���Դ��ID��
* \retval      ����Դ��ָ���0( û�ҵ�)��
*\par example code
*\code
*	void* resource = get_scene_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_scene_child( const scene_t* scene, const unsigned int id );

/******************************************************************************/
/*!                    
* \par  Description:
*      ��ָ����resgroup �в���ָ�� �ֲ�ID ����Դ��
* \param[in]    resgroup -- ��resgroup ��ָ�롣
* \param[in]    id -- Ҫ���ҵ���Դ��ID��
* \retval      ����Դ��ָ���0( û�ҵ�)��
*\par example code
*\code
*	void* resource = get_resgroup_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_resgroup_child( const resgroup_resource_t* resgroup, const unsigned int id );

/******************************************************************************/
/*!                    g
* \par  Description:
*      ��picRegion ��ȡ��֡��Ϊframe ��ͼƬ֡�����õ��������pictureframe �С�
* \param[in]    picRegion -- ��picRegion ��ָ�롣
* \param[in]    frame -- ��֡����ţ��� 0 ��ʼ��
* \param[out]    pictureframe -- ���ص�֡�ṹ��
* \retval      0 �ɹ���-1 ʧ�ܡ�
*\par example code
*\code
*	picregion_resource_t* picture = (picregion_resource_t*)get_resource( scene, 2 );
*	picregion_frame_t* pictureframe = (picregion_frame_t* )malloc( sizeof( picregion_frame_t ) );
*	get_picregion_frame( picture, 3, pictureframe );
* \endcode
*******************************************************************************/
int get_picregion_frame( const picregion_resource_t* picRegion , const unsigned short frame , picregion_frame_t* pictureframe );

typedef struct
{
    style_t* (*const open_style_file)( const unsigned char* filepath );
    void (*const close_style_file)( style_t* style );
    scene_t* (*const load_scene ) ( style_t* style, const unsigned id );
    void (*const free_scene ) ( scene_t* scene );
    void* (*const get_scene_child ) ( const scene_t* scene, const unsigned int id );
    void* (*const get_resgroup_child ) ( const resgroup_resource_t* resgroup, const unsigned int id );
    int (*const get_picregion_frame ) ( const picregion_resource_t* picRegion , const unsigned short frame , picregion_frame_t* pictureframe );
}style_api_t;

/******************************************************************************/
/*!                    
* \par  Description:
*     ȡ��style ������api �Ľṹ�塣
* \retval   �洢��style ����api �Ľṹ�塣
*\par example code
*\code
*	style_api_t* style = get_style_api( );
*      style->debug_style_file();
* \endcode
*******************************************************************************/
style_api_t* get_style_api();

#endif /*__STYLE_H__*/
