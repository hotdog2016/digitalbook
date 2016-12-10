#include <types.h>
#include <draw.h>
#include <mydebug.h>
#include <encode.h>
#include <font.h>
#include <disdev.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

static PT_DisDevINFO gptDisDevInfo;
static T_FontBitmap gtFontBitMap;
static PT_FontOprType gptFontOpr;
static PT_Encodeopr  gptEncodeOpr;
static UINT32 guiFontCode;
static struct stat tFStat;
static char * gpcTextStartAddr;
static char * gpcTextEndAddr; 
static char * gpcCurrentAddr; 
static UINT32  guiPageSize = 0;

static int LoadText(char * TextName)
{
    int iTextFd;
    int i;
    iTextFd = open(TextName,O_RDWR);
    if(iTextFd <0)
    {
        DebugPrint("Open file is error!");
        return -1;
    }
    stat(TextName,&tFStat);
    gpcTextStartAddr = mmap(NULL ,tFStat.st_size, PROT_READ, MAP_SHARED, iTextFd, 0); 
    gpcTextEndAddr = gpcTextStartAddr + tFStat.st_size;
    for(i=0 ; i<3 ;i++)
    {
       DebugPrint("%02x ",gpcTextStartAddr[i]);
    }
    DebugPrint("\n");
    close(iTextFd);
    return 0;
}


static void DrawOneFont(PT_FontBitmap ptFontBitMap)
{
    int i; 
	int iXres , iYres;
    int iBits;
    BYTE onebyte = 0;

//    DebugPrint("gtFontBitMap.left   = %d\n",ptFontBitMap->left);
//    DebugPrint("gtFontBitMap.top    = %d\n",ptFontBitMap->top);
//    DebugPrint("gtFontBitMap.pitch  = %d\n",ptFontBitMap->pitch);
//    DebugPrint("gtFontBitMap.xMaiXres    = %d\n",ptFontBitMap->xMaiXres);
//    DebugPrint("xres                  = %d\n",gptDisDevInfo->xres);
//    DebugPrint("ptFontBitMap = %p\n",ptFontBitMap);

    for(iYres = ptFontBitMap->top ; iYres < ptFontBitMap->yMax ; iYres++)
    {
        i = (iYres - ptFontBitMap->top) * ptFontBitMap->pitch;
        iBits=7;
        for(iXres = ptFontBitMap->left ; iXres < ptFontBitMap->xMax ; iXres++)
        {
            if(iBits == 7)
                onebyte = ptFontBitMap->p_bitmap_buffer[i++];
            if(onebyte & (1<<iBits))
                gptDisDevInfo->ShowPixel(iXres,iYres,0x0000ff);
            else gptDisDevInfo->ShowPixel(iXres,iYres,0);
            iBits--;
            if(-1 == iBits)
                iBits = 7;
        }
    }
} 

static int NextLine()
{
    gtFontBitMap.cur_pos_x = 0;
    gtFontBitMap.cur_pos_y += 24;
    return 0;
}

static int RefurbishScreen()
{
    gtFontBitMap.cur_pos_x = 0;
    gtFontBitMap.cur_pos_y = 24;
    gptDisDevInfo->CleanScreen();
    return 0;
}
int ShowPage(char **pcCurrentAddr)
{
    int iBytes;

    gtFontBitMap.cur_pos_x = 0;
    gtFontBitMap.cur_pos_y = 24;

    while(1)
    {
        iBytes=gptEncodeOpr->GetEncode(&guiFontCode,*pcCurrentAddr);
        gptFontOpr->GetFontBitmap(guiFontCode,&gtFontBitMap);

        if('\n' == guiFontCode || '\0' == guiFontCode)
        {
            char c = guiFontCode;
            switch(c)
            {
                case '\n' :NextLine();
                            if(gtFontBitMap.cur_pos_y > gptDisDevInfo->yres){
                                guiPageSize += iBytes;
                                *pcCurrentAddr += iBytes; 
                                return 0;
                             }
                           break;
                case '\0' :gtFontBitMap.cur_pos_x += 24;
                           if(gtFontBitMap.left > (gptDisDevInfo->xres)){
                               NextLine();
                               if(gtFontBitMap.cur_pos_y > gptDisDevInfo->yres){
                                    guiPageSize += iBytes;
                                    *pcCurrentAddr += iBytes;
                                    return 0;
                                 }
                           }
                           break;
            }
            guiPageSize += iBytes;
            *pcCurrentAddr += iBytes; 
                continue;
        }
            DrawOneFont(&gtFontBitMap);
        gtFontBitMap.cur_pos_x = gtFontBitMap.next_pos_x;
        if(gtFontBitMap.left > (gptDisDevInfo->xres - 50)){
            NextLine();
        }
        if(gtFontBitMap.cur_pos_y > gptDisDevInfo->yres)
        {  
            guiPageSize += iBytes;
            *pcCurrentAddr += iBytes; 
            return 0;
        }
        guiPageSize += iBytes;
        *pcCurrentAddr += iBytes; 
    }
}

int ShowText(char * TextName)
{

	LoadText(TextName);
    gpcCurrentAddr = gpcTextStartAddr;

    gptDisDevInfo = GetDevInfo("fb");
    if(NULL == gptDisDevInfo)
    {
        DebugPrint("No display decice support!\n");
        return -1;
    }


    gptEncodeOpr =  GetEncodeOpr("utf-8");   
    if(NULL == gptEncodeOpr)
    {
        DebugPrint("No encode support!\n");
        return -1;
    }

    gptFontOpr=GetFontOprType("freetype");
    if(NULL == gptFontOpr)
    {
        DebugPrint("No font support!\n");
        return -1;
    }

    ShowPage(&gpcCurrentAddr);

    while(1)
    {
        if(!PageOpr(&gpcCurrentAddr,&guiPageSize))
        {
            RefurbishScreen();
            ShowPage(&gpcCurrentAddr);
        }
    }
}

#if 0
int SetDisDev()
{
    DisplayInit();
    ChoseDisDev("fb");
    return 0;
}

int  SetFont()
{
    FontInit();
    ChoseFontOpr();
    return 0;
}

int SetEncode()
{
    EncodeInit();
    ChoseEncodeOpr("utf-8");
    return 0;
}
#endif
