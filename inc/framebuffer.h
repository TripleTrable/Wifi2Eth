#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <sys/ioctl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdbool.h>

#include "shapes.h"


bool fb_init(uint8_t* framebuffer);
bool fb_close(uint8_t* framebuffer);

bool fb_drawRect(uint8_t* framebuffer,Rect rect, uint8_t r,uint8_t g, uint8_t b);


#endif