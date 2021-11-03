#include "framebuffer.h"

#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>


FT_Library library;
FT_Face face;

bool fb_init(uint8_t **framebuffer, fb_var_screeninfo *vInfo, const char* font)
{
    int fb = open(FBDEV, O_RDWR);
    
    if (!vInfo)
    {
        perror("vInfo must not be NULL");
        exit(1);
    }

    ioctl(fb, FBIOGET_VSCREENINFO, vInfo);

    *framebuffer = mmap(0, vInfo->xres * vInfo->yres * (vInfo->bits_per_pixel / 8), PROT_READ | PROT_WRITE, MAP_SHARED, fb, (off_t)0);

    if(FT_Init_FreeType(&library))
    {
        perror("error: freetype not initialized");
        exit(1);
    }
    if (FT_New_Face(library, font, 0, &face)) {
        perror("unable to load font file");
        exit(-1);
    }


}

bool fb_close(uint8_t *framebuffer)
{
}

bool fb_drawRect(uint8_t *framebuffer, const fb_var_screeninfo *vInfo, Rect rect, uint8_t r, uint8_t g, uint8_t b)
{
    //top line
    for (int i = rect.x1; i <= rect.x2; i++)
    {
        framebuffer[(i + rect.y1 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 0] = r;
        framebuffer[(i + rect.y1 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 1] = g;
        framebuffer[(i + rect.y1 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 2] = b;
    }
    //left line
    for (int i = rect.y1; i <= rect.y2; i++)
    {
        framebuffer[(rect.x1 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 0] = r;
        framebuffer[(rect.x1 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 1] = g;
        framebuffer[(rect.x1 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 2] = b;
    }
    //bottom line
    for (int i = rect.x1; i <= rect.x2; i++)
    {
        framebuffer[(i + rect.y2 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 0] = r;
        framebuffer[(i + rect.y2 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 1] = g;
        framebuffer[(i + rect.y2 * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 2] = b;
    }
    //right line
    for (int i = rect.y1; i <= rect.y2; i++)
    {
        framebuffer[(rect.x2 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 0] = r;
        framebuffer[(rect.x2 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 1] = g;
        framebuffer[(rect.x2 + i * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 2] = b;
    }
}

bool fb_drawFilledRect(uint8_t *framebuffer, const fb_var_screeninfo *vInfo, Rect rect, uint8_t r, uint8_t g, uint8_t b)
{
    //top line
    for (int j = rect.y1; j <= rect.y2; j++)
    {
        for (int i = rect.x1; i <= rect.x2; i++)
        {
            framebuffer[(i + j * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 0] = r;
            framebuffer[(i + j * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 1] = g;
            framebuffer[(i + j * vInfo->xres) * (vInfo->bits_per_pixel / 8) + 2] = b;
        }
    }
}

//TODO: REFACTOR

bool fb_setCharSize(uint32_t height)
{
    if (FT_Set_Pixel_Sizes(face, height*3/4, height*3/4)) {
        perror("FT_Set_Pixel_Sizes failed");
        exit(-1);
    }
}


bool fb_drawChar(uint8_t* framebuffer, const fb_var_screeninfo *vInfo,uint32_t x, uint32_t y,uint32_t height,char c, uint8_t r, uint8_t g, uint8_t b)
{
    size_t fb_bytes= vInfo->bits_per_pixel/8;
    FT_Load_Char(face, c, FT_LOAD_RENDER);
    y+=height*3/4 - face->glyph->bitmap_top;
    for(int i = 0; i < face->glyph->bitmap.rows;i++)
    {
        for(int j = 0; j < face->glyph->bitmap.width;j++)
        {
            framebuffer[(y+i)*vInfo->xres*fb_bytes + (j+x)*fb_bytes] = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch* i+j);
            framebuffer[(y+i)*vInfo->xres*fb_bytes + (j+x)*fb_bytes + 1] = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch* i+j);
            framebuffer[(y+i)*vInfo->xres*fb_bytes + (j+x)*fb_bytes + 2] = *(face->glyph->bitmap.buffer + face->glyph->bitmap.pitch* i+j);
        }
    }
}

bool fb_drawText(uint8_t* framebuffer, const fb_var_screeninfo *vInfo,uint32_t x, uint32_t y,uint32_t height, const char* text,uint8_t r, uint8_t g, uint8_t b)
{
    while(*text)
    {
        fb_drawChar(framebuffer,vInfo,x,y,height,*text,r,g,b);
        text++;
        x += face->glyph->advance.x >> 6;
    }
}