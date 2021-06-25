/*!
 * \file   ptp.h
 * \brief
 * \author Jerry He, hejf@actions-semi.com
 * \par GENERAL DESCRIPTION:
 *  PTP 常量定义
 *  \version 1.0
 *  \date  2008.08.08
 *******************************************************************************/
#ifndef __PTP_H_34543
#define __PTP_H_34543

/*!
 * \def  PTP_SEQUENCE_ALL
 */
/*!全部引用序列编号*/
#define PTP_SEQUENCE_ALL    0xFFFFFFFF

#define   PROPERTY_FORM_NONE                 0x00
#define   PROPERTY_FORM_RANGE                0x01
#define   PROPERTY_FORM_ENUM                 0x02
#define   PROPERTY_FORM_DATETIME           0x03
#define   PROPERTY_FORM_FIXED_ARRAY     0x04
#define   PROPERTY_FORM_REGULAR_EXPRESSION   0x05
#define   PROPERTY_FORM_BYTE_ARRAY           0x06
#define   PROPERTY_FORM_LONG_STRING          0xFF

#define   PROPERTY_GROUP_NONE           0x00000000
#define   PROPERTY_GROUP_SYNC           0x00000001
#define   PROPERTY_GROUP_UI             0x00000002
#define   PROPERTY_GROUP_OBJECT_INFO    0x00000004
#define   PROPERTY_GROUP_OFTEN_USED     0x00000008
#define   PROPERTY_GROUP_SUPPLEMENTAL   0x00000100
#define   PROPERTY_GROUP_UNKNOWN        0x00010000
#define   PROPERTY_GROUP_SLOW           0x00FF0000
#define   PROPERTY_GROUP_ALL            0xFFFFFFFF

/*
 * USB class-specific requests
 */
#define USB_PTPREQUEST_TYPE_OUT   0x21    /* Host to Device */
#define USB_PTPREQUEST_TYPE_IN    0xA1    /* Device to Host */
#define USB_PTPREQUEST_CANCELIO   0x64    /* Cancel request */
#define USB_PTPREQUEST_GETEVENT   0x65    /* Get extened event data */
#define USB_PTPREQUEST_RESET      0x66    /* Reset Device */
#define USB_PTPREQUEST_GETSTATUS  0x67    /* Get Device Status */

#define USB_PTPCANCELIO_ID   0x4001

#define PTP_CONTAINER_TYPE_UNDEFINED   0
#define PTP_CONTAINER_TYPE_CMD         1
#define PTP_CONTAINER_TYPE_DATA        2
#define PTP_CONTAINER_TYPE_RESPONSE    3
#define PTP_CONTAINTER_TYPE_EVENT      4

/* Define standard min and max macros */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

/* PTP Data type codes. */
#define PTP_DATATYPE_UNDEFINED   0x0000
#define PTP_DATATYPE_INT8        0x0001
#define PTP_DATATYPE_UINT8       0x0002
#define PTP_DATATYPE_INT16       0x0003
#define PTP_DATATYPE_UINT16      0x0004
#define PTP_DATATYPE_INT32       0x0005
#define PTP_DATATYPE_UINT32      0x0006
#define PTP_DATATYPE_INT64       0x0007
#define PTP_DATATYPE_UINT64      0x0008
#define PTP_DATATYPE_INT128      0x0009
#define PTP_DATATYPE_UINT128     0x000A
#define PTP_DATATYPE_AINT8       0x4001
#define PTP_DATATYPE_AUINT8      0x4002
#define PTP_DATATYPE_AINT16      0x4003
#define PTP_DATATYPE_AUINT16     0x4004
#define PTP_DATATYPE_AINT32      0x4005
#define PTP_DATATYPE_AUINT32     0x4006
#define PTP_DATATYPE_AINT64      0x4007
#define PTP_DATATYPE_AUINT64     0x4008
#define PTP_DATATYPE_AINT128     0x4009
#define PTP_DATATYPE_AUINT128    0x400A
#define PTP_DATATYPE_STRING      0xFFFF
#define PTP_DATATYPE_ARRAYMASK   0x4FF0
#define PTP_DATATYPE_ARRAY       0x4000
#define PTP_DATATYPE_VALUEMASK   0xFFF0
#define PTP_DATATYPE_VALUE       0x0000

/*
 * Data code ranges and masks. Each data code has 16 bits:
 *
 * Bit 15(std/vendor)
 *    0 -- the code is defined by PTP standard
 *    1 -- the code is vendor specific
 *
 * Bit 14 - 12(data type)
 *   14 13 12
 *   0  0  0    -- undefined data type
 *   0  0  1    -- op code
 *   0  1  0    -- response code
 *   0  1  1    -- format code
 *   1  0  0    -- event code
 *   1  0  1    -- property code
 *   1  1  0    -- reserved
 *   1  1  1    -- reserved
 *
 * Bit 11 - bit 0 (data value)
 */
#define  PTP_DATACODE_VENDORMASK           0x8000
#define  PTP_DATACODE_TYPEMASK             0x7000
#define  PTP_DATACODE_VALUEMASK            0x0FFF
#define  PTP_DATACODE_TYPE_UNKNOWN         0x0000
#define  PTP_DATACODE_TYPE_OPERATION       0x1000
#define  PTP_DATACODE_TYPE_RESPONSE        0x2000
#define  PTP_DATACODE_TYPE_FORMAT          0x3000
#define  PTP_DATACODE_TYPE_EVENT           0x4000
#define  PTP_DATACODE_TYPE_PROPERTY        0x5000
#define  PTP_DATACODE_TYPE_RESERVED_1      0x6000
#define  PTP_DATACODE_TYPE_RESERVED_2      0x7000

/*
 * To verify an op code
 *  (Code & PTP_DATACODE_TYPEMASK) == PTP_DATACODE_TYPE_OPERATION
 * To verify a response code
 *  (Code & PTP_DATACODE_TYPEMASK) == PTP_DATACODE_TYPE_RESPONSE)
 */

/* Image format codes receive special treatment.*/
#define  PTP_DATACODE_TYPEIMAGEMASK        0x7800;
#define  PTP_DATACODE_TYPE_IMAGEFORMAT     0x3800;
#define  PTP_DATACODE_VALUE_IMAGEVMASK     0x07FF;

/*
 * To verify an image code
 * (Code & PTP_DATACODE_TYPEIMAGEMASK) == PTP_DATACODE_TYPE_IMAGEFORMAT
 */
/*
 * PTP specially defined constants
 */
#define PTP_OBJECTHANDLE_ALL          0xFFFFFFFF
#define PTP_OBJECTHANDLE_UNDEFINED    0x0
#define PTP_OBJECTHANDLE_ROOT         0x0
#define PTP_PROPERTY_ALL              0xFFFFFFFF
#define PTP_PROPERTY_UNDEFINED        0x0
#define PTP_STORAGEID_ALL             0xFFFFFFFF
#define PTP_STORAGEID_DEFAULT         0
#define PTP_STORAGEID_UNDEFINED       0
#define PTP_STORAGEID_PHYSICAL        0x0000FFFF
#define PTP_STORAGEID_LOGICAL         0xFFFF0000
#define PTP_SESSIONID_ALL             0
#define PTP_SESSIONID_NOSESSION       0
#define PTP_FORMATCODE_NOTUSED        0x0
#define PTP_FORMATCODE_ALL            0xFFFFFFFF
#define PTP_FORMATCODE_DEFAULT        0x0000
#define PTP_TRANSACTIONID_ALL         0xFFFFFFFF
#define PTP_TRANSACTIONID_NOSESSION   0

/*
 * standard operation codes:
 */
