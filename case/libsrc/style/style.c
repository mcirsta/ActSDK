/*
*******************************************************************************
*
*                       NOYA1100 ---style�����
*                (c) Copyright 2002-2008, Actions Co,Ld.
*                        All Right Reserved
*
* FileName: global.h     Author:  zhaowenjie        Date:2008/12/16
* Description:    style �������ֽӿڵľ���ʵ�֡�
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
*                            Module: ������дģ����
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:
*      <author>    <time>       <version >    <desc>
*         zhaowenjie      2008/12/16     1.0         �������ļ���
*         zhaowenjie      2008/12/16     1.1         �������Ϊ����1100 ���������
********************************************************************************/
/*!
* \file     style.c
* \brief    �Ƿ�����������С���˵ĳ������ڶ�ȡsty�ļ���
* \author   ���Ľ�
*               
* \version 1.0
* \date  2009/1/14
*******************************************************************************/

#include "style.h"
#include "style_common.h"


#ifndef WINDOWS

#include <libc.h>
#include <debug.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#include <fcntl.h>

#ifdef _MSC_VER

#pragma pack(1)
#include <io.h>

#define read _read
#define open  _open
#define lseek _lseek
#define close _close

#else

#include <unistd.h>

#endif


#define SCENE_ITEM_SIZE 3

#define TELL( fd ) lseek( fd, 0, SEEK_CUR )


/*��Դ�����ļ���ͷ����Ϣ*/
typedef struct
{
    unsigned int    magic;        /*0x18*/
    unsigned int    scene_sum;
    char          reserve[8];
}PACK
style_head_t;

static int _read_head( style_t* style );
scene_t* _get_scene( style_t* style, unsigned int* scene_item );

static style_api_t s_style_api =
{
    open_style_file,
    close_style_file,
    load_scene,
    free_scene,
    get_scene_child,
    get_resgroup_child,
    get_picregion_frame,
};

#ifndef WINDOWS

unsigned int style_apitbl[] =
{
    (unsigned int)open_style_file,
    (unsigned int)close_style_file,
    (unsigned int)load_scene,
    (unsigned int)free_scene,
    (unsigned int)get_scene_child,
    (unsigned int)get_resgroup_child,
    (unsigned int)get_picregion_frame,
    (unsigned int)get_style_api,
};



///////////////////////////////////////////////////////////////////////////////

