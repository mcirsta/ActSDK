/*!
 * semihosting for actions platforms(mips based)
 *
 * sangwei@actions-semi.com
 */
#ifndef __SEMIHOSTING_H__
#define __SEMIHOSTING_H__

/* The following flags are defined to be independent of the host
 as well as the target side implementation of these constants.
 All constants are defined with a leading FILEIO_ in the name
 to allow the usage of these constants together with the
 corresponding implementation dependent constants in one module. */
/* open(2) flags */
#define FILEIO_O_RDONLY           0x0
#define FILEIO_O_WRONLY           0x1
#define FILEIO_O_RDWR             0x2
#define FILEIO_O_APPEND           0x8
#define FILEIO_O_CREAT          0x200
#define FILEIO_O_TRUNC          0x400
#define FILEIO_O_EXCL           0x800
#define FILEIO_O_SUPPORTED		(FILEIO_O_RDONLY | FILEIO_O_WRONLY| \
								FILEIO_O_RDWR   | FILEIO_O_APPEND| \
								FILEIO_O_CREAT  | FILEIO_O_TRUNC| \
								FILEIO_O_EXCL)

/* mode_t bits */
#define FILEIO_S_IFREG        0100000
#define FILEIO_S_IFDIR         040000
#define FILEIO_S_IFCHR         020000
#define FILEIO_S_IRUSR           0400
#define FILEIO_S_IWUSR           0200
#define FILEIO_S_IXUSR           0100
#define FILEIO_S_IRWXU           0700
#define FILEIO_S_IRGRP            040
#define FILEIO_S_IWGRP            020
#define FILEIO_S_IXGRP            010
#define FILEIO_S_IRWXG            070
#define FILEIO_S_IROTH             04
#define FILEIO_S_IWOTH             02
#define FILEIO_S_IXOTH             01
#define FILEIO_S_IRWXO             07
#define FILEIO_S_SUPPORTED         (FILEIO_S_IFREG|FILEIO_S_IFDIR|  \
									FILEIO_S_IRWXU|FILEIO_S_IRWXG|  \
									FILEIO_S_IRWXO)

/* lseek(2) flags */
#define FILEIO_SEEK_SET             0
#define FILEIO_SEEK_CUR             1
#define FILEIO_SEEK_END             2

/* errno values */
#define FILEIO_EPERM                1
#define FILEIO_ENOENT               2
#define FILEIO_EINTR                4
#define FILEIO_EIO                  5
#define FILEIO_EBADF                9
#define FILEIO_EACCES              13
#define FILEIO_EFAULT              14
#define FILEIO_EBUSY               16
#define FILEIO_EEXIST              17
#define FILEIO_ENODEV              19
#define FILEIO_ENOTDIR             20
#define FILEIO_EISDIR              21
#define FILEIO_EINVAL              22
#define FILEIO_ENFILE              23
#define FILEIO_EMFILE              24
#define FILEIO_EFBIG               27
#define FILEIO_ENOSPC              28
#define FILEIO_ESPIPE              29
#define FILEIO_EROFS               30
#define FILEIO_ENOSYS		       88
#define FILEIO_ENAMETOOLONG        91
#define FILEIO_EUNKNOWN          9999

/* limits */
#define FILEIO_INT_MIN                  -2147483648L
#define FILEIO_INT_MAX                   2147483647L
#define FILEIO_UINT_MAX                 4294967295UL
#define FILEIO_LONG_MIN       -9223372036854775808LL
#define FILEIO_LONG_MAX        9223372036854775807LL
#define FILEIO_ULONG_MAX     18446744073709551615ULL

/* 调用号 */
#define SYSCALL_ID_ERROR            0x00
#define SYSCALL_ID_OPEN             0x01
#define SYSCALL_ID_READ             0x02
#define SYSCALL_ID_WRITE            0x03
#define SYSCALL_ID_CLOSE            0x04
#define SYSCALL_ID_LSEEK            0x05

#define SYSCALL_ID_SYSTEM           0x10

#define SYSCALL_ID_USDKINFO         0x20
#define SYSCALL_ID_RPC              0x21