#define PTP_OPCODE_UNDEFINED               0x1000
#define PTP_OPCODE_GETDEVICEINFO           0x1001
#define PTP_OPCODE_OPENSESSION             0x1002
#define PTP_OPCODE_CLOSESESSION            0x1003
#define PTP_OPCODE_GETSTORAGEIDS           0x1004
#define PTP_OPCODE_GETSTORAGEINFO          0x1005
#define PTP_OPCODE_GETNUMOBJECTS           0x1006
#define PTP_OPCODE_GETOBJECTHANDLES        0x1007
#define PTP_OPCODE_GETOBJECTINFO           0x1008
#define PTP_OPCODE_GETOBJECT               0x1009
#define PTP_OPCODE_GETTHUMB                0x100A
#define PTP_OPCODE_DELETEOBJECT            0x100B
#define PTP_OPCODE_SENDOBJECTINFO          0x100C
#define PTP_OPCODE_SENDOBJECT              0x100D
#define PTP_OPCODE_INITIATECAPTURE         0x100E
#define PTP_OPCODE_FORMATSTORE             0x100F
#define PTP_OPCODE_RESETDEVICE             0x1010
#define PTP_OPCODE_SELFTEST                0x1011
#define PTP_OPCODE_SETOBJECTPROTECTION     0x1012
#define PTP_OPCODE_POWERDOWN               0x1013
#define PTP_OPCODE_GETDEVICEPROPDESC       0x1014
#define PTP_OPCODE_GETDEVICEPROPVALUE      0x1015
#define PTP_OPCODE_SETDEVICEPROPVALUE      0x1016
#define PTP_OPCODE_RESETDEVICEPROPVALUE    0x1017
#define PTP_OPCODE_TERMINATECAPTURE        0x1018
#define PTP_OPCODE_MOVEOBJECT              0x1019
#define PTP_OPCODE_COPYOBJECT              0x101A
#define PTP_OPCODE_GETPARTIALOBJECT        0x101B
#define PTP_OPCODE_INITIATEOPENCAPTURE     0x101C
#define PTP_OPCODE_VENDOREXTENDEDBASE      0x9000

/* MTP extended operations */
#define	MTP_OPCODE_UNDEFINED               0x9800
#define	MTP_OPCODE_GETOBJECTPROPSUPPORTED  0x9801
#define	MTP_OPCODE_GETOBJECTPROPDESC       0x9802
#define	MTP_OPCODE_GETOBJECTPROPVALUE      0x9803
#define	MTP_OPCODE_SETOBJECTPROPVALUE      0x9804
#define	MTP_OPCODE_GETOBJECTPROPLIST       0x9805
#define MTP_OPCODE_SETOBJECTPROPLIST       0x9806
#define MTP_OPCODE_GETINTERDEPPROPDESC     0x9807
#define MTP_OPCODE_SENDOBJECTPROPLIST      0x9808
#define	MTP_OPCODE_GETOBJECTREFERENCES     0x9810
#define	MTP_OPCODE_SETOBJECTREFERENCES     0x9811

/* MTP Playback control operation */
#define	MTP_OPCODE_PLAYBACK_SKIP				0x9820

/* Operations for Janus MTP extension  */
#define MTP_OPCODE_JANUS_UNDEFINED                 0x9100
//#define MTP_OPCODE_JANUS_GETDEVCERTCHANLLENGE      0x9101
//#define MTP_OPCODE_JANUS_SETDEVCERTRESPONSE        0x9102
#define MTP_OPCODE_JANUS_GETSECURETIMECHALLENGE    0x9101
#define MTP_OPCODE_JANUS_SETSECURETIMERESPONSE     0x9102
#define MTP_OPCODE_JANUS_SETLICENSERESPONSE        0x9103
#define MTP_OPCODE_JANUS_GETSYNCLIST               0x9104
#define MTP_OPCODE_JANUS_SENDMETERCHALLENGEQUERY   0x9105
#define MTP_OPCODE_JANUS_GETMETERDATA              0x9106
#define MTP_OPCODE_JANUS_RESETMETERDATA            0x9107
#define MTP_OPCODE_JANUS_CLEANDATASTORE            0x9108
#define MTP_OPCODE_JANUS_GETLICENSESTATEDATA       0x9109
#define MTP_OPCODE_JANUS_SENDWMDRMPDCOMMAND        0x910a
#define MTP_OPCODE_JANUS_SENDWMDRMPDREQUEST        0x910b
/* Operation for Windows Media 10 MTP extension */
#define	MTP_OPCODE_WMP_UNDEFINED				0x9200
#define	MTP_OPCODE_WMP_METADATAROUNDTRIP			0x9201

/*
 * standard event codes:
 */
#define PTP_EVENTCODE_UNDEFINED                0x4000
#define PTP_EVENTCODE_CANCELTRANSACTION        0x4001
#define PTP_EVENTCODE_OBJECTADDED              0x4002
#define PTP_EVENTCODE_OBJECTREMOVED            0x4003
#define PTP_EVENTCODE_STOREADDED               0x4004
#define PTP_EVENTCODE_STOREREMOVED             0x4005
#define PTP_EVENTCODE_DEVICEPROPCHANGED        0x4006
#define PTP_EVENTCODE_OBJECTINFOCHANGED        0x4007
#define PTP_EVENTCODE_DEVICEINFOCHANGED        0x4008
#define PTP_EVENTCODE_REQUESTOBJECTTRANSFER    0x4009
#define PTP_EVENTCODE_STOREFULL                0x400A
#define PTP_EVENTCODE_DEVICERESET              0x400B
#define PTP_EVENTCODE_STORAGEINFOCHANGED       0x400C
#define PTP_EVENTCODE_CAPTURECOMPLETE          0x400D
#define PTP_EVENTCODE_UNREPORTEDSTATUS         0x400E
#define PTP_EVENTCODE_VENDOREXTENTION1         0xC001
#define PTP_EVENTCODE_VENDOREXTENTION2         0xC002

/*
 * MTP-extended Events
 */
#define MTP_EVENTCODE_UNDEFINED                0xB800
#define MTP_EVENTCODE_OBJECTPROPCHANGED        0xB801
#define MTP_EVENTCODE_OBJECTPROPDESCCHANGED    0xB802
#define MTP_EVENTCODE_OBJECTREFERENCESCHANGED  0xB803
#define MTP_EVENTCODE_DEVICEPROPDESCCHANGED    0xB804

/* Events for Janus MTP extension */
#define MTP_EVENTCODE_JANUS_UNDEFINED          0xC100
#define MTP_EVENTCODE_JANUS_EVENT1             0xC101

/*
 * standard response codes:
 */
