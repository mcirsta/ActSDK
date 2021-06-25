/********************************************************************************
 *                              USDK(1100)
 *                            Module: ERRNO
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       fanqh     2008-12-30 17:25     1.0             build this file
 ********************************************************************************/
/*!
 * \file     act_errno.h
 * \brief    errno����ͷ�ļ�
 * \author   fanqh
 * \version 1.0
 * \date  2008-12-30 17:25
 *******************************************************************************/
#ifndef __ACT_ERRNO_H__
#define __ACT_ERRNO_H__

#include "libc/errno.h"

#define   SYS_ERRONO_BASE							2000

#define   GUI_ERRONO_BASE							3000
/*! ����ȡֵ���Ϸ�*/
#define   GUI_CONTEXT_INVALID_VALUE					(GUI_ERRONO_BASE+1)
/*!	ʹ�õ���ȫ�ֱ���δ��ʼ��*/
#define   GUI_CONTEXT_VARIABLE_UNINIT				(GUI_ERRONO_BASE+2)
/*! �����ļ���ʧ��*/
#define   GUI_FONT_OPEN_FONTLIB_FAILED				(GUI_ERRONO_BASE+3)
/*! �����ļ��ر�ʧ��*/
#define   GUI_FONT_CLOSE_FONTLIB_FAILED				(GUI_ERRONO_BASE+4)
/*! ��ѡ�������δ��*/
#define	  GUI_FONT_SELECTED_UNINIT					(GUI_ERRONO_BASE+5)
/*! ��ѡ����ֺŲ�֧��*/
#define	  GUI_FONT_UNSUPPORTED_FONTSIZE				(GUI_ERRONO_BASE+6)
/*! �ַ���ָ��Ϊ��*/
#define	  GUI_TEXT_STR_NULL_POINTER					(GUI_ERRONO_BASE+7)
/*! ����ת������*/
#define   GUI_TEXT_ENC_CONVERT_ERROR				(GUI_ERRONO_BASE+8)
/*! ���ֿ��ȡ���ݳ���*/
#define	  GUI_TEXT_GET_DATA_FROM_FONTLIB_ERROR		(GUI_ERRONO_BASE+9)
/*! �ӻ����ȡ���ݳ���*/
#define   GUI_TEXT_GET_DATA_FROM_CACHE_ERROR		(GUI_ERRONO_BASE+10)
/*! ����Դ�ļ�ʧ��*/
#define   GUI_RES_OPEN_FILE_FAILED					(GUI_ERRONO_BASE+11)
/*! ��ȡ�������ݷǷ�*/
#define	  GUI_RES_INVALID_VALUE						(GUI_ERRONO_BASE+12)
/*! �����������Ϸ�*/
#define   GUI_RES_INVALID_PARAMETER					(GUI_ERRONO_BASE+13)
/*! framebuffer�豸δ��*/
#define   GUI_FB_DEVICE_UNOPENED					(GUI_ERRONO_BASE+14)
/*! framebuffer��ַ�Ƿ�*/
#define   GUI_FB_INVALID_FBADDR						(GUI_ERRONO_BASE+15)
/*! ִ��ioctl�������*/
#define   GUI_FB_CMD_EXEC_ERROR						(GUI_ERRONO_BASE+16)
/*! ���ڹ�������������ȷ*/
#define   GUI_WM_INVALID_PARAMETER					(GUI_ERRONO_BASE+17)
/*! ���ھ��Ϊ0���߲�����*/
#define   GUI_WM_INVALID_HANDLE						(GUI_ERRONO_BASE+18)
/*! ��ǰ���㴰�ھ��Ϊ0*/
#define   GUI_WM_NO_FOCUS_WIN_EXIST   				(GUI_ERRONO_BASE+19)
/*! ��������ָ�벻�Ϸ�*/
#define   GUI_PARAMETER_NULL_POINTER 				(GUI_ERRONO_BASE+20)
/*! ����ֵ���Ϸ�*/
#define   GUI_PARAMETER_INVALIDE_VALUE 				(GUI_ERRONO_BASE+21)
/*! ����ֵ���Ϸ�*/
#define   GUI_VARIABLE_INVALID_VALUE				(GUI_ERRONO_BASE+22)
/*! ����ռ�ʧ��*/
#define   GUI_MALLOC_FAILED							(GUI_ERRONO_BASE+23)
/*! DC���δ��ʼ��*/
#define   GUI_GDI_DC_UNINITIALIZED					(GUI_ERRONO_BASE+24)
/*! DC����ֵ���Ϸ�*/
#define   GUI_GDI_DC_VALUE_INVALID					(GUI_ERRONO_BASE+25)

#define   FS_ERRONO_BASE								4000

#define   FLASH_ERRONO_BASE						5000

#define   AOTG_UOC_ERRONO_BASE				6000

