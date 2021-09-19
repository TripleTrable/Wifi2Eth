#include "framebuffer.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include <sys/mman.h>

#include <signal.h>
#include <sys/time.h>
#include <linux/input.h>



#define X_AXIS_SCALE 16383

#define Y_AXIS_SCALE 9599

int main()
{
    int fdScreen = open("/dev/fb0",O_RDWR);

    struct fb_var_screeninfo varInfo;
    ioctl(fdScreen,FBIOGET_VSCREENINFO,&varInfo);

    int fb_width = varInfo.xres;
    int fb_height = varInfo.yres;
    int fb_bpp = varInfo.bits_per_pixel;
    int fb_bytes = fb_bpp / 8;


    unsigned char* display = mmap(0,fb_width * fb_height*fb_bytes,PROT_READ | PROT_WRITE,MAP_SHARED,fdScreen,(off_t)0);
    memset(display,0,fb_width * fb_height*fb_bytes);

    for (int j = 0; j < 100; j++)
        for (int i = 0; i < 100; i++)
        {
            display[(100+j) * fb_bytes + (100+i) * fb_width * fb_bytes] = 0xff; 
            display[(100+j) * fb_bytes + 1 + (100+i) * fb_width * fb_bytes] = 0xff; 
            display[(100+j) * fb_bytes + 2 + (100+i) * fb_width * fb_bytes] = 0xff; 

        }




    int mouse = open("/dev/input/event0",O_RDONLY);
    struct input_event ev[64];
    while(1) {
    size_t rb = read(mouse,ev,sizeof(struct input_event)*64);

if (rb < (int) sizeof(struct input_event)) {
    perror("evtest: short read");
    exit (1);
}

int pos_x = 0;
int pos_y = 0;
for (int yalv = 0;
     yalv < (int) (rb / sizeof(struct input_event));
     yalv++)
{
    
    //if (EV_KEY == ev[yalv].type)
    if(ev[yalv].type == EV_SYN)
    {
        printf("----------------------\n");
    }
    else if (ev[yalv].type == EV_KEY)
    {
        if(ev[yalv].code == BTN_TOUCH)
        {
            printf("Touchpad %s\n",ev[yalv].value ? "touched" : "released");
        }
    }
    else if (ev[yalv].type == EV_ABS)
    {
        if(ev[yalv].code == ABS_X)
        {
            pos_x = (int)round(ev[yalv].value/(double)X_AXIS_SCALE*fb_width);
            printf("X-axis pos: %d\n",pos_x);
        }
        else if(ev[yalv].code == ABS_Y)
        {
            pos_y = (int)round(ev[yalv].value/(double)Y_AXIS_SCALE*fb_height);
            printf("Y-axis pos: %d\n",pos_y);
        }
       display[pos_x * fb_bytes + pos_y * fb_width * fb_bytes] = 0xFF; 
       display[pos_x * fb_bytes + 1 + pos_y * fb_width * fb_bytes] = 0xFF; 
       display[pos_x * fb_bytes + 2 + pos_y * fb_width * fb_bytes] = 0xFF; 

    }
    else
    {
        printf("%ld.%06ld ",
               ev[yalv].time.tv_sec,
               ev[yalv].time.tv_usec);
        printf("type %d code %d value %d\n",
               ev[yalv].type,
               ev[yalv].code, ev[yalv].value);
    }

}
}


}
