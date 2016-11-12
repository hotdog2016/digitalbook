#include <mydebug.h>
#include <disdev.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>

static void LcdPutPixel(int x, int y, unsigned int color);
static int LcdInit();
static int LcdCleanScreen();
static T_DisDevINFO gt_fbinfo ={
    .name         = "fb",
    .ShowPixel    = LcdPutPixel,
    .DeviceInit   = LcdInit,
    .CleanScreen  = LcdCleanScreen,
};

static int LcdCleanScreen()
{
    memset(gt_fbinfo.fb_addr , 0 , gt_fbinfo.lcd_size);
    return 0;
}
static void LcdPutPixel(int x, int y, unsigned int color)
{
	unsigned char *pen_8 = gt_fbinfo.fb_addr+y*gt_fbinfo.lcd_width+x*gt_fbinfo.pixel_bytes;
	unsigned short *pen_16;	
	unsigned int *pen_32;	
	unsigned int red, green, blue;	
    unsigned int pixel_bits;

	pen_16 = (unsigned short *)pen_8;
	pen_32 = (unsigned int *)pen_8;
    pixel_bits =gt_fbinfo.pixel_bytes * 8; 
	switch (pixel_bits)
	{
		case 8:
		{
			*pen_8 = color;
			break;
		}
		case 16:
		{
			/* 565 */
			red   = (color >> 16) & 0xff;
			green = (color >> 8) & 0xff;
			blue  = (color >> 0) & 0xff;
			color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
			*pen_16 = color;
			break;
		}
		case 32:
		{
			*pen_32 = color;
			break;
		}
		default:
		{
			DebugPrint("can't surport %dbpp\n", gt_fbinfo.pixel_bytes);
			break;
		}
    }

}


static int LcdInit()
{
    int check;
    int fb_fd;
    struct fb_fix_screeninfo fb_fix; 
    struct fb_var_screeninfo fb_var;

    fb_fd = open("/dev/fb0",O_RDWR);
    if(fb_fd < 0){
        DebugPrint("open fb is error!\n");
        return -1;
    }

    check = ioctl(fb_fd , FBIOGET_VSCREENINFO , &fb_var);
    if(check < 0){
        DebugPrint("ioctl fb is error!\n");
        return -1;
    }

    check = ioctl(fb_fd , FBIOGET_FSCREENINFO , &fb_fix);
    if(check < 0){
        DebugPrint("ioctl fb is error!\n");
        return -1;
    }

    gt_fbinfo.lcd_width  = fb_var.xres *fb_var.bits_per_pixel / 8;
    gt_fbinfo.pixel_bytes = fb_var.bits_per_pixel / 8;
    gt_fbinfo.lcd_size   = fb_var.yres * fb_var.xres * fb_var.bits_per_pixel / 8;
    gt_fbinfo.xres   = fb_var.xres;
    gt_fbinfo.yres   = fb_var.yres;

    gt_fbinfo.fb_addr = mmap(NULL ,  gt_fbinfo.lcd_size , 
            PROT_READ | PROT_WRITE , 
            MAP_SHARED ,fb_fd , 0);

    if(NULL == gt_fbinfo.fb_addr){
        DebugPrint("can't map!\n");
        return -1;
    }
    LcdCleanScreen();
    return 0;
}


int FBInit()
{
    return RegisterDisDev(&gt_fbinfo);
}