#define   MMM_ERRONO_BASE					7000
/*! ���ļ�ʧ�� */
#define MMM_ERR_OPEN_FILE               (MMM_ERRONO_BASE+1)
/*! �ļ���ʽ��֧�� */
#define MMM_ERR_FILE_NOT_SUPPORT  (MMM_ERRONO_BASE+2)
/*! �ļ����� */
#define MMM_ERR_FILE_DAMAGED         (MMM_ERRONO_BASE+3)
/*! ������� */
#define MMM_ERR_DECODER_ERROR      (MMM_ERRONO_BASE+4)
/*! ������� */
#define MMM_ERR_ENCODER_ERROR       (MMM_ERRONO_BASE+5)
/*! �������ݸ�ʽ��֧�� */
#define MMM_ERR_VO_NOT_SUPPORT      (MMM_ERRONO_BASE+6)
/*! �˸�ʽ��Ƶ��֧��¼�� */
#define MMM_ERR_ENCORDER_FORMAT_NOT_SUPPORT    (MMM_ERRONO_BASE+7)
/*! ���̿ռ䲻�� */
#define MMM_ERR_ENCORDER_NO_SPACE   (MMM_ERRONO_BASE+8)
/*! �˸�ʽ��Ƶ��֧������ */
#define MMM_ERR_VOCALIZE_FORMAT_NOT_SUPPORT    (MMM_ERRONO_BASE+9)
/*! ��license�����Ŵ����������ǲ���ʱ���Ѿ����� */
#define MMM_ERR_NO_LICENSE              (MMM_ERRONO_BASE+10)
/*! DRMʱ�Ӵ��� */
#define MMM_ERR_SECURE_CLOCK         (MMM_ERRONO_BASE+11)
/*! DRM��Ϣ���� */
#define MMM_ERR_LICENSE_INFO         (MMM_ERRONO_BASE+12)
/*! ��Ч������ע��������봦��ĳ��״̬ʱ��Ӧ�ò㷢�͵�ĳЩ�������Ч������Ż���Ӧ�ı�Ϊ��Ч���� */
#define MMM_ERR_INVALUABLE_OPERATION    (MMM_ERRONO_BASE+13)
/*! �������� */
#define MMM_ERR_OTHER                   (MMM_ERRONO_BASE+14)

#define   AOTG_UDC_ERRONO_BASE				8000

#define   UDC4UVC_ERRONO_BASE					9000

#define   FSG_ERRONO_BASE							10000

#define   VRAM_ERRONO_BASE							11000

#define   MMM_DCA_ERRONO_BASE					12000

#define   MMM_DCT_ERRONO_BASE					13000

#define   KSC_ERRONO_BASE							14000

#define   MMM_MTP_ERRONO_BASE					15000

#define   UVC_ERRONO_BASE 							16000

#define   MSMTP_ERRONO_BASE						17000

#define   MTPLST_ERRONO_BASE 			  			18000

#define   STUB_ERRONO_BASE							19000

#define   FONTDRV_ERRONO_BASE					20000

#define   TVOUT_ERRONO_BASE						21000

#define   UPGRADE_ERRONO_BASE					22000
/*! �������̼������� */
#define UPGRADE_FW_NOT_EXIST        (UPGRADE_ERRONO_BASE+1)

/*! �������ռ䲻�� */
#define UPGRADE_BUF_NOT_ENOUGH      (UPGRADE_ERRONO_BASE+2)

/*! �������̼���ͷ����־���� */
#define UPGRADE_FW_HEADFLAG_ERR     (UPGRADE_ERRONO_BASE+3)

#define   DWL_ERRONO_BASE							23000

#define   PP_ERRONO_BASE								24000

#define   PLAYLIST_ERRONO_BASE                          25000
/*!�����ڴ�ʧ��*/
#define PLIST_NOMEM               (PLAYLIST_ERRONO_BASE+1)

/*!����Ƿ�����*/
#define PLIST_INVALIDPARAM           (PLAYLIST_ERRONO_BASE+2)

/*!ָ���ı�ṹΪ��.��Ҫ��һ���б������б��ֲ�����ʱ�������ָ���Ľṹ�����б�����˽ṹΪ�գ��򷵻ش˴��롣*/
#define PLIST_EMPTYLISTFIELD      (PLAYLIST_ERRONO_BASE+3)

/*!ʹ��ʧЧ���б���*/
#define PLIST_INVALIDHANDLE       (PLAYLIST_ERRONO_BASE+4)

/*!��ͬ�Ĳ����б�ṹ*/
#define PLIST_DIFFSTRUCT          (PLAYLIST_ERRONO_BASE+5)

/*!����λ�ò����б���*/
#define PLIST_INVALIDEINDEX       (PLAYLIST_ERRONO_BASE+6)

/*!�б������ļ����ֶ�,ĳЩ����Ҫ���б���뺬���ļ�ȫ·���ֶ�*/
#define PLIST_NONAMEFIELD         (PLAYLIST_ERRONO_BASE+7)

/*!ʹ��fopen ��ʱ��������*/
#define PLIST_FOPENFAIL           (PLAYLIST_ERRONO_BASE+8)

