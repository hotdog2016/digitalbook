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
}T_DisDevINFO , *P_DisDevINFO;

int RegisterDisDev(P_DisDevINFO p_disdevinfo);
P_DisDevINFO GetDevInfo(char *dev_name);
int DisplayInit();
int FBInit();
