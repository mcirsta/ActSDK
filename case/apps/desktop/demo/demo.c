#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>

#include "libc_extra.h"
#include "ucos/api.h"

char t[256];	/* text buffer */
int fd;		/* debug output fd */
FILE *fp;	/* debug output file pointer */

/* Dump the contents of a directory. */
void dumpdir(const char *dir, int recurse, int raw)
{
  DIR *dp = opendir(dir);
  if (!dp) {
    fprintf(fp, "failed to open %s\n", dir);
    return;
  }
  struct dirent *de;
  while ((de = readdir(dp))) {
    if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
      continue;

    if (raw) {
      /* /mnt/sdisk's file system seems to have a different dirent
         structure... */
      fprintf(fp, "%s", dir);
      uint8_t *x = (uint8_t *)de;
      int i;
      for (i = 0; i < 16; i++) {
        fprintf(fp, " %02X", x[i]);
      }
      fprintf(fp, "[%s] \n", x);
    }
    else
      fprintf(fp, "%s/%s\n", dir, de->d_name);

/*    
    if (recurse && (de->d_type & DT_DIR) && !raw) {
      char rd[256];
      sprintf(rd, "%s/%s", dir, de->d_name);
      dumpdir(rd, recurse++, raw);
    }
*/
  }
  closedir(dp);
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
  
  dumpdir("/mnt/disk0", 1, 0);	/* standard internal storage */
  dumpdir("/mnt/sdisk", 1, 1);	/* contains the OS, weird */
  dumpdir("/mnt/diska", 1, 0);	/* contains emulators and user interface */
  dumpdir("/mnt/card", 1, 0);	/* SD card*/
  
  dumpdir("/mnt", 0, 0);	/* doesn't work */
  dumpdir("/mnt/udisk", 1, 0);
  dumpdir("/mnt/sbakdisk", 1, 1);
}

int main(int argc, const char *argv[])
{
  fd = open("/mnt/card/test.txt", O_WRONLY | O_CREAT | O_TRUNC);
#define HELLO_WORLD "Hello, World!\n"
  /* Test write(). */
  write(fd, HELLO_WORLD, sizeof(HELLO_WORLD) - 1);
  /* Test sprintf(). */
  sprintf(t, "foo%d\n", 1);
  write(fd, t, strlen(t));
  close(fd);

  /* Test stdio. */
  fp = fopen("/mnt/card/ftest.txt", "w");
  setbuf(fp, NULL);
  fputs("Huhu!\n", fp);
  //findDirs();
  fclose(fp);
}