#define PTP_RESPONSECODE_UNDEFINED                     0x2000
#define PTP_RESPONSECODE_OK                            0x2001
#define PTP_RESPONSECODE_GENERALERROR                  0x2002
#define PTP_RESPONSECODE_SESSIONNOTOPEN                0x2003
#define PTP_RESPONSECODE_INVALIDTRANSACTIONID          0x2004
#define PTP_RESPONSECODE_OPERATIONNOTSUPPORTED         0x2005
#define PTP_RESPONSECODE_PARAMETERNOTSUPPORTED         0x2006
#define PTP_RESPONSECODE_INCOMPLETETRANSFER            0x2007
#define PTP_RESPONSECODE_INVALIDSTORAGEID              0x2008
#define PTP_RESPONSECODE_INVALIDOBJECTHANDLE           0x2009
#define PTP_RESPONSECODE_PROPERTYNOTSUPPORTED          0x200A
#define PTP_RESPONSECODE_INVALIDOBJECTFORMATCODE       0x200B
#define PTP_RESPONSECODE_STOREFULL                     0x200C
#define PTP_RESPONSECODE_OBJECTWRITEPROTECTED          0x200D
#define PTP_RESPONSECODE_STOREWRITEPROTECTED           0x200E
#define PTP_RESPONSECODE_ACCESSDENIED                  0x200F
#define PTP_RESPONSECODE_NOTHUMBNAILPRESENT            0x2010
#define PTP_RESPONSECODE_SELFTESTFAILED                0x2011
#define PTP_RESPONSECODE_PARTIALDELETION               0x2012
#define PTP_RESPONSECODE_STORENOTAVAILABLE             0x2013
#define PTP_RESPONSECODE_NOSPECIFICATIONBYFORMAT       0x2014
#define PTP_RESPONSECODE_NOVALIDOBJECTINFO             0x2015
#define PTP_RESPONSECODE_INVALIDCODEFORMAT             0x2016
#define PTP_RESPONSECODE_UNKNOWNVENDORCODE             0x2017
#define PTP_RESPONSECODE_CAPTUREALREADYTERMINATED      0x2018
#define PTP_RESPONSECODE_DEVICEBUSY                    0x2019
#define PTP_RESPONSECODE_INVALIDPARENT                 0x201A
#define PTP_RESPONSECODE_INVALIDPROPFORMAT             0x201B
#define PTP_RESPONSECODE_INVALIDPROPVALUE              0x201C
#define PTP_RESPONSECODE_INVALIDPARAMETER              0x201D
#define PTP_RESPONSECODE_SESSIONALREADYOPENED          0x201E
#define PTP_RESPONSECODE_TRANSACTIONCANCELLED          0x201F
/*
 * MTP responses
 */
#define MTP_RESPONSECODE_UNDEFINED                     0xA800
#define MTP_RESPONSECODE_INVALIDOBJECTPROPCODE         0xA801
#define MTP_RESPONSECODE_INVALIDOBJECTPROPFORMAT       0xA802
#define MTP_RESPONSECODE_INVALIDOBJECTPROPVALUE        0xA803
#define MTP_RESPONSECODE_INVALIDOBJECTREFERENCE        0xA804
#define MTP_RESPONSECODE_INVALIDOBJECTGROUPCODE        0xA805
#define MTP_RESPONSECODE_INVALIDDATASET                0xA806
#define MTP_RESPONSECODE_SPECIFICATION_BY_GROUP_UNSUPPORTED 0xA807
#define MTP_RESPONSECODE_SPECIFICATION_BY_DEPTH_UNSUPPORTED 0xA808

/* Responses for Janus MTP extension  */
#define MTP_RESPONSECODE_JANUS_UNDEFINED               0xA100
#define MTP_RESPONSECODE_JANUS_RESPONSEREQUIRED        0xA101

/*
 * standard property codes:
 */
#define PTP_PROPERTYCODE_UNDEFINED                 0x5000
#define PTP_PROPERTYCODE_BATTERYLEVEL              0x5001
#define PTP_PROPERTYCODE_FUNCTIONMODE              0x5002
#define PTP_PROPERTYCODE_IMAGESIZE                 0x5003
#define PTP_PROPERTYCODE_COMPRESSIONSETTING        0x5004
#define PTP_PROPERTYCODE_WHITEBALANCE              0x5005
#define PTP_PROPERTYCODE_RGBGAIN                   0x5006
#define PTP_PROPERTYCODE_FNUMBER                   0x5007
#define PTP_PROPERTYCODE_FOCALLENGTH               0x5008
#define PTP_PROPERTYCODE_FOCUSDISTANCE             0x5009
#define PTP_PROPERTYCODE_FOCUSMODE                 0x500A
#define PTP_PROPERTYCODE_EXPOSUREMETERINGMODE      0x500B
#define PTP_PROPERTYCODE_FLASHMODE                 0x500C
#define PTP_PROPERTYCODE_EXPOSURETIME              0x500D
#define PTP_PROPERTYCODE_EXPOSUREPROGRAMMODE       0x500E
#define PTP_PROPERTYCODE_EXPOSUREINDEX             0x500F
#define PTP_PROPERTYCODE_EXPOSURECOMPENSATION      0x5010
#define PTP_PROPERTYCODE_DATETIME                  0x5011
#define PTP_PROPERTYCODE_CAPTUREDELAY              0x5012
#define PTP_PROPERTYCODE_STILLCAPTUREMODE          0x5013
#define PTP_PROPERTYCODE_CONTRAST                  0x5014
#define PTP_PROPERTYCODE_SHARPNESS                 0x5015
#define PTP_PROPERTYCODE_DIGITALZOOM               0x5016
#define PTP_PROPERTYCODE_EFFECTMODE                0x5017
#define PTP_PROPERTYCODE_BURSTNUMBER               0x5018
#define PTP_PROPERTYCODE_BURSTINTERVAL             0x5019
#define PTP_PROPERTYCODE_TIMELAPSENUMBER           0x501A
#define PTP_PROPERTYCODE_TIMELAPSEINTERVAL         0x501B
#define PTP_PROPERTYCODE_FOCUSMETERINGMODE         0x501C
#define PTP_PROPERTYCODE_UPLOADURL          	   0x501D
#define PTP_PROPERTYCODE_ARTIST		          	   0x501E
#define PTP_PROPERTYCODE_COPYRIGHT	          	   0x501F
#define PTP_PROPERTYCODE_VENDOREXTENTION1          0xD001
#define PTP_PROPERTYCODE_VENDOREXTENTION2          0xD002

/* MTP defined Device properties*/
#define MTP_PROPERTYCODE_UNDEFINED                 0xD400
#define MTP_PROPERTYCODE_SYNCHRONIZATIONPARTNER    0xD401
#define MTP_PROPERTYCODE_DEVICEFRIENDLYNAME        0xD402

/* Device properties for Janus MTP extension */
#define MTP_PROPERTYCODE_JANUS_UNDEFINED           0xD100
#define MTP_PROPERTYCODE_JANUS_SECURETIME          0xD101
#define MTP_PROPERTYCODE_JANUS_DEVICECERTIFICATE   0xD102

/* Device properties for MTP Playback*/
#define MTP_PROPERTYCODE_PLAYBACK_RATE			   0xD410
#define MTP_PROPERTYCODE_PLAYBACK_OBJECT		   0xD411
#define MTP_PROPERTYCODE_PLAYBACK_CONT_INDEX	   0xD412
#define MTP_PROPERTYCODE_PLAYBACK_POSITION		   0xD413

#define	MTP_OBJ_PROPERTYCODE_PURCHASE_ALBTRK       0xD901

/*!
 * \def  MTP_OBJ_PROPERTYCODE_STORAGEID
 */
/*! StorageID*/
#define MTP_OBJ_PROPERTYCODE_STORAGEID             0xDC01

/*!
 * \def  MTP_OBJ_PROPERTYCODE_OBJECTFORMAT
 */
/*! Object Format*/
#define MTP_OBJ_PROPERTYCODE_OBJECTFORMAT          0xDC02

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PROTECTIONSTATUS
 */
/*! Protection Status*/
#define MTP_OBJ_PROPERTYCODE_PROTECTIONSTATUS      0xDC03

/*!
 * \def  MTP_OBJ_PROPERTYCODE_OBJECTSIZE
 */
/*! Object Size*/
#define MTP_OBJ_PROPERTYCODE_OBJECTSIZE            0xDC04

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ASSOCIATIONTYPE
 */
/*! Association Type*/
#define MTP_OBJ_PROPERTYCODE_ASSOCIATIONTYPE       0xDC05

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ASSOCIATIONDESC
 */
/*! Association Desc*/
#define MTP_OBJ_PROPERTYCODE_ASSOCIATIONDESC       0xDC06

/*!
 * \def  MTP_OBJ_PROPERTYCODE_OBJECTFILENAME
 */