/*!���ݿ��д��ļ���Ϣ�Ѿ�����*/
#define PLIST_FILEEXIST           (PLAYLIST_ERRONO_BASE+9)

/*!ʹ���ļ�ѡ�������ù���Ŀ¼ʱ����*/
#define PLIST_FSELSETWORKDIRFAIL  (PLAYLIST_ERRONO_BASE+10)

/*!�ֶ���ȫhashʧ��*/
#define PLIST_FIELDHASHCOLLIDE    (PLAYLIST_ERRONO_BASE+11)

/*!û�м�¼�����仯*/
#define PLIST_NORECORDCHANGE      (PLAYLIST_ERRONO_BASE+12)

/*!��ͼ�޸�ϵͳ�����ı�*/
#define PLIST_CHANGESYSTBL        (PLAYLIST_ERRONO_BASE+13)

/*!�Ƿ����б�����*/
#define PLIST_INVALIDLISTTYPE   (PLAYLIST_ERRONO_BASE+14)

/*!��ͼ�޸Ĳ����޸ĵ��ֶ�.��ǰֻ֧���Ǽ����ļ�ȫ·�����޸�*/
#define PLIST_MODIFYREADABLEFIELD  (PLAYLIST_ERRONO_BASE+15)

/*!�����б��в����ڵ��ֶ�*/
#define PLIST_FIELDNOTEXIST        (PLAYLIST_ERRONO_BASE+16)

/*!����ָ�������������������ļ�*/
#define PLIST_NOFILE               (PLAYLIST_ERRONO_BASE+17)

/*!·��Ϊ���ַ���*/
#define PLIST_NULLPATH             (PLAYLIST_ERRONO_BASE+18)

/*!Ŀ¼ѡ�������ù�����������*/
#define PLIST_FSELSETFILTERFAIL    (PLAYLIST_ERRONO_BASE+19)

/*!�����ݿ��ļ�ʧ��*/
#define PLIST_OPENDBFAIL           (PLAYLIST_ERRONO_BASE+20)

/*!�����ֶ����б��в�����*/
#define PLIST_NOFILTERFIELD        (PLAYLIST_ERRONO_BASE+21)

/*!�Ƿ����ļ�����*/
#define PLIST_INVALIDFILETYPE      (PLAYLIST_ERRONO_BASE+22)

/*!�˱�δ����*/
#define PLSIT_NOTEXISTTABLE        (PLAYLIST_ERRONO_BASE+23)

/*!���ݿ��ļ�����*/
#define PLIST_CORRUPTDBFILE          (PLAYLIST_ERRONO_BASE+24)

#define   MLANG_ERRONO_BASE                          26000
/*! ��������Ϊ��ָ��*/
#define   MLANG_NULL_POINTER			(MLANG_ERRONO_BASE+1)
/*! �򿪱���ת���ļ�ʧ��*/
#define   MLANG_OPEN_CODEPAGE_FAILED	(MLANG_ERRONO_BASE+2)
/*! �򿪶�����������ļ�ʧ��*/
#define   MLANG_OPEN_CONFIG_FAILED		(MLANG_ERRONO_BASE+3)
/*! ������������ļ�����ʧ��*/
#define   MLANG_PARSE_CONFIG_FAILED		(MLANG_ERRONO_BASE+4)
/*! ����id���Ϸ�*/
#define   MLANG_INVALID_LANGUAGE_ID		(MLANG_ERRONO_BASE+5)

#define   FSEL_ERRONO_BASE                          27000

/*! �����������*/
#define  FSEL_PARA_ERROR              (FSEL_ERRONO_BASE+1)
/*! ���ݿ�û�в�ѯ���*/
#define  FSEL_NO_RECORD               (FSEL_ERRONO_BASE+2)
/*! �������ݿ�ָ��ִ�з��ز��ɹ�*/
#define  FSEL_OP_DB_ERROR             (FSEL_ERRONO_BASE+3)
/*! �ڴ�����ʧ��*/
#define  FSEL_MALLOC_ERROR            (FSEL_ERRONO_BASE+4)
/*! �����쳣,���������ϲ�ѯ�ļ�ʧ��*/
#define  FSEL_DATA_ERROR              (FSEL_ERRONO_BASE+5)
/*! ���̿ռ���*/
#define  FSEL_DISK_FULL               (FSEL_ERRONO_BASE+6)
/*! δ�趨����Ŀ¼*/
#define  FSEL_NOWORKDIR               (FSEL_ERRONO_BASE+7)

#define   APPS_ERRONO_BASE                          28000

/*! �����˳�����Ϣѭ��*/
#define  APPS_QUIT                    (APPS_ERRONO_BASE+1)

#define  APCONFIG_ERRONO_BASE                       29000
/*! �Ҳ���KEY */
#define CONFIG_KEY_NOT_FOUND          (APCONFIG_ERRONO_BASE+1)
/*! ����buffer�ĳ��ȳ������ֵ */
#define CONFIG_EXCEED_MAX_LENGTH      (APCONFIG_ERRONO_BASE+2)

#endif
