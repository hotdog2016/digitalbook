#ifndef __DISDEV__H
#define __DISDEV__H
typedef struct DisDevINFO {
    char *name;
    int xres;
    int yres;
    int lcd_width;
    int lcd_size;
    int pixel_bytes;
    unsigned char * fb_addr;
    void (*ShowPixel)(int x, int y, unsigned int color);
    int (*DeviceInit)();
    int (*CleanScreen)();
    struct  DisDevINFO * next;
}T_DisDevINFO , *PT_DisDevINFO;

int RegisterDisDev(PT_DisDevINFO p_disdevinfo);
PT_DisDevINFO GetDevInfo(char *dev_name);
int DisplayInit();
int FBInit();


#endif