/*! Object File Name*/
#define MTP_OBJ_PROPERTYCODE_OBJECTFILENAME        0xDC07

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DATECREATED
 */
/*! Date Created*/
#define MTP_OBJ_PROPERTYCODE_DATECREATED           0xDC08

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DATEMODIFIED
 */
/*! Date Modified*/
#define MTP_OBJ_PROPERTYCODE_DATEMODIFIED          0xDC09

/*!
 * \def  MTP_OBJ_PROPERTYCODE_KEYWORDS
 */
/*! Keywords*/
#define MTP_OBJ_PROPERTYCODE_KEYWORDS              0xDC0A

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PARENT
 */
/*! Parent Object*/
#define MTP_OBJ_PROPERTYCODE_PARENT                0xDC0B

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PERSISTENTGUID
 */
/*! Persistent Unique Object Identfier*/
#define MTP_OBJ_PROPERTYCODE_PERSISTENTGUID        0xDC41

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SYNCID
 */
/*! SyncID*/
#define MTP_OBJ_PROPERTYCODE_SYNCID                0xDC42

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PROPERTYBAG
 */
/*! Property Bag*/
#define MTP_OBJ_PROPERTYCODE_PROPERTYBAG           0xDC43

/*!
 * \def  MTP_OBJ_PROPERTYCODE_NAME
 */
/*! Name*/
#define MTP_OBJ_PROPERTYCODE_NAME                  0xDC44

/*!
 * \def  MTP_OBJ_PROPERTYCODE_CREATEDBY
 */
/*! Create By*/
#define MTP_OBJ_PROPERTYCODE_CREATEDBY             0xDC45

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ARTIST
 */
/*! Artist*/
#define MTP_OBJ_PROPERTYCODE_ARTIST                0xDC46

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DATEAUTHORED
 */
/*! Date Authored*/
#define MTP_OBJ_PROPERTYCODE_DATEAUTHORED          0xDC47

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DESCRIPTION
 */
/*! Description*/
#define MTP_OBJ_PROPERTYCODE_DESCRIPTION           0xDC48

/*!
 * \def  MTP_OBJ_PROPERTYCODE_URLREFERENCE
 */
/*! URL Reference*/
#define MTP_OBJ_PROPERTYCODE_URLREFERENCE          0xDC49

/*!
 * \def  MTP_OBJ_PROPERTYCODE_LANGUAGELOCALE
 */
/*! Language-Locale*/
#define MTP_OBJ_PROPERTYCODE_LANGUAGELOCALE        0xDC4A

/*!
 * \def  MTP_OBJ_PROPERTYCODE_COPYRIGHTINFO
 */
/*! Copyright Information*/
#define MTP_OBJ_PROPERTYCODE_COPYRIGHTINFO         0xDC4B

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SOURCE
 */
/*! Source*/
#define MTP_OBJ_PROPERTYCODE_SOURCE                0xDC4C

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ORIGINLOCATION
 */
/*! Orignin Location*/
#define MTP_OBJ_PROPERTYCODE_ORIGINLOCATION        0xDC4D

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DATEADDED
 */
/*! Date Added*/
#define MTP_OBJ_PROPERTYCODE_DATEADDED             0xDC4E

/*!
 * \def  MTP_OBJ_PROPERTYCODE_NONCONSUMABLE
 */
/*! Nonconsumable*/
#define MTP_OBJ_PROPERTYCODE_NONCONSUMABLE         0xDC4F

/*!
 * \def  MTP_OBJ_PROPERTYCODE_CORRUPTUNPLAYABLE
 */
/*! Corrupt or Unplayable*/
#define MTP_OBJ_PROPERTYCODE_CORRUPTUNPLAYABLE     0xDC50

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLEFORMAT
 */
/*! Sample Format*/
#define MTP_OBJ_PROPERTYCODE_SAMPLEFORMAT          0xDC81

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLESIZE
 */
/*! Sample Size*/
#define MTP_OBJ_PROPERTYCODE_SAMPLESIZE            0xDC82

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLEHEIGHT
 */
/*! Sample Height*/
#define MTP_OBJ_PROPERTYCODE_SAMPLEHEIGHT          0xDC83

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLEWIDTH
 */
/*! Sample Width*/
#define MTP_OBJ_PROPERTYCODE_SAMPLEWIDTH           0xDC84

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLEDURATION
 */
/*! Sample Duration*/
#define MTP_OBJ_PROPERTYCODE_SAMPLEDURATION        0xDC85

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLEDATA
 */
/*! Sample Data*/
#define MTP_OBJ_PROPERTYCODE_SAMPLEDATA            0xDC86

/*!
 * \def  MTP_OBJ_PROPERTYCODE_WIDTH
 */
/*! Width*/
#define MTP_OBJ_PROPERTYCODE_WIDTH                 0xDC87

/*!
 * \def  MTP_OBJ_PROPERTYCODE_HEIGHT
 */
/*! Height*/
#define MTP_OBJ_PROPERTYCODE_HEIGHT                0xDC88

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DURATION
 */
/*! Duration*/
#define MTP_OBJ_PROPERTYCODE_DURATION              0xDC89

/*!
 * \def  MTP_OBJ_PROPERTYCODE_USERRATING
 */
/*! User Rating*/
#define MTP_OBJ_PROPERTYCODE_USERRATING            0xDC8A

/*!
 * \def  MTP_OBJ_PROPERTYCODE_TRACK
 */
/*! Track*/
#define MTP_OBJ_PROPERTYCODE_TRACK                 0xDC8B

/*!
 * \def  MTP_OBJ_PROPERTYCODE_GENRE
 */
/*! Genre*/
#define MTP_OBJ_PROPERTYCODE_GENRE                 0xDC8C

/*!
 * \def  MTP_OBJ_PROPERTYCODE_CREDITS
 */
/*! Credits*/
#define MTP_OBJ_PROPERTYCODE_CREDITS               0xDC8D

/*!
 * \def  MTP_OBJ_PROPERTYCODE_LYRICS
 */
/*! Lyrics*/
#define MTP_OBJ_PROPERTYCODE_LYRICS                0xDC8E

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SUBSCRIPCONTENTID
 */
/*! Subscrip Content ID*/
#define MTP_OBJ_PROPERTYCODE_SUBSCRIPCONTENTID     0xDC8F

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PRODUCEDBY
 */
/*! Produced By*/
#define MTP_OBJ_PROPERTYCODE_PRODUCEDBY            0xDC90

/*!
 * \def  MTP_OBJ_PROPERTYCODE_USECOUNT
 */
/*! Use Count*/
#define MTP_OBJ_PROPERTYCODE_USECOUNT              0xDC91

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SKIPCOUNT
 */
/*! Skip Count*/
#define MTP_OBJ_PROPERTYCODE_SKIPCOUNT             0xDC92

/*!
 * \def  MTP_OBJ_PROPERTYCODE_LASTACCESSED
 */
/*! Last Accessed*/
#define MTP_OBJ_PROPERTYCODE_LASTACCESSED          0xDC93

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PARENTALRATING
 */
/*! Parent Rating*/
#define MTP_OBJ_PROPERTYCODE_PARENTALRATING        0xDC94

/*!
 * \def  MTP_OBJ_PROPERTYCODE_METAGENRE
 */
/*! Meta Genre*/
#define MTP_OBJ_PROPERTYCODE_METAGENRE             0xDC95

/*!
 * \def  MTP_OBJ_PROPERTYCODE_COMPOSER
 */
/*! Composer*/
#define MTP_OBJ_PROPERTYCODE_COMPOSER              0xDC96

/*!
 * \def  MTP_OBJ_PROPERTYCODE_EFFECTIVERATING
 */
/*! Effective Rating*/
#define MTP_OBJ_PROPERTYCODE_EFFECTIVERATING       0xDC97

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SUBTITLE
 */
