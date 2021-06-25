#ifndef __SQLITE_CONFIGURE_H_
#define __SQLITE_CONFIGURE_H_

#define SQLITE_OMIT_FLOATING_POINT          /*�ǵģ����ǵ�CPU��֧�ָ�������*/
#define SQLITE_OMIT_LOAD_EXTENSION          /*�ǵģ����ǲ���Ҫ���������Ĺ���ģ�飬��ʹ�û������ݿ⼴��*/
//#define SQLITE_DISABLE_LFS                /*�������ǲ�֧�ִ��ļ�֧�֣����������ļ�����4GB byte*/
#define SQLITE_THREADSAFE   1               /*�������ǲ���Ҫ���߳�֧�֣���ͬһʱ�����һ������������ݿ�*/
#define SQLITE_SYSTEM_MALLOC                /*�ǵģ����ǵ�ϵͳ�߱��������ڴ�������*/
#define SQLITE_MALLOC_SOFT_LIMIT 1024       /*�����޶�sqliteһ��������ڴ���С���ó���1K�����ʵ����Ӹ����ݿ��Լӿ������ٶ�*/
//#define SQLITE_OMIT_VIRTUALTABLE          /*����������Ҫ�������ݿ⣬�Լӿ����ݿ�Ĵ�ȡ�ٶ�*/
//#define SQLITE_TCL                        /*�������ǲ���ҪXML���ͳ���Ĺ��ܣ����й��ܾ�ͨ��C�������*/
//#define SQLITE_OMIT_TEMPDB                /*�ǵģ�������Ҫ��ʱ���ݿ���������������ٶ�*/
//#define SQLITE_HAS_CODEC                  /*�������ǲ���Ҫ�����ݿ���м��ܣ����Ӱ�����ǵ������ٶ�*/
//#define SQLITE_OMIT_ERRMSG_REPORT           /*�ǵģ����ǲ���Ҫ���ֱ����Խ�ʡ����ռ�*/
#define SQLITE_ENABLE_MEMORY_MANAGEMENT     /*�ǵģ����ǵ�ϵͳ�ڴ�̫С����Ҫ�����ڴ����*/
#define SQLITE_OMIT_GET_TABLE               /*�ǵģ����ǲ���Ҫʹ�ø��ļ��ڶ���ı����*/
#define SQLITE_INNER_ALLOC_ENABLE 0        /*���������ʹ���ڲ����ڴ�������*/

#define SQLITE_LIMIT_HEAP_SIZE (250*1024)   /* �����������ݿ�ֻ��ʹ�øô�С���ڴ���Դ */
#define TEMP_STORE 2        /*�������ʱ�������������ڴ���*/
#define SQLITE_ENABLE_UPDATE_DELETE_LIMIT  1

#endif /*#define __SQLITE_CONFIGURE_H_*/
