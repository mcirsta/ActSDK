/**
* head file for key_buf_queue.c
* liwei, 2006,09
*/

#ifndef __KEY_BUF_QUEUE__
#define __KEY_BUF_QUEUE__


#define KEY_MESSAGE_BUF_SIZE 10


void qstore(unsigned int q);
unsigned int qretrieve(void);
int is_empty(void);
int is_full(void);
unsigned int sizeof_key_buf(void);

#endif  //_KEY_BUF_QUEUE_