/*! Subtitle*/
#define MTP_OBJ_PROPERTYCODE_SUBTITLE              0xDC98

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ORIGINALRELEASEDATE
 */
/*! Original Release Date*/
#define MTP_OBJ_PROPERTYCODE_ORIGINALRELEASEDATE   0xDC99

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ALBUMNAME
 */
/*! Album Name*/
#define MTP_OBJ_PROPERTYCODE_ALBUMNAME             0xDC9A

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ALBUMARTIST
 */
/*! Album Artist*/
#define MTP_OBJ_PROPERTYCODE_ALBUMARTIST           0xDC9B

/*!
 * \def  MTP_OBJ_PROPERTYCODE_MOOD
 */
/*! Mood*/
#define MTP_OBJ_PROPERTYCODE_MOOD                  0xDC9C

/*!
 * \def  MTP_OBJ_PROPERTYCODE_DRMSTATUS
 */
/*! DRM status*/
#define MTP_OBJ_PROPERTYCODE_DRMSTATUS             0xDC9D

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SUBDESCRIPTION
 */
/*! SubDescription*/
#define MTP_OBJ_PROPERTYCODE_SUBDESCRIPTION        0xDC9E

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ISCROPPED
 */
/*! Is Cropped*/
#define MTP_OBJ_PROPERTYCODE_ISCROPPED             0xDCD1

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ISCOLORCORRECTED
 */
/*! Is Color Corrected*/
#define MTP_OBJ_PROPERTYCODE_ISCOLORCORRECTED      0xDCD2

/*!
 * \def  MTP_OBJ_PROPERTYCODE_TOTALBITRATE
 */
/*! Total BitRate*/
#define MTP_OBJ_PROPERTYCODE_TOTALBITRATE          0xDE91

/*!
 * \def  MTP_OBJ_PROPERTYCODE_BITRATETYPE
 */
/*! Bitrate Type*/
#define MTP_OBJ_PROPERTYCODE_BITRATETYPE           0xDE92

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SAMPLERATE
 */
/*! Sample Rate*/
#define MTP_OBJ_PROPERTYCODE_SAMPLERATE            0xDE93

/*!
 * \def  MTP_OBJ_PROPERTYCODE_NUMBEROFCHANNELS
 */
/*! Num Of Channels*/
#define MTP_OBJ_PROPERTYCODE_NUMBEROFCHANNELS      0xDE94

/*!
 * \def  MTP_OBJ_PROPERTYCODE_AUDIOBITDEPTH
 */
/*! Audio BitDepth*/
#define MTP_OBJ_PROPERTYCODE_AUDIOBITDEPTH         0xDE95

/*!
 * \def  MTP_OBJ_PROPERTYCODE_SCANTYPE
 */
/*! Scan Type*/
#define MTP_OBJ_PROPERTYCODE_SCANTYPE              0xDE97

/*!
 * \def  MTP_OBJ_PROPERTYCODE_AUDIOWAVECODEC
 */
/*! Audio WAVE Codec*/
#define MTP_OBJ_PROPERTYCODE_AUDIOWAVECODEC        0xDE99

/*!
 * \def  MTP_OBJ_PROPERTYCODE_AUDIOBITRATE
 */
/*! Audio BitRate*/
#define MTP_OBJ_PROPERTYCODE_AUDIOBITRATE          0xDE9A

/*!
 * \def  MTP_OBJ_PROPERTYCODE_VIDEOFOURCCCODEC
 */
/*! Video FourCC Codec*/
#define MTP_OBJ_PROPERTYCODE_VIDEOFOURCCCODEC      0xDE9B

/*!
 * \def  MTP_OBJ_PROPERTYCODE_VIDEOBITRATE
 */
/*! Viedeo BitRate*/
#define MTP_OBJ_PROPERTYCODE_VIDEOBITRATE          0xDE9C

/*!
 * \def  MTP_OBJ_PROPERTYCODE_FRAMESPER1KSECONDS
 */
/*! Frames Per Thousand Seconds*/
#define MTP_OBJ_PROPERTYCODE_FRAMESPER1KSECONDS    0xDE9D

/*!
 * \def  MTP_OBJ_PROPERTYCODE_KEYFRAMEDISTANCE
 */
/*! KeyFrame Distance*/
#define MTP_OBJ_PROPERTYCODE_KEYFRAMEDISTANCE      0xDE9E

/*!
 * \def  MTP_OBJ_PROPERTYCODE_BUFFERSIZE
 */
/*! Buffer Size*/
#define MTP_OBJ_PROPERTYCODE_BUFFERSIZE            0xDE9F

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ENCODINGQUALITY
 */
/*! Encoding Quality*/
#define MTP_OBJ_PROPERTYCODE_ENCODINGQUALITY       0xDEA0

/*!
 * \def  MTP_OBJ_PROPERTYCODE_ENCODING_PROFILE
 */
/*! Encoding Profile*/
#define MTP_OBJ_PROPERTYCODE_ENCODING_PROFILE      0xDEA1

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PLAYBACK_RATE
 */
/*! Playback Rate*/
#define	MTP_OBJ_PROPERTYCODE_PLAYBACK_RATE			0xDF01

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PLAYBACK_OBJECT
 */
/*! Playback Object*/
#define	MTP_OBJ_PROPERTYCODE_PLAYBACK_OBJECT		0xDF02

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PLAYBACK_CONT_INDEX
 */
/*! Playback Content Index*/
#define	MTP_OBJ_PROPERTYCODE_PLAYBACK_CONT_INDEX	0xDF03

/*!
 * \def  MTP_OBJ_PROPERTYCODE_PLAYBACK_POSITION
 */
/*! Playback Position*/
#define	MTP_OBJ_PROPERTYCODE_PLAYBACK_POSITION		0xDF04

/* standard format codes: */
/*!
 * \def  PTP_FORMATMASK_IMAGE
 */
/*! Image*/
#define  PTP_FORMATMASK_IMAGE          0x0800
/*!
 * \def  PTP_FORMATCODE_UNDEFINED
 */
/*! Undefined*/
#define  PTP_FORMATCODE_UNDEFINED      0x3000
/*!
 * \def  PTP_FORMATCODE_ASSOCIATION
 */
/*! Association*/
#define  PTP_FORMATCODE_ASSOCIATION    0x3001

/*!
 * \def  PTP_FORMATCODE_SCRIPT
 */
/*! Script*/
#define  PTP_FORMATCODE_SCRIPT         0x3002

/*!
 * \def  PTP_FORMATCODE_EXECUTABLE
 */
/*! Executable*/
#define  PTP_FORMATCODE_EXECUTABLE     0x3003

/*!
 * \def  PTP_FORMATCODE_TEXT
 */
/*! Text*/
#define  PTP_FORMATCODE_TEXT           0x3004

/*!
 * \def  PTP_FORMATCODE_HTML
 */
/*! Html*/
#define  PTP_FORMATCODE_HTML           0x3005

/*!
 * \def  PTP_FORMATCODE_DPOF
 */
/*! Dpof*/
#define  PTP_FORMATCODE_DPOF           0x3006

/*!
 * \def  PTP_FORMATCODE_AIFF
 */
/*! Aiff*/
#define  PTP_FORMATCODE_AIFF           0x3007

/*!
 * \def  PTP_FORMATCODE_WAVE
 */
/*! Wave*/
#define  PTP_FORMATCODE_WAVE           0x3008

/*!
 * \def  PTP_FORMATCODE_MP3
 */
/*! Mp3*/
#define  PTP_FORMATCODE_MP3            0x3009

/*!
 * \def  PTP_FORMATCODE_AVI
 */
/*! avi*/
#define  PTP_FORMATCODE_AVI            0x300A

