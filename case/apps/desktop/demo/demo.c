#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stat.h>

#include "libc_extra.h"
#include "ucos/api.h"
#include "ucos/syscalls_general.h"
#include "app_define.h"

static char buffer[10240];
int fd;		/* debug output fd */
FILE *fp;	/* debug output file pointer */


static const clk_domain_t s_clk_domain =
{ CLK_LOAD_MIN, CLK_LOAD_HIGH_0 };


/* Dump the contents of a directory. */
void dumpdir(const char *dir, const char *fName,int recurse, int raw)
{ 
  fp = fopen(fName, "w");
  
  DIR *dp = opendir(dir);
  if (!dp) {
    fputs("failed to open", fp);
    return;
  }
  struct dirent *de;
  while ((de = readdir(dp))) {
    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
      continue;

    if (raw) {
      /* /mnt/sdisk's file system seems to have a different dirent
         structure... */
      fputs(dir, fp);
//       uint8_t *x = (uint8_t *)de;
//       int i;
//       for (i = 0; i < 16; i++) {
//         fprintf(fp, " %02X", x[i]);
//       }
//       fprintf(fp, "[%s] \n", x);
    }
    else
    {
        fputs(dir, fp);
        fputc('/', fp);
        fputs(de->d_name, fp);
        fputc('\n', fp);
    }
/*    
    if (recurse && (de->d_type & DT_DIR) && !raw) {
      char rd[256];
      sprintf(rd, "%s/%s", dir, de->d_name);
      dumpdir(rd, recurse++, raw);
    }
*/
  }
  closedir(dp);
  fclose(fp);
}


/* Dump the contents of a directory. */
void dumpSysDir(const char *dir, const char *fName,int recurse, int raw)
{ 
  fp = fopen(fName, "w");
  
  int dp = sys_open(dir, O_RDONLY, 0);
  if (dp<0) {
    fputs("failed to open", fp);
    return;
  }
  struct dirent *de;
  while ((de = sys_readdir(dp, 0 , de, 100))) {
    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
      continue;

    fputs(dir, fp);
    fputc('/', fp);
    fputs(de->d_name, fp);
    fputc('\n', fp);

/*    
    if (recurse && (de->d_type & DT_DIR) && !raw) {
      char rd[256];
      sprintf(rd, "%s/%s", dir, de->d_name);
      dumpdir(rd, recurse++, raw);
    }
*/
  }
  sys_close(dp);
  fclose(fp);
}

/* Dump the contents of various file systems. */
void findDirs(void)
{
/*
    Confirmed present on the Powkiddy J6 by decompiling binaries on SD card
    /mnt/card                 - external SD card      -   /mnt/card/apps/ebook/ebook.app
    /mnt/diska                - system libs and apps  -   /mnt/diska/lib/fusion.so
    /mnt/sdisk                - emulator libs         -   /mnt/sdisk/libmmmfp.so
    /mnt/disk0                - alternative to sdisk location for emulators  -   /mnt/disk0/libf1.so
*/
  
  dumpdir("/mnt/diska", "/mnt/card/diska.txt", 1, 0);
  dumpdir("/mnt/diska/apps", "/mnt/card/apps.txt" , 1, 0);


  dumpdir("/mnt/diska/lib", "/mnt/card/lib.txt" ,1, 0);
  dumpdir("/mnt/diska/lib/commonui", "/mnt/card/commonui.txt" ,1, 0);
  dumpdir("/mnt/diska/sys", "/mnt/card/sys.txt" , 1, 0);
  
  dumpdir("/mnt/diska/apps/browser", "/mnt/card/browser.txt" , 1, 0);
    dumpdir("/mnt/diska/apps/launcher", "/mnt/card/launcher.txt" , 1, 0);
    dumpdir("/mnt/diska/apps/pl_gener", "/mnt/card/pl_gener.txt" , 1, 0);
    dumpdir("/mnt/diska/apps/setting", "/mnt/card/setting.txt" , 1, 0);
    dumpdir("/mnt/diska/apps/usb", "/mnt/card/usb.txt" , 1, 0);
    
    
        
    dumpdir("/mnt/sdisk", "/mnt/card/sdisk.txt" , 1, 0);
}

void copyFile(const char* source, const char* dest)
{
    fp = fopen("/mnt/card/log.txt", "w");
    fputs("Start of log\n", fp);

    int dump = open(dest, O_WRONLY | O_CREAT | O_TRUNC);
    if (dump < 0) {
            fputs("error when open dest\n", fp);
        /* Handle error */
    }

    int flash = open(source, O_RDONLY);
    if (flash < 0) {
        fputs("error when open source\n", fp);
        /* Handle error */
    }

    int rd = 0;
    do {
        rd = read(flash, buffer, 1024);
        if (rd < 0) {
           fputs("error when read\n", fp);
            break;
            /* Handle error */
        }
        else if (rd == 0) {
            fputs("done with read\n", fp);
            break;  
        }
        sys_write(dump, buffer, rd);
    } while(rd>0);

    fputs("All done\n", fp);
    sys_close(dump);
    sys_close(flash);
    fclose(fp);
}

