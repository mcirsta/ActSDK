#ifndef _ACT_PICTUREFLOW_EXTERNAL_
#define _ACT_PICTUREFLOW_EXTERNAL_




#define SGN printf("%s %d\n",__FILE__,__LINE__);




// 显示模式，循环和普通
typedef enum DISPLAY_MODE {DISPLAY_LOOP=1, \
                            DISPLAY_NORMAL=2} \
    display_mode_t;

// 动画模式, 是否间隔均匀的渲染动画过程每一帧
typedef enum ANIMATION_MODE {ANIMATION_UNIFORM=1, \
                           ANIMATION_DEFAULT=2} \
    animation_mode_t;
    
// image to be mapped into slide
typedef struct RImage_s
{
    unsigned long width;
    unsigned long height;
    unsigned long reserved;
    unsigned char *data;
    unsigned char bpp;
} RImage_t;

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//-------------------------------------------------------
extern int pf_setSlideSize(void *pf, int width, int height);
extern int pf_setSlideCount(void *pf, int count);
extern int pf_resize(void *pf, int w, int h);
extern int pf_getSlideCount(void *pf);
extern int pf_setZoom(void *pf, int z);
extern int pf_setSlideSpace(void *pf, int space);
extern int pf_setSlide(void *pf, int index, RImage_t *image);
extern int pf_setBlankSlide(void *pf, RImage_t *image);
extern int pf_activeSlide(void *pf, int index);

extern int pf_showPrevious(void *pf);
extern int pf_showNext(void *pf);
extern int pf_showSlide(void *pf, int index);
extern int pf_showTail(void *pf);
extern int pf_showHead(void *pf);

extern int pf_open(void **pf);
extern void *pf_getCanvasBuffer(void *pf);
extern int pf_setCanvasBuffer(void *pf, void *ptr);
extern int pf_isOnAnimation(void *pf);
extern int pf_render(void *pf);
extern void pf_close(void *pf);

// added on 20080321
extern int pf_changeMode(void *pf, display_mode_t mode);
extern int pf_setSideSlideCount(void *pf ,int num);
extern int pf_setCanvasImage(void *pf, RImage_t *image);
extern int pf_setCanvasMonoColor(void *pf, unsigned long color);

// added on 20080404
extern int pf_setSpeed(void *pf, int speed);  // speed should be 1~7;

// added on 20080411
//extern int pf_setAnimationMode(void *pf, animation_mode_t animation_mode);
//-------------------------------------------------------
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!






//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
// 调用者必须实现以下函数，确保静态库能准确调用
//内存分配
extern void *coverflow_malloc(unsigned int m_size);

//内存释放
extern void coverflow_free(void *ptr);
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1


#endif
