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

static P_DisDevINFO gp_disdevinfo;
static T_FontBitmap gt_fontbitmap;
static P_FontOprType gp_fontopr;
static P_Encodeopr  gp_encodeopr;
static unsigned int g_onefontcode;
static struct stat buf;
static unsigned char * start_addr;
static unsigned char * end_addr; 
unsigned int  page_bytes = 0;
unsigned char * g_current_addr; 

int LoadText()
{
    int text_fd;
    int i;
    text_fd = open("hanzi.txt",O_RDWR);
    if(text_fd <0)
    {
        DebugPrint("Open file is error!");
        return -1;
    }
    stat("hanzi.txt",&buf);
    start_addr = mmap(NULL ,buf.st_size, PROT_READ, MAP_SHARED, text_fd, 0); 
    end_addr = start_addr + buf.st_size;
    for(i=0 ; i<3 ;i++)
    {
       DebugPrint("%02x ",start_addr[i]);
    }
    DebugPrint("\n");
    close(text_fd);
    return 0;
}

static int ChoseEncodeOpr(char * encode_name)
{
    gp_encodeopr =  GetEncodeOpr(encode_name);   
    if(NULL == gp_encodeopr)
    {
        DebugPrint("No encode support!\n");
        return -1;
    }
    return 0;
}
static int ChoseDisDev(char *dev_name)
{
    gp_disdevinfo = GetDevInfo(dev_name);
    if(NULL == gp_disdevinfo)
    {
        DebugPrint("No display decice support!\n");
        return -1;
    }
    gp_disdevinfo->DeviceInit();
    return 0;
}
static int ChoseFontOpr()
{
    gp_fontopr=GetFontOprType("freetype");
    if(NULL == gp_fontopr)
    {
        DebugPrint("No font support!\n");
        return -1;
    }
    gp_fontopr->FontTypeInit();
    return 0;
}

static void DrawOneFont(P_FontBitmap p_fontbitmap)
{
    int i , x , y;
    int bit;
    unsigned char onebyte = 0;

//    DebugPrint("gt_fontbitmap.left   = %d\n",p_fontbitmap->left);
//    DebugPrint("gt_fontbitmap.top    = %d\n",p_fontbitmap->top);
//    DebugPrint("gt_fontbitmap.pitch  = %d\n",p_fontbitmap->pitch);
//    DebugPrint("gt_fontbitmap.xMax    = %d\n",p_fontbitmap->xMax);
//    DebugPrint("xres                  = %d\n",gp_disdevinfo->xres);
//    DebugPrint("p_fontbitmap = %p\n",p_fontbitmap);

    for(y = p_fontbitmap->top ; y < p_fontbitmap->yMax ; y++)
    {
        i = (y - p_fontbitmap->top) * p_fontbitmap->pitch;
        bit=7;
        for(x = p_fontbitmap->left ; x < p_fontbitmap->xMax ; x++)
        {
            if(bit == 7)
                onebyte = p_fontbitmap->p_bitmap_buffer[i++];
            if(onebyte & (1<<bit))
                gp_disdevinfo->ShowPixel(x,y,0x0000ff);
            else
                gp_disdevinfo->ShowPixel(x,y,0);
            bit--;
            if(-1 == bit)
                bit = 7;
        }
    }
} 

static int NextLine()
{
    gt_fontbitmap.cur_pos_x = 0;
    gt_fontbitmap.cur_pos_y += 24;
    return 0;
}

static int RefurbishScreen()
{
    gt_fontbitmap.cur_pos_x = 0;
    gt_fontbitmap.cur_pos_y = 24;
    gp_disdevinfo->CleanScreen();
    return 0;
}
int ShowPage(unsigned char **current_addr)
{
    int bytes;
    gt_fontbitmap.cur_pos_x = 0;
    gt_fontbitmap.cur_pos_y = 24;
    while(1)
    {
        bytes=gp_encodeopr->GetEncode(&g_onefontcode,*current_addr);
        gp_fontopr->GetFontBitmap(g_onefontcode,&gt_fontbitmap);

        if('\n' == g_onefontcode || '\0' == g_onefontcode)
        {
            char c = g_onefontcode;
            switch(c)
            {
                case '\n' :NextLine();
                            if(gt_fontbitmap.cur_pos_y > gp_disdevinfo->yres){
                                page_bytes += bytes;
                                *current_addr += bytes; 
                                return 0;
                             }
                           break;
                case '\0' :gt_fontbitmap.cur_pos_x += 24;
                           if(gt_fontbitmap.left > (gp_disdevinfo->xres)){
                               NextLine();
                               if(gt_fontbitmap.cur_pos_y > gp_disdevinfo->yres){
                                    page_bytes += bytes;
                                    *current_addr += bytes;
                                    return 0;
                                 }
                           }
                           break;
            }
            page_bytes += bytes;
            *current_addr += bytes; 
                continue;
        }
            DrawOneFont(&gt_fontbitmap);
        gt_fontbitmap.cur_pos_x = gt_fontbitmap.next_pos_x;
        if(gt_fontbitmap.left > (gp_disdevinfo->xres - 50)){
            NextLine();
        }
        if(gt_fontbitmap.cur_pos_y > gp_disdevinfo->yres)
        {  
            page_bytes += bytes;
            *current_addr += bytes; 
            return 0;
        }
        page_bytes += bytes;
        *current_addr += bytes; 
    }
}

int ShowText()
{
    g_current_addr = start_addr;
    ShowPage(&g_current_addr);
    while(1)
    {
        if(!PageOpr(&g_current_addr,&page_bytes))
        {
            RefurbishScreen();
            ShowPage(&g_current_addr);
        }
    }
}

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