#ifndef _ASSEMBLER_
/*
 Synopsis:
 int open(const char *pathname, int flags, mode_t mode);

 Request:
 Fopen,pathptr/len,flags,mode

 flags is the bitwise or of the following values:

 O_CREAT
 If the file does not exist it will be created. The host rules apply as far as file ownership and time stamps are concerned.
 O_EXCL
 When used with O_CREAT, if the file already exists it is an error and open() fails.
 O_TRUNC
 If the file already exists and the open mode allows writing (O_RDWR or O_WRONLY is given) it will be truncated to length 0.
 O_APPEND
 The file is opened in append mode.
 O_RDONLY
 The file is opened for reading only.
 O_WRONLY
 The file is opened for writing only.
 O_RDWR
 The file is opened for reading and writing. Each other bit is silently ignored.

 mode is the bitwise or of the following values:

 S_IRUSR
 User has read permission.
 S_IWUSR
 User has write permission.
 S_IRGRP
 Group has read permission.
 S_IWGRP
 Group has write permission.
 S_IROTH
 Others have read permission.
 S_IWOTH
 Others have write permission. Each other bit is silently ignored.

 Return value:
 open returns the new file descriptor or -1 if an error occured.

 Errors:

 EEXIST
 pathname already exists and O_CREAT and O_EXCL were used.
 EISDIR
 pathname refers to a directory.
 EACCES
 The requested access is not allowed.
 ENAMETOOLONG
 pathname was too long.
 ENOENT
 A directory component in pathname does not exist.
 ENODEV
 pathname refers to a device, pipe, named pipe or socket.
 EROFS
 pathname refers to a file on a read-only filesystem and write access was requested.
 EFAULT
 pathname is an invalid pointer value.
 ENOSPC
 No space on device to create the file.
 EMFILE
 The process already has the maximum number of files open.
 ENFILE
 The limit on the total number of files open on the system has been reached.
 EINTR
 The call was interrupted by the user.
 */
extern int remote_open(char *name, int flags, int mode);

/*
 Synopsis:
 int read(int fd, void *buf, unsigned int count);

 Request:
 Fread,fd,bufptr,count

 Return value:
 On success, the number of bytes read is returned.
 Zero indicates end of file.  If count is zero, read
 returns zero as well.  On error, -1 is returned.

 Errors:

 EBADF
 fd is not a valid file descriptor or is not open for reading.
 EFAULT
 buf is an invalid pointer value.
 EINTR
 The call was interrupted by the user.
 */
extern int remote_read(int fd, unsigned char *buf, unsigned int count);

/*
 Synopsis:
 int write(int fd, const void *buf, unsigned int count);

 Request:
 Fwrite,fd,bufptr,count

 Return value:
 On success, the number of bytes written are returned.
 Zero indicates nothing was written.  On error, -1
 is returned.

 Errors:

 EBADF
 fd is not a valid file descriptor or is not open for writing.
 EFAULT
 buf is an invalid pointer value.
 EFBIG
 An attempt was made to write a file that exceeds the host specific maximum file size allowed.
 ENOSPC
 No space on device to write the data.
 EINTR
 The call was interrupted by the user.
 */
extern int remote_write(int fd, unsigned char *buf, unsigned int count);

/*
 Synopsis:
 int close(int fd);

 Request:
 Fclose,fd

 Return value:
 close returns zero on success, or -1 if an error occurred.

 Errors:

 EBADF
 fd isn't a valid open file descriptor.
 EINTR
 The call was interrupted by the user.
 */
extern int remote_close(int fd);

/*
 Synopsis:
 long lseek (int fd, long offset, int flag);

 Request:
 Flseek,fd,offset,flag

 flag is one of:

 SEEK_SET
 The offset is set to offset bytes.
 SEEK_CUR
 The offset is set to its current location plus offset bytes.
 SEEK_END
 The offset is set to the size of the file plus offset bytes.

 Return value:
 On success, the resulting unsigned offset in bytes from
 the beginning of the file is returned.  Otherwise, a
 value of -1 is returned.

 Errors:

 EBADF
 fd is not a valid open file descriptor.
 ESPIPE
 fd is associated with the GDB console.
 EINVAL
 flag is not a proper value.
 EINTR
 The call was interrupted by the user.
 */
extern int remote_lseek(int fd, int offset, int pos);

/*
 Synopsis:
 int system(const char *command);

 Request:
 Fsystem,commandptr/len

 Return value:
 The value returned is -1 on error and the return status
 of the command otherwise.  Only the exit status of the
 command is returned, which is extracted from the hosts
 system return value by calling WEXITSTATUS(retval).
 In case /bin/sh could not be executed, 127 is returned.

 Errors:

 EINTR
 The call was interrupted by the user.
 */
extern int remote_system(const char *command);

/*
 Synopsis:
 int remote_call(const char *funcname, int paranum, ...);

 parameter:
 funcname: 远程函数名
 paranum: 远程函数参数个数，必填！

 Request:
 Frpc,strptr/len

 Return value:
 函数返回值

 NOTE!
 限制：只能传递整形，只能传值，传址无效；
 最多只能传递6个整形参数

 */
extern int remote_call(const char *funcname, int paranum, ...);

int remote_printf(const char *str);

#endif

#ifndef _ASSEMBLER_

typedef struct
{
    unsigned int time;
    unsigned long long cclk_tick;
} extra_arg_t;

extern int remote_send_usdk_info(void *ptr, unsigned int length);

#endif

#endif /* __SEMIHOSTING_H__ */