/* The following macros are defined in
* #define DEVICE_FLASH    "/dev/flash"
* #define DEVICE_CARD     "/dev/card"
* #define CARD_ROOT       "/mnt/card"
*/



void copySysFile(const char* source, const char* dest)
{
    fp = fopen("/mnt/card/log.txt", "w");
    fputs("Start of log\n", fp);

    int dump = sys_open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (dump < 0) {
            fputs("error when open dest\n", fp);
        /* Handle error */
    }

    int flash = sys_open(source, O_RDONLY, 0);
    if (flash < 0) {
        fputs("error when open source\n", fp);
        /* Handle error */
    }

    int rd = 0;
    do {
        rd = sys_read(flash, buffer, 1024);
        if (rd < 0) {
           fputs("error when read\n", fp);
            break;
            /* Handle error */
        }
        else if (rd == 0) {
            fputs("done with read\n", fp);
            break;  
        }
        sys_write(dump, buffer, rd);
    } while(rd>0);

    fputs("All done\n", fp);
    sys_close(dump);
    sys_close(flash);
    fclose(fp);
}

void doStat(const char* target, const char* logName)
{
    fp = fopen(logName, "w");
    struct stat statbuf;
    if(sys_stat(target, &statbuf) == 0)
    {
        fputs("Sys stat success\n", fp);
        fprintf(fp, "Type: %d\n" , statbuf.st_mode);
    }
    else
    {
        fputs("Sys stat failure\n", fp);
    }
    
    if(stat(target, &statbuf) == 0)
    {
        fputs("Stat success\n", fp);
        fprintf(fp, "Type: %d\n" , statbuf.st_mode);
    }
    else
    {
        fputs("Sys stat failure\n", fp);
    }
    
    struct statfs statfsbuf;
    if(sys_statfs(target, &statfsbuf) == 0)
    {
        fputs("Statfs success\n", fp);
        fprintf(fp, "FS type: %ld\n" , statfsbuf.f_type);
        if(statfsbuf.f_type == 0)
        {
           fputs("No f_type\n", fp);
        }
        fprintf(fp, "Avail blocks: %d\n" , statfsbuf.f_bavail);
        fprintf(fp, "Blocks: %ld\n" , statfsbuf.f_blocks);
    }
    else
    {
        fputs("Statfs failure\n", fp);
    }
    
    fclose(fp);
}

int main(int argc, const char *argv[])
{
    fp = fopen("/mnt/card/test.txt", "w");
    fputs("Hello console", fp);
    fclose(fp);
    
    copyFile("/mnt/diska/sys/config.bin", "/mnt/card/config.bin");
    copyFile("/mnt/diska/lib/applib.so", "/mnt/card/applib.so");
    copyFile("/mnt/diska/lib/style.so", "/mnt/card/style.so");
    copyFile("/mnt/diska/lib/fusion.so", "/mnt/card/fusion.so");
    copyFile("/mnt/diska/apps/config.app", "/mnt/card/config.app");
    copyFile("/mnt/diska/apps/m_engine.app", "/mnt/card/m_engine.app");
    copyFile("/mnt/diska/apps/manager.app", "/mnt/card/manager.app");
    copyFile("/mnt/diska/apps/msg_serv.app", "/mnt/card/msg_serv.app");
    copyFile("/mnt/diska/apps/browser/browser.app", "/mnt/card/browser.app");
    copyFile("/mnt/diska/apps/browser/browser.desktop", "/mnt/card/browser.desktop");
    copyFile("/mnt/diska/apps/launcher/lang.desktop", "/mnt/card/lang.desktop");
    copyFile("/mnt/diska/apps/launcher/lang_s480272.desktop", "/mnt/card/lang_s480272.desktop");
    copyFile("/mnt/diska/apps/launcher/launcher.app", "/mnt/card/launcher.app");
    copyFile("/mnt/diska/apps/launcher/tools.desktop", "/mnt/card/tools.desktop");
    copyFile("/mnt/diska/apps/pl_gener/pl_gener.app", "/mnt/card/pl_gener.app");
    copyFile("/mnt/diska/apps/usb/usb.app", "/mnt/card/usb.app");
    copyFile("/mnt/diska/lib/commonui/commonui.so", "/mnt/card/commonui.so");

    copySysFile("/mnt/sdisk/libgb1.so", "/mnt/card/libgb1.so");
    copySysFile("/mnt/sdisk/libemu.so", "/mnt/card/libemu.so");
    
    doStat("/mnt/sdisk/libgb1.so", "/mnt/card/stat_libgb1.txt");
    doStat("/dev/flash", "/mnt/card/stat_flash.txt");
    doStat("/dev/flashu", "/mnt/card/stat_flashu.txt");
    doStat("/dev/flashau", "/mnt/card/stat_flasha.txt");
    doStat("/mnt/diska/lib/style.so", "/mnt/card/stat_style.txt");
    
    findDirs();
    
    return 0;

}
