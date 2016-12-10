#ifndef __BITMAPINFO__H
#define __BITMAPINFO__H

#include <types.h>
#pragma pack (push)
#pragma pack(1)

typedef struct BITMAPFILEHEADER {
	WORD bfType; 
	DWORD bfSize;
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits;
}T_BMPFileHead,*PT_BMPFileHead;

#pragma pack (pop)

#pragma pack (push)
#pragma pack(1)

typedef struct BITMAPINFOHEADER {
	DWORD biSize;
	int biWidth;
	int biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	int biXPelsPerMeter;
	int biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}T_BMPInfoHead,*PT_BMPInfoHead;

#pragma pack (pop)

typedef struct PICTURE{
	UINT16 wX;
	UINT16 wY;
	UINT32 dwWidth;
	UINT32 dwHeight; 
	UINT32 *dwpPicBuf;
}T_Picture,*PT_Picture;

int GetBMPFileInfo();

#endif

