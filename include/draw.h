#ifndef _DRAW_H
#define _DRAW_H
typedef struct PageInfo{
    struct PageInfo *prev;
    struct PageInfo *next;
    int page_bytes;
    unsigned char * page_start_addr;
    unsigned char * page_end_addr;
    unsigned char * page_addr;
}T_PageInfo ,*P_PageInfo;
int PageOpr(unsigned char ** current_addr,unsigned int * page_bytes );

int    SetDisDev();
int    SetFont();
int    SetEncode();

int ShowPage(unsigned char **current_addr);
int LoadText();
int ShowText();
#endif