/*!
 * \def  PTP_FORMATCODE_MPEG
 */
/*! mpeg*/
#define  PTP_FORMATCODE_MPEG           0x300B

/*!
 * \def  PTP_FORMATCODE_ASF
 */
/*! asf*/
#define  PTP_FORMATCODE_ASF            0x300C

#define  PTP_FORMATCODE_IMAGE_UNDEFINED    0x3800
/*!
 * \def  PTP_FORMATCODE_IMAGE_EXIF
 */
/*! Image Exif*/
#define  PTP_FORMATCODE_IMAGE_EXIF         0x3801
/*!
 * \def  PTP_FORMATCODE_IMAGE_TIFFEP
 */
/*! Image Tiffep*/
#define  PTP_FORMATCODE_IMAGE_TIFFEP       0x3802

/*!
 * \def  PTP_FORMATCODE_IMAGE_FLASHPIX
 */
/*! Flash Pix*/
#define  PTP_FORMATCODE_IMAGE_FLASHPIX     0x3803

/*!
 * \def  PTP_FORMATCODE_IMAGE_BMP
 */
/*! BMP*/
#define  PTP_FORMATCODE_IMAGE_BMP          0x3804

/*!
 * \def  PTP_FORMATCODE_IMAGE_CIFF
 */
/*! CIFF*/
#define  PTP_FORMATCODE_IMAGE_CIFF         0x3805

/*!
 * \def  PTP_FORMATCODE_IMAGE_GIF
 */
/*! GIF*/
#define  PTP_FORMATCODE_IMAGE_GIF          0x3807

/*!
 * \def  PTP_FORMATCODE_IMAGE_JFIF
 */
/*! JFIF*/
#define  PTP_FORMATCODE_IMAGE_JFIF         0x3808

/*!
 * \def  PTP_FORMATCODE_IMAGE_PCD
 */
/*! PCD*/
#define  PTP_FORMATCODE_IMAGE_PCD          0x3809

/*!
 * \def  PTP_FORMATCODE_IMAGE_PICT
 */
/*! PICT*/
#define  PTP_FORMATCODE_IMAGE_PICT         0x380A

/*!
 * \def  PTP_FORMATCODE_IMAGE_PNG
 */
/*! PNG*/
#define  PTP_FORMATCODE_IMAGE_PNG          0x380B

/*!
 * \def  PTP_FORMATCODE_IMAGE_TIFF
 */
/*! TIFF*/
#define  PTP_FORMATCODE_IMAGE_TIFF         0x380D

/*!
 * \def  PTP_FORMATCODE_IMAGE_TIFFIT
 */
/*! TIFFIT*/
#define  PTP_FORMATCODE_IMAGE_TIFFIT       0x380E
/*!
 * \def  PTP_FORMATCODE_IMAGE_JP2
 */
/*! JP2*/
#define  PTP_FORMATCODE_IMAGE_JP2          0x380F

/*!
 * \def  PTP_FORMATCODE_IMAGE_JPX
 */
/*! JPX*/
#define  PTP_FORMATCODE_IMAGE_JPX          0x3810

/* MTP-defined Object Formats*/
/*!
 * \def  MTP_FORMATCODE_UNDEFINED_FIRMWARE
 */
/*! JPX*/
#define  MTP_FORMATCODE_UNDEFINED_FIRMWARE              0xB802

/*!
 * \def  MTP_FORMATCODE_WINDOWS_IMAGE_FORMAT
 */
/*! JPX*/
#define  MTP_FORMATCODE_WINDOWS_IMAGE_FORMAT            0xB881

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_AUDIO
 */
/*! JPX*/
#define  MTP_FORMATCODE_UNDEFINED_AUDIO                 0xB900

/*!
 * \def  MTP_FORMATCODE_WMA
 */
/*! JPX*/
#define  MTP_FORMATCODE_WMA                             0xB901

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_VIDEO
 */
/*! JPX*/
#define  MTP_FORMATCODE_UNDEFINED_VIDEO                 0xB980

/*!
 * \def  MTP_FORMATCODE_WMV
 */
/*! WMV*/
#define  MTP_FORMATCODE_WMV                             0xB981

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_COLLECTION
 */
/*! COLLECTION*/
#define  MTP_FORMATCODE_UNDEFINED_COLLECTION            0xBA00

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_MULTIMEDIA_ALBUM
 */
/*! Abstract Multimedia Album*/
#define  MTP_FORMATCODE_ABSTRACT_MULTIMEDIA_ALBUM       0xBA01

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_IMAGE_ALBUM
 */
/*! Abstract Image Album*/
#define  MTP_FORMATCODE_ABSTRACT_IMAGE_ALBUM            0xBA02

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_AUDIO_ALBUM
 */
/*! Abstract Audio Album*/
#define  MTP_FORMATCODE_ABSTRACT_AUDIO_ALBUM            0xBA03

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_VIDEO_ALBUM
 */
/*! Abstract Video Album*/
#define  MTP_FORMATCODE_ABSTRACT_VIDEO_ALBUM            0xBA04

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST
 */
/*! Abstract AV Playlist*/
#define  MTP_FORMATCODE_ABSTRACT_AV_PLAYLIST            0xBA05

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_CONTACT_GROUP
 */
/*! Abstract Contact Group*/
#define  MTP_FORMATCODE_ABSTRACT_CONTACT_GROUP          0xBA06

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_MESSAGE_FOLDER
 */
/*! Abstract Message Folder*/
#define  MTP_FORMATCODE_ABSTRACT_MESSAGE_FOLDER         0xBA07

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_CHAPTERED_PRODUCTION
 */
/*! Abstract Chaptered Production*/
#define  MTP_FORMATCODE_ABSTRACT_CHAPTERED_PRODUCTION   0xBA08

/*!
 * \def  MTP_FORMATCODE_WPL_PLAYLIST
 */
/*! WPL Playlist*/
#define  MTP_FORMATCODE_WPL_PLAYLIST                    0xBA10

/*!
 * \def  MTP_FORMATCODE_M3U_PLAYLIST
 */
/*! M3U Playlist*/
#define  MTP_FORMATCODE_M3U_PLAYLIST                    0xBA11

/*!
 * \def  MTP_FORMATCODE_MPL_PLAYLIST
 */
/*! MPL Playlist*/
#define  MTP_FORMATCODE_MPL_PLAYLIST                    0xBA12

/*!
 * \def  MTP_FORMATCODE_ASX_PLAYLIST
 */
/*! ASX Playlist*/
#define  MTP_FORMATCODE_ASX_PLAYLIST                    0xBA13

/*!
 * \def  MTP_FORMATCODE_PLS_PLAYLIST
 */
/*! PLS Playlist*/
#define  MTP_FORMATCODE_PLS_PLAYLIST                    0xBA14

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_DOCUMENT
 */
/*! Undefined Document*/
#define  MTP_FORMATCODE_UNDEFINED_DOCUMENT              0xBA80

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_DOCUMENT
 */
/*! Abstract Document*/
#define  MTP_FORMATCODE_ABSTRACT_DOCUMENT               0xBA81

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_MESSAGE
 */
/*! Undefined Message*/
#define  MTP_FORMATCODE_UNDEFINED_MESSAGE               0xBB00

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_MESSAGE
 */
/*! Abstract Message*/
#define  MTP_FORMATCODE_ABSTRACT_MESSAGE                0xBB01

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_CONTACT
 */
/*! Undefined Contact*/
#define  MTP_FORMATCODE_UNDEFINED_CONTACT               0xBB80

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_CONTACT
 */
/*! Abstract Contact*/
#define  MTP_FORMATCODE_ABSTRACT_CONTACT                0xBB81

/*!
 * \def  MTP_FORMATCODE_VCARD2
 */
