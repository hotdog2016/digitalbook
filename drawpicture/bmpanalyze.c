#include <bitmapinfo.h>
#include <disdev.h>
#include <types.h>
#include <disdev.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define  GET_PIXEL(x,y,PicBuf)           ((PicBuf)[(x)+(y)*(128)])

static T_BMPFileHead g_tBMPFileHead;
static T_BMPInfoHead g_tBMPInfoHead;
static PT_DisDevINFO gptDisDevInfo;
static T_Picture     g_tSrcPic;
static T_Picture     g_tDstPic;

static int ChoseDisDev(char *pcDevName)
{
    gptDisDevInfo = GetDevInfo(pcDevName);
    if(NULL == gptDisDevInfo)
    {
        printf("No display decice support!\n");
        return -1;
    }
    gptDisDevInfo->DeviceInit();
    return 0;
}


int show(unsigned long *dwpPicBuf)
{
	UINT32 dwXres;
	UINT32 dwYres;
	UINT32 dwColor;
	for(dwYres =0 ; dwYres < g_tBMPInfoHead.biHeight;dwYres++)
	{
		for(dwXres = 0;dwXres < g_tBMPInfoHead.biWidth;dwXres++)
		{
			dwColor = GET_PIXEL(dwXres,dwYres,dwpPicBuf);
			gptDisDevInfo->ShowPixel(dwXres,dwYres,dwColor);
		}
	}
	return 0;

}

static int BMPGetPic(unsigned long *dwpPicBuf,char * pbBMPBuf)
{
	UINT32 dwXres;
	UINT32 dwYres;
	UINT32 dwColor;
	char * pbPixelAddr=NULL;
	int i;

	for(dwYres = g_tBMPInfoHead.biHeight  ;dwYres > 0 ;dwYres--)
	{
		for(dwXres = 0 ; dwXres < g_tBMPInfoHead.biWidth;dwXres++,dwpPicBuf++)
		{
			pbPixelAddr = pbBMPBuf + dwXres*g_tBMPInfoHead.biBitCount/8 + dwYres*g_tBMPInfoHead.biWidth*3; 
			dwColor = 0;
			for(i = 0; i< 3 ; i++)
			{
				dwColor |= pbPixelAddr[i] << (i*8);//bitmap 图片读出来是B G R,需要反转一下;
			}
			dwColor &= 0xffffff;
			*dwpPicBuf = dwColor;
		}
	}
	return 0;
}


//int zoom(PT_Picture Dst,PT_Picture Src)
//{
//	int x,y;
//	long getx ,gety;
//	for(y=0;y < Dst->dwHeight;y++)
//	{
//		for(x=0;x < Dst->dwWidth;x++)
//		{
//			getx=(x*Src->dwWidth/Dst->dwWidth);
//			gety=(y*Src->dwHeight/Dst->dwHeight);
//			Dst[y][x] = Src[gety][getx];
//		}
//	}
//}




int GetBMPFileInfo()
{
	FILE * fpBitmap;
	int iRet;
	char * pbBMPBuf;
	unsigned long * dwpPicBuf;
	int iWidthBytes;
	int iPicBytes;

	ChoseDisDev("fb");
	fpBitmap = fopen("ok.bmp","rb");
	if(NULL == fpBitmap)
	{
		printf("open BMP file faile!\n");
		return -1;
	}

	iRet = fread(&g_tBMPFileHead ,sizeof(g_tBMPFileHead),1,fpBitmap);
	if(iRet ==0 )
	{
		perror("g_tBMPFileHead get data");
		return -1;
	}


	printf("BMPInfosize = %d\n",sizeof(g_tBMPInfoHead));
	iRet = fread(&g_tBMPInfoHead,sizeof(g_tBMPInfoHead),1,fpBitmap);
	if(iRet ==0)
	{
		perror("BMPInfo get data");
		return -1;
	}

	iWidthBytes = g_tBMPInfoHead.biWidth * g_tBMPInfoHead.biBitCount/8;
	iPicBytes = iWidthBytes * g_tBMPInfoHead.biHeight;	

	pbBMPBuf = malloc(sizeof(char)*iPicBytes);
	if(NULL == pbBMPBuf)
	{
		printf("bPicBuf malloc error\n");
		return -1;
	}
	dwpPicBuf = malloc(sizeof(unsigned long) * g_tBMPInfoHead.biWidth * g_tBMPInfoHead.biHeight);
	if(dwpPicBuf == NULL)
	{
		printf("dwpPicBuf malloc error\n");
		return -1;
	}

	fread(pbBMPBuf , iPicBytes , 1 ,fpBitmap );

	BMPGetPic(dwpPicBuf,pbBMPBuf);
	show(dwpPicBuf);
	
	free(pbBMPBuf);
	free(dwpPicBuf);
	fclose(fpBitmap);

	return 0;
}