int __attribute__((constructor)) style_api_init(void)
{
    PRINTF("%s\n",__FUNCTION__);    
    
    api_install( APINO_STYLE, (unsigned)style_apitbl);

    return 0;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     exit����
* \note
* ʹ�ú�__exitȷ��������λ��.dtors��
*******************************************************************************/
void __attribute__((destructor)) style_api_exit(void)
{
    PRINTF("%s\n",__FUNCTION__);
    
    api_uninstall( APINO_STYLE );
    return ;
}

#endif /* WINDOWS */


///////////////////////////////////////////////////////////////////////////////

style_t* open_style_file( const unsigned char* filepath )
{
    int ret;
    style_t* style;
    
    if ( filepath == NULL )
    {
        return NULL;
    }

    style = (style_t*)malloc( sizeof( style_t ) );
    if ( style == NULL )
    {
        PRINTF( " (style_t*)malloc( sizeof( style_t ) ); failed ! \n" );
        return NULL;
    }

    style->file = open( filepath , O_RDONLY );
    if ( style->file < 0 )
    {
        PRINTF(" open file %s failed! \n", filepath);

        free( style );
        return NULL;
    }

    ret = _read_head( style );     /*��ȡ�ļ���ͷ��*/
    if ( ret < 0 )
    {
        close( style->file );
        free( style );
        return NULL;
    }

    return style;
}

/* ��ȡstyle �ļ���ͷ����Ϣ, ����ֵ�Ǹ������ļ���scene ������*/
static int _read_head( style_t* style )
{
    style_head_t head;
    int ret;

    ret = read( style->file, &head, sizeof( style_head_t ) );
    if ( ret < 0 )
    {
        return -1;
    }

    PRINTF("\n---- read in the head----\n");
    PRINTF("magic = %d\n", head.magic);
    PRINTF("scene sum = %d\n", head.scene_sum);
    PRINTF("---- end the head ----\n");

    style->sum = head.scene_sum;
    assert(head.scene_sum < 0xffffffff);
    style->scenes = (int*)malloc( ( head.scene_sum * SCENE_ITEM_SIZE * 4 ) );
    ret = read( style->file, style->scenes, ( head.scene_sum * SCENE_ITEM_SIZE * 4 ) );
    if ( ret < 0 )
    {
        return -1;
    }

    return 0;
}

void close_style_file( style_t* style )
{
    if ( style == NULL )
    {
        return;
    }

    close( style->file );
    style->file = 0;
    
    free( style->scenes );
    style->scenes = NULL;
    
    free( style );    
    style = NULL;
}


/* ! \cond STYLE_API */

/******************************************************************************/
/*!                    
* \par  Description:
*      ��sty �ļ���ȡ��ָ��ID ��scene  ��
* \param[in]    filepath -- ��sty �ļ���·����
* \param[in]    id -- ��scene ��ID��
* \retval       ��scene ��ָ���0( û�ҵ� )��
*\par example code
*\code
*   scene_t* scene = load_scene( "music.sty" , 1 );
* \endcode
*******************************************************************************/

scene_t* load_scene( style_t* style, const unsigned id )
{
    unsigned int i;
    unsigned int* scene_item;

    if ( style == NULL )
    {
        return NULL;
    }

    scene_item = style->scenes;
    for ( i = 0; i < style->sum; i++ )
    {
        if ( *scene_item == id )
        {
            return _get_scene( style, scene_item );
        }

        scene_item = scene_item + SCENE_ITEM_SIZE;
    }

    return NULL;
}

scene_t* _get_scene( style_t* style, unsigned int* scene_item )
{
    scene_t* scene;
    unsigned int offset;
    unsigned int size;
    int ret;
    
    offset = *( scene_item + 1 );
    size = *( scene_item + 2 );

    scene = ( scene_t* )malloc( size );
    ret = (int)lseek( style->file, offset, SEEK_SET );
    if ( ret < 0 )
    {
        PRINTF( " lseek( style->file, offset, SEEK_SET ); failed !!! " );

        free( scene );
        return NULL;
    }
    
    ret = read( style->file, scene, size );
    if ( ret < 0 )
    {
        PRINTF( " read( style->file, scene, size ); failed !!! " );

        free( scene );
        return NULL;
    }

    return scene;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      �ͷ� scene ��Դ  ��
* \param[in]    scene -- Ҫ�ͷŵ� scene ��
*\par example code
*\code
*   free_scene( scene );
* \endcode
*******************************************************************************/
void free_scene( scene_t* scene )
{
    free( scene );
    scene = NULL;
}

/******************************************************************************/
/*!                    
* \par  Description:
*      ��ָ����scene �в���ָ�� �ֲ�ID ����Դ��
* \param[in]    scene -- ��scene ��ָ�롣
* \param[in]    id -- Ҫ���ҵ���Դ��ID��
* \retval      ����Դ��ָ���0( û�ҵ�)��
*\par example code
*\code
*   void* resource = get_scene_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_scene_child( const scene_t* scene, const unsigned int id )
{
    unsigned int i;
    resource_t* resource = NULL;
    
    if ( scene == NULL )
    {
        return NULL;
    }

    resource = (resource_t*)( (int)scene + (int)scene->child_offset );
    for ( i = 0; i < scene->resource_sum; i++ )
    {
        if ( resource->id == id )
        {
            return (void*)resource;
        }

        resource = (resource_t*)( (int)resource + (int)resource->offset );
    }

    return NULL;        
    
}


/******************************************************************************/
/*!                    
* \par  Description:
*      ��ָ����resgroup �в���ָ�� �ֲ�ID ����Դ��
* \param[in]    resgroup -- ��resgroup ��ָ�롣
* \param[in]    id -- Ҫ���ҵ���Դ��ID��
* \retval      ����Դ��ָ���0( û�ҵ�)��
*\par example code
*\code
*   void* resource = get_resgroup_child( resgroup, 2 );
* \endcode
*******************************************************************************/
void* get_resgroup_child( const resgroup_resource_t* resgroup, const unsigned int id )
{
    unsigned int i;
    resource_t* resource = NULL;
    
    if ( resgroup == NULL )
    {
        return NULL;
    }

    resource = (resource_t*)( (int)resgroup + (int)resgroup->child_offset );
    for ( i = 0; i < resgroup->resource_sum; i++ )
    {
        if ( resource->id == id )
        {
            return (void*)resource;
        }

        resource = (resource_t*)( (int)resource + (int)resource->offset );
    }

    return NULL;
}


/******************************************************************************/
/*!                    
* \par  Description:
*      ��picRegion ��ȡ��֡��Ϊframe ��ͼƬ֡�����õ��������pictureframe �С�
* \param[in]    picRegion -- ��picRegion ��ָ�롣
* \param[in]    frame -- ��֡����ţ��� 0 ��ʼ��
* \param[out]    pictureframe -- ���ص�֡�ṹ��
* \retval      0 �ɹ���-1 ʧ�ܡ�
*\par example code
*\code
*   picregion_resource_t* picture = (picregion_resource_t*)get_resource( scene, 2 );
*   picregion_frame_t* pictureframe = (picregion_frame_t* )malloc( sizeof( picregion_frame_t ) );
*   get_picregion_frame( picture, 3, pictureframe );
* \endcode
*******************************************************************************/
int get_picregion_frame( const picregion_resource_t* picRegion, const unsigned short frame, 
                        picregion_frame_t* pictureframe )
{
    unsigned short* pictureid;
    unsigned char* single_char;
    unsigned char first_char;
    unsigned char second_char;
    unsigned short pic_id;

    if ( ( picRegion == NULL )  || ( pictureframe == NULL ) )
    {
        return FAILURE;
    }

    if ( frame >= picRegion->frames )
    {
        /* �����˸�ͼƬ��ͼ����*/
        return FAILURE;
    }

     /* ȡ��Ҫͼ���ͼƬID ��Ҫ����ֽ�ȡ����Ϊ��ӵõ��ĵ�ַ�ܿ����ǲ������ */
    pictureid = (unsigned short*)( (int)picRegion + (int)picRegion->pic_offset );
    single_char = (unsigned char*)( pictureid + frame );
    first_char = *single_char;
    second_char = *( single_char + 1 );
    pic_id = ( unsigned short )second_char;
    pic_id = ( ((unsigned short)second_char) << 8 ) | first_char;

    pictureframe->id = picRegion->resource.id;
    pictureframe->frame = frame;
    pictureframe->x = picRegion->x;
    pictureframe->y = picRegion->y;
    pictureframe->width = picRegion->width;
    pictureframe->height = picRegion->height;
    pictureframe->pic_id = pic_id;

    return SUCCESS;
}

/******************************************************************************/
/*!                    
* \par  Description:
*     ȡ��style ������api �Ľṹ�塣
* \retval   �洢��style ����api �Ľṹ�塣
*\par example code
*\code
*   style_api_t* style = get_style_api( );
*      style->debug_style_file();
* \endcode
*******************************************************************************/
style_api_t* get_style_api(void)
{
    return &s_style_api;
}

/*! \endcond*/