/*! VCARD2*/
#define  MTP_FORMATCODE_VCARD2                          0xBB82

/*!
 * \def  MTP_FORMATCODE_VCARD3
 */
/*! VCARD3*/
#define  MTP_FORMATCODE_VCARD3                          0xBB83

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_CALENDAR_ITEM
 */
/*! Undefined Calendar Item*/
#define  MTP_FORMATCODE_UNDEFINED_CALENDAR_ITEM         0xBE00

/*!
 * \def  MTP_FORMATCODE_ABSTRACT_CALENDAR_ITEM
 */
/*! Abstract Calendar Item*/
#define  MTP_FORMATCODE_ABSTRACT_CALENDAR_ITEM          0xBE01

/*!
 * \def  MTP_FORMATCODE_VCALENDAR1
 */
/*! VCalendar1*/
#define  MTP_FORMATCODE_VCALENDAR1                      0xBE02

/*!
 * \def  MTP_FORMATCODE_UNDEFINED_WINDOWS_EXECUTABLE
 */
/*! Undefined Windows Executable*/
#define  MTP_FORMATCODE_UNDEFINED_WINDOWS_EXECUTABLE    0xBE80

/*
 * Property values definitions
 */
/*
 * Property description data set form flags definitions
 */
#define PTP_FORMFLAGS_NONE        0x00
#define PTP_FORMFLAGS_RANGE       0x01
#define PTP_FORMFLAGS_ENUM        0x02

/*
 * power states:
 */
#define PTP_POWERSTATE_DEVICEOFF     0x0000
#define PTP_POWERSTATE_SLEEP         0x0001
#define PTP_POWERSTATE_FULL          0x0002

/*
 * white balances:
 */
#define PTP_WHITEBALANCE_UNDEFINED     0x0000
#define PTP_WHILEBALANCE_MANUAL        0x0001
#define PTP_WHITEBALANCE_AUTOMATIC     0x0002
#define PTP_WHITEBALANCE_ONEPUSHAUTO   0x0003
#define PTP_WHITEBALANCE_DAYLIGHT      0x0004
#define PTP_WHITEBALANCE_FLORESCENT    0x0005
#define PTP_WHITEBALANCE_TUNGSTEN      0x0006
#define PTP_WHITEBALANCE_FLASH         0x0007

/*
 * focus modes:
 */
#define PTP_FOCUSMODE_UNDEFINED   0x0000
#define PTP_FOCUSMODE_MANUAL      0x0001
#define PTP_FOCUSMODE_AUTO        0x0002
#define PTP_FOCUSMODE_MACRO       0x0003

/*
 * focus metering:
 */
#define  PTP_FOCUSMETERING_UNDEFINED      0x0000
#define  PTP_FOCUSMETERING_CENTERSPOT     0x0001
#define  PTP_FOCUSMETERING_MULTISPOT      0x0002

/*
 * flash modes:
 */
#define PTP_FLASHMODE_UNDEFINED       0x0000
#define PTP_FLASHMODE_AUTO            0x0001
#define PTP_FLASHMODE_OFF             0x0002
#define PTP_FLASHMODE_FILL            0x0003
#define PTP_FLASHMODE_REDEYEAUTO      0x0004
#define PTP_FLASHMODE_REDEYEFILL      0x0005
#define PTP_FLASHMODE_EXTERNALSYNC    0x0006
#define PTP_FLASHMODE_MASK            0xFFF0

/*
 * exposure modes:
 */
#define PTP_EXPOSUREMODE_UNDEFINED             0x0000
#define PTP_EXPOSUREMODE_MANUALSETTING         0x0001
#define PTP_EXPOSUREMODE_AUTOPROGRAM           0x0002
#define PTP_EXPOSUREMODE_APERTUREPRIORITY      0x0003
#define PTP_EXPOSUREMODE_SHUTTERPRIORITY       0x0004
#define PTP_EXPOSUREMODE_PROGRAMCREATIVE       0x0005
#define PTP_EXPOSUREMODE_PROGRAMACTION         0x0006
#define PTP_EXPOSUREMODE_PORTRAIT              0x0007

/*
 * capturing modes:
 */
#define  PTP_CAPTUREMODE_UNDEFINED      0x0000
#define  PTP_CAPTUREMODE_NORMAL         0x0001
#define  PTP_CAPTUREMODE_BURST          0x0002
#define  PTP_CAPTUREMODE_TIMELAPSE      0x0003

/*
 * focus metering modes:
 */
#define  PTP_FOCUSMETERMODE_UNDEFINED   0x0000
#define  PTP_FOCUSMETERMODE_CENTERSPOT  0x0001
#define  PTP_FOCUSMETERMODE_MULTISPOT   0x0002

/*
 * effect modes:
 */
#define PTP_EFFECTMODE_UNDEFINED   0x0000
#define PTP_EFFECTMODE_COLOR       0x0001
#define PTP_EFFECTMODE_BW          0x0002
#define PTP_EFFECTMODE_SEPIA       0x0003

/*
 * storage types:
 */
#define PTP_STORAGETYPE_UNDEFINED       0x0000
#define PTP_STORAGETYPE_FIXEDROM        0x0001
#define PTP_STORAGETYPE_REMOVABLEROM    0x0002
#define PTP_STORAGETYPE_FIXEDRAM        0x0003
#define PTP_STORAGETYPE_REMOVABLERAM    0x0004

/*
 * storage access capabilities:
 */
#define PTP_STORAGEACCESS_RWD   0x0000
#define PTP_STORAGEACCESS_R     0x0001
#define PTP_STORAGEACCESS_RD    0x0002

/*
 * association types:
 */
#define PTP_ASSOCIATIONTYPE_UNDEFINED          0x0000
#define PTP_ASSOCIATIONTYPE_FOLDER             0x0001
#define PTP_ASSOCIATIONTYPE_ALBUM              0x0002
#define PTP_ASSOCIATIONTYPE_BURST              0x0003
#define PTP_ASSOCIATIONTYPE_HPANORAMA          0x0004
#define PTP_ASSOCIATIONTYPE_VPANORAMA          0x0005
#define PTP_ASSOCIATIONTYPE_2DPANORAMA         0x0006
#define PTP_ASSOCIATIONTYPE_ANCILLARYDATA      0x0007
#define PTP_ASSOCIATIONTYPE_MASK               0xFFF0

/*
 * protection status:
 */
#define PTP_PROTECTIONSTATUS_NONE          0x0000
#define PTP_PROTECTIONSTATUS_READONLY      0x0001

#define MTP_PROTECTIONSTATUS_NOPROTECTION       0x0000
#define MTP_PROTECTIONSTATUS_READONLY           0x0001
#define MTP_PROTECTIONSTATUS_READONLY_PERMANENT 0x0002
#define MTP_PROTECTIONSTATUS_ABSTRACT_OBJECT    0x0003

/* file system types:*/
#define PTP_FILESYSTEMTYPE_UNDEFINED       0x0000
#define PTP_FILESYSTEMTYPE_FLAT            0x0001
#define PTP_FILESYSTEMTYPE_HIERARCHICAL    0x0002
#define PTP_FILESYSTEMTYPE_DCF             0x0003

/*
 * functional modes:
 */
#define  PTP_FUNCTIONMODE_STDANDARD    0x0000
#define  PTP_FUNCTIONMODE_SLEEP        0x0001

/*
 * Get/Set
 */
#define  PTP_PROPGETSET_GETONLY    0x00
#define  PTP_PROPGETSET_GETSET     0x01
#define	 PTP_PROPGETSET_GETSET2	   0x02

