
#include "i2c.h"

/*!
 *    i2c …Ë±∏÷∏’Î \n  
 */
void * pi2c_dev=NULL; 

int fm_enter_standby(int stanb)
{
    int flag = -1;

    int buf= stanb;
    unsigned int wt_addr = 0xc1;
    
    pi2c_dev=i2c_open(NORMAL_SPEED_I2C,0);

    i2c_write(pi2c_dev, wt_addr, &buf, 1);
   
    i2c_close(pi2c_dev);   
}

