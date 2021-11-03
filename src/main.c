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
    struct fb_var_screeninfo varInfo;
    const char* font = "/usr/share/fonts/inconsolata/Inconsolata.otf";

    

    uint8_t* display;
    fb_init(&display,&varInfo,font);
    uint32_t fb_width = varInfo.xres;
    uint32_t fb_height = varInfo.yres;
    uint32_t fb_bytes = varInfo.bits_per_pixel/8;
    memset(display,0x44, varInfo.xres * varInfo.yres * varInfo.bits_per_pixel/8);
     Rect r;
     r.x1 = r.y1 = 100;
     r.x2 = r.y2 = 200;
     Rect g;
     g.x1 = g.y1 = 200;
     g.x2 = g.y2 = 300;
     Rect b;
     b.x1 = b.y1 = 300;
     b.x2 = b.y2 = 400;
     fb_drawRect(display,&varInfo,r,0xFF,0x0,0x0);
     fb_drawFilledRect(display,&varInfo,g,0x0,0xFF,0x0);
     fb_drawRect(display,&varInfo,b,0x0,0x0,0xFF);

    //TODO: REFACTOR

    fb_setCharSize(50);

    fb_drawChar(display,&varInfo,500,500,10,'A',255,255,255);


        


//     int mouse = open("/dev/input/event0",O_RDONLY);
//     struct input_event ev[64];
//     while(1) {
//     size_t rb = read(mouse,ev,sizeof(struct input_event)*64);

// if (rb < (int) sizeof(struct input_event)) {
//     perror("evtest: short read");
//     exit (1);
// }

// int pos_x = 0;
// int pos_y = 0;
// for (int yalv = 0;
//      yalv < (int) (rb / sizeof(struct input_event));
//      yalv++)
// {
    
//     //if (EV_KEY == ev[yalv].type)
//     if(ev[yalv].type == EV_SYN)
//     {
//         printf("----------------------\n");
//     }
//     else if (ev[yalv].type == EV_KEY)
//     {
//         if(ev[yalv].code == BTN_TOUCH)
//         {
//             printf("Touchpad %s\n",ev[yalv].value ? "touched" : "released");
//         }
//     }
//     else if (ev[yalv].type == EV_ABS)
//     {
//         if(ev[yalv].code == ABS_X)
//         {
//             pos_x = (int)round(ev[yalv].value/(double)X_AXIS_SCALE*fb_width);
//             printf("X-axis pos: %d\n",pos_x);
//         }
//         else if(ev[yalv].code == ABS_Y)
//         {
//             pos_y = (int)round(ev[yalv].value/(double)Y_AXIS_SCALE*fb_height);
//             printf("Y-axis pos: %d\n",pos_y);
//         }
//        display[pos_x * fb_bytes + pos_y * fb_width * fb_bytes] = 0xFF; 
//        display[pos_x * fb_bytes + 1 + pos_y * fb_width * fb_bytes] = 0xFF; 
//        display[pos_x * fb_bytes + 2 + pos_y * fb_width * fb_bytes] = 0xFF; 

//     }
//     else
//     {
//         printf("%ld.%06ld ",
//                ev[yalv].time.tv_sec,
//                ev[yalv].time.tv_usec);
//         printf("type %d code %d value %d\n",
//                ev[yalv].type,
//                ev[yalv].code, ev[yalv].value);
//     }

// }
// }


}