/* Common Audio Sample Rate*/
#define MTP_AUDIO_SAMPLERATE_8K			 8000
#define MTP_AUDIO_SAMPLERATE_32K        32000
#define MTP_AUDIO_SAMPLERATE_CD         44100
#define MTP_AUDIO_SAMPLERATE_48K        48000
#define MTP_AUDIO_SAMPLERATE_DVD        96000

/* Common Audio Bit Rate*/
#define MTP_AUDIO_BITRATE_GSM           13000
#define MTP_AUDIO_BITRATE_G721          32000
#define MTP_AUDIO_BITRATE_G711          64000
#define MTP_AUDIO_BITRATE_128K          128000
#define MTP_AUDIO_BITRATE_CDDA          144100
#define MTP_AUDIO_BITRATE_160K          160000
#define MTP_AUDIO_BITRATE_192K          192000
#define MTP_AUDIO_BITRATE_256K          256000
#define MTP_AUDIO_BITRATE_STUDIO        384000

/* Common Metagenre defined*/
#define MTP_METAGENRE_NOT_USED                          0x0000
#define MTP_METAGENRE_GENERIC_MUSIC_AUDIO_FILE          0x0001
#define MTP_METAGENRE_GENERIC_NONMUSIC_AUDIO_FILE       0x0011
#define MTP_METAGENRE_SPOKEN_WORD_AUDIO_BOOK_FILES      0x0012
#define MTP_METAGENRE_SPOKEN_WORD_NONAUDIO_BOOK_FILES   0x0013
#define MTP_METAGENRE_SPOKEN_WORD_NEWS                  0x0014
#define MTP_METAGENRE_SPOKEN_WORD_TALK_SHOWS            0x0015
#define MTP_METAGENRE_GENERIC_VIDEO_FILE                0x0021
#define MTP_METAGENRE_NEWS_VIDEO_FILE                   0x0022
#define MTP_METAGENRE_MUSIC_VIDEO_FILE                  0x0023
#define MTP_METAGENRE_HOME_VIDEO_FILE                   0x0024
#define MTP_METAGENRE_FEATURE_FILM_VIDEO_FILE           0x0025
#define MTP_METAGENRE_TV_SHOW_VIDEO_FILE                0x0026
#define MTP_METAGENRE_TRAINING_VIDEO_FILE               0x0027
#define MTP_METAGENRE_PHOTO_MONTAGE_VIDEO_FILE          0x0028
#define MTP_METAGENRE_GENERIC_NONAUDIO_NONVIDEO_FILE    0x0030
#define MTP_METAGENRE_VENDOR_DEFINED_MASK               0x8000

/* Common NumberOfChannels defined*/
#define MTP_CHANNELS_NOT_USED                           0x0000
#define MTP_CHANNELS_MONO                               0x0001
#define MTP_CHANNELS_STEREO                             0x0002
#define MTP_CHANNELS_2DOT1                              0x0003
#define MTP_CHANNELS_3                                  0x0004
#define MTP_CHANNELS_3DOT1                              0x0005
#define MTP_CHANNELS_4                                  0x0006
#define MTP_CHANNELS_4DOT1                              0x0007
#define MTP_CHANNELS_5                                  0x0008
#define MTP_CHANNELS_5DOT1                              0x0009
#define MTP_CHANNELS_6                                  0x000A
#define MTP_CHANNELS_6DOT1                              0x000B
#define MTP_CHANNELS_7                                  0x000C
#define MTP_CHANNELS_7DOT1                              0x000D
#define MTP_CHANNELS_8                                  0x000E
#define MTP_CHANNELS_8DOT1                              0x000F
#define MTP_CHANNELS_9                                  0x0010
#define MTP_CHANNELS_9DOT1                              0x0011

/* Common ScanTypes defined*/
#define MTP_SCANTYPE_NOT_USED                           0x0000
#define MTP_SCANTYPE_PROGESSIVE                         0x0001
#define MTP_SCANTYPE_FIELDINTERLEAVEDUPPERFIRST         0x0002
#define MTP_SCANTYPE_FIELDINTERLEAVEDLOWERFIRST         0x0003
#define MTP_SCANTYPE_FIELDSINGLEUPPERFIRST              0x0004
#define MTP_SCANTYPE_FIELDSINGLELOWERFIRST              0x0005
#define MTP_SCANTYPE_MIXEDINTERLACE                     0x0006
#define MTP_SCANTYPE_MIXEDINTERLACEANDPROGRESSIVE       0x0007

/* Selected Audio Wave Formats*/
#define MTP_WAVEFORMAT_UNKNOWN                          0x0000
#define MTP_WAVEFORMAT_PCM                              0x0001
#define MTP_WAVEFORMAT_ADPCM                            0x0002
#define MTP_WAVEFORMAT_IEEEFLOAT                        0x0003
#define MTP_WAVEFORMAT_DTS                              0x0008
#define MTP_WAVEFORMAT_DRM                              0x0009
#define MTP_WAVEFORMAT_WMSP2                            0x000B
#define MTP_WAVEFORMAT_GSM610                           0x0031
#define MTP_WAVEFORMAT_MSNAUDIO                         0x0032
#define MTP_WAVEFORMAT_MPEG                             0x0050
#define MTP_WAVEFORMAT_MPEGLAYER3                       0x0055
#define MTP_WAVEFORMAT_MSAUDIO1                         0x0160
#define MTP_WAVEFORMAT_MSAUDIO2                         0x0161
#define MTP_WAVEFORMAT_MSAUDIO3                         0x0162
#define MTP_WAVEFORMAT_WMAUDIOLOSSLESS                  0x0163
#define MTP_WAVEFORMAT_WMASPDIF                         0x0164
#define MTP_WAVEFORMAT_AAC                              0xA106

/* Selected Video FOURCC Codes*/
#define MTP_VIDEOFOURCC_H263                            0x33363248  // "H263"
#define MTP_VIDEOFOURCC_H269                            0x39263248  // "H269"
#define MTP_VIDEOFOURCC_MPEG                            0x4745504D  // "MPEG"
#define MTP_VIDEOFOURCC_WMV1                            0x31564D57  // "WMV1"
#define MTP_VIDEOFOURCC_WMV2                            0x32564D57  // "WMV2"
#define MTP_VIDEOFOURCC_WMV3                            0x33564D57  // "WMV3"
#define PTP_SUBFORMATCODE_FLAC    0xa001
#define PTP_SUBFORMATCODE_APE      0xa002
#define PTP_SUBFORMATCODE_OGG      0xa003
#define PTP_SUBFORMATCODE_AA        0xa004
#define PTP_SUBFORMATCODE_RMVB    0xa005
#define PTP_SUBFORMATCODE_RM        0xa006
#define PTP_SUBFORMATCODE_JPE       0xa007
#define PTP_SUBFORMATCODE_TXT       0xa008
#define PTP_SUBFORMATCODE_DCF      0xa009
#define PTP_SUBFORMATCODE_SWF     0xa00a
#define PTP_SUBFORMATCODE_AAC      0xa00b

#define PTP_SUBFORMATCODE_JAR       0xa00C
#define PTP_SUBFORMATCODE_FLV       0xa00D
#define PTP_SUBFORMATCODE_3GP       0xa00E
#define PTP_SUBFORMATCODE_MOV      0xa00F
#define PTP_SUBFORMATCODE_MKV      0xa010
#define PTP_SUBFORMATCODE_AMR      0xa011
#define PTP_SUBFORMATCODE_MPC      0xa012
//#define PTP_SUBFORMATCODE_AIFF     0xa013
#define PTP_SUBFORMATCODE_AIFF     PTP_FORMATCODE_AIFF
#define PTP_SUBFORMATCODE_MP4      0xa014
#define PTP_SUBFORMATCODE_RA        0xa015
#define PTP_SUBFORMATCODE_RAM      0xa016
#define PTP_SUBFORMATCODE_M4A      0xa017

#endif
