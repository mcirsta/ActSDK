#ifndef __SQLITE_CONFIGURE_H_
#define __SQLITE_CONFIGURE_H_

#define SQLITE_OMIT_FLOATING_POINT          /*是的，我们的CPU不支持浮点运算*/
#define SQLITE_OMIT_LOAD_EXTENSION          /*是的，我们不需要加载其它的功能模块，仅使用基本数据库即可*/
//#define SQLITE_DISABLE_LFS                /*不，我们不支持大文件支持，我们最大的文件仅有4GB byte*/
#define SQLITE_THREADSAFE   1               /*不，我们不需要多线程支持，在同一时间仅有一个任务访问数据库*/
#define SQLITE_SYSTEM_MALLOC                /*是的，我们的系统具备基本的内存管理机制*/
#define SQLITE_MALLOC_SOFT_LIMIT 1024       /*这里限定sqlite一次申请的内存块大小不得超过1K，但适当增加该数据可以加快运行速度*/
//#define SQLITE_OMIT_VIRTUALTABLE          /*不，我们需要虚拟数据库，以加快数据库的存取速度*/
//#define SQLITE_TCL                        /*不，我们不需要XML解释程序的功能，所有功能均通过C语言完成*/
//#define SQLITE_OMIT_TEMPDB                /*是的，我们需要临时数据库来帮助我们提高速度*/
//#define SQLITE_HAS_CODEC                  /*不，我们不需要对数据库进行加密，这会影响我们的运行速度*/
//#define SQLITE_OMIT_ERRMSG_REPORT           /*是的，我们不需要文字报错，以节省代码空间*/
#define SQLITE_ENABLE_MEMORY_MANAGEMENT     /*是的，我们的系统内存太小，需要进行内存管理*/
#define SQLITE_OMIT_GET_TABLE               /*是的，我们不需要使用该文件内定义的表操作*/
#define SQLITE_INNER_ALLOC_ENABLE 0        /*在这里，我们使用内部的内存管理机制*/

#define SQLITE_LIMIT_HEAP_SIZE (250*1024)   /* 我们限制数据库只能使用该大小的内存资源 */
#define TEMP_STORE 2        /*在这里，临时表和索引存放于内存中*/
#define SQLITE_ENABLE_UPDATE_DELETE_LIMIT  1

#endif /*#define __SQLITE_CONFIGURE_H_*/
