#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <linux/fb.h>
#include <stdint.h>
#include <stdbool.h>

#include "shapes.h"


#define FBDEV "/dev/fb0"

typedef struct fb_var_screeninfo fb_var_screeninfo;

bool fb_init(uint8_t** framebuffer,fb_var_screeninfo* vInfo, const char* font);
bool fb_close(uint8_t* framebuffer);

bool fb_drawRect(uint8_t* framebuffer,const fb_var_screeninfo * vInfo,Rect rect, uint8_t r,uint8_t g, uint8_t b);
bool fb_drawFilledRect(uint8_t* framebuffer,const fb_var_screeninfo * vInfo,Rect rect, uint8_t r,uint8_t g, uint8_t b);

bool fb_setCharSize(uint32_t height);
bool fb_drawChar(uint8_t* framebuffer, const fb_var_screeninfo *vInfo,uint32_t x, uint32_t y, uint32_t height,char c, uint8_t r, uint8_t g, uint8_t b);
bool fb_drawText(uint8_t* framebuffer, const fb_var_screeninfo *vInfo,uint32_t x, uint32_t y,uint32_t height, const char* text,uint8_t r, uint8_t g, uint8_t b);

#endif