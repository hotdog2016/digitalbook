#include <types.h>
#include <disdev.h>
#include <mydebug.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <jpeglib.h>
#include <stdlib.h>
#include <setjmp.h>

static PT_DisDevINFO gptDisDevInfo;
FILE * fPic;

static int LoadPic(char * PicName)
{
	if ((fPic = fopen(PicName, "rb")) == NULL) {
	    fprintf(stderr, "can't open %s\n", PicName);
	    exit(1);
	}

    return 0;
}

static int ChoseDisDev(char *pcDevName)
{
    gptDisDevInfo = GetDevInfo(pcDevName);
    if(NULL == gptDisDevInfo)
    {
        DebugPrint("No display decice support!\n");
        return -1;
    }
    gptDisDevInfo->DeviceInit();
    return 0;
}

int GetDisDev()
{
    ChoseDisDev("fb");
    return 0;
}

static int ShowLine(UINT32 dwYres,BYTE * bBuf)
{
	UINT32 i;
	UINT32 dwXres;
	UINT32 dwColor;
	for(dwXres = 0 ; dwXres < gptDisDevInfo->xres ;dwXres++ ,bBuf += 3)
	{
		dwColor = 0;
		for(i = 0; i< 3 ; i++)
		{
			dwColor |= bBuf[i] << ((2-i)*8);
		}
		dwColor &= 0xffffff;
		gptDisDevInfo->ShowPixel(dwXres,dwYres,dwColor);
	}
	return 0;
}

int ShowPicDetail()
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	BYTE * bBuf;	
	UINT32 dwTride;

	GetDisDev();

	LoadPic("1.jpg");

	gptDisDevInfo->CleanScreen();
	
	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);

	jpeg_stdio_src(&cinfo, fPic);

	jpeg_read_header(&cinfo, TRUE);

	cinfo.scale_num = 1;
	cinfo.scale_denom = 1;


	jpeg_start_decompress(&cinfo);

	dwTride = cinfo.output_width * cinfo.output_components;

#if 0
	printf("width = %d\n",cinfo.output_width);
	printf("height = %d\n",cinfo.output_height);
	printf("color components = %d\n",cinfo.out_color_components);	
	printf("output_components = %d\n",cinfo.output_components);
	
	DebugPrint("cinfo.scale_num = %d\n",cinfo.scale_num);
	DebugPrint("cinfo.scale_num = %d\n",cinfo.scale_denom);
#endif

	bBuf = malloc(dwTride);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		jpeg_read_scanlines(&cinfo,&bBuf,1);
		if(cinfo.output_scanline < gptDisDevInfo->yres)
		ShowLine(cinfo.output_scanline,bBuf);
	}

	free(bBuf);

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	return 0;
}
