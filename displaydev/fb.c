#include <types.h>
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

static void LcdPutPixel(int iXres, int iYres, UINT32 uiColor);
static int LcdInit();
static int LcdCleanScreen();

static T_DisDevINFO gtFBInfo ={
    .name         = "fb",
    .ShowPixel    = LcdPutPixel,
    .DeviceInit   = LcdInit,
    .CleanScreen  = LcdCleanScreen,
};

static int LcdCleanScreen()
{
    memset(gtFBInfo.fb_addr , 0 , gtFBInfo.lcd_size);
    return 0;
}
static void LcdPutPixel(int iXres, int iYres, UINT32 uiColor)
{
	BYTE *pcPixel8 = gtFBInfo.fb_addr+iYres*gtFBInfo.lcd_width+iXres*gtFBInfo.pixel_bytes;
	UINT16 *pusPixel16;	
	UINT32 *puiPixel32;	
	UINT32 iRed, iGreen, iBlue;	
    int iPixelBits;

	pusPixel16 = (UINT16 *)pcPixel8;
	puiPixel32 = (UINT32 *)pcPixel8;
    iPixelBits = gtFBInfo.pixel_bytes * 8; 
	switch (iPixelBits)
	{
		case 8:
		{
			*pcPixel8 = uiColor;
			break;
		}
		case 16:
		{
			/* 565 */
			iRed   = (uiColor >> 16) & 0xff;
			iGreen = (uiColor >> 8) & 0xff;
			iBlue  = (uiColor >> 0) & 0xff;
			uiColor = ((iRed >> 3) << 11) | ((iGreen >> 2) << 5) | (iBlue >> 3);
			*pusPixel16 = uiColor;
			break;
		}
		case 32:
		{
			*puiPixel32 = uiColor;
			break;
		}
		default:
		{
			DebugPrint("can't surport %dbpp\n", gtFBInfo.pixel_bytes);
			break;
		}
    }

}


static int LcdInit()
{
    int iError;
    int iFBFd;
    struct fb_fix_screeninfo tScreenFixInfo; 
    struct fb_var_screeninfo tScreenVarInfo;

    iFBFd = open("/dev/fb0",O_RDWR);
    if(iFBFd < 0){
        DebugPrint("open fb is error!\n");
        return -1;
    }

    iError = ioctl(iFBFd , FBIOGET_VSCREENINFO , &tScreenVarInfo);
    if(iError < 0){
        DebugPrint("ioctl fb is error!\n");
        return -1;
    }

    iError = ioctl(iFBFd , FBIOGET_FSCREENINFO , &tScreenFixInfo);
    if(iError < 0){
        DebugPrint("ioctl fb is error!\n");
        return -1;
    }

    gtFBInfo.lcd_width  = tScreenVarInfo.xres *tScreenVarInfo.bits_per_pixel / 8;
    gtFBInfo.pixel_bytes = tScreenVarInfo.bits_per_pixel / 8;
    gtFBInfo.lcd_size   = tScreenVarInfo.yres * tScreenVarInfo.xres * tScreenVarInfo.bits_per_pixel / 8;
    gtFBInfo.xres   = tScreenVarInfo.xres;
    gtFBInfo.yres   = tScreenVarInfo.yres;

    gtFBInfo.fb_addr = mmap(NULL ,  gtFBInfo.lcd_size , 
            PROT_READ | PROT_WRITE , 
            MAP_SHARED ,iFBFd , 0);

    if(NULL == gtFBInfo.fb_addr){
        DebugPrint("can't map!\n");
        return -1;
    }
    LcdCleanScreen();
    return 0;
}

int FBInit()
{
	LcdInit();
    return RegisterDisDev(&gtFBInfo);
}


