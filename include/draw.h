#ifndef _DRAW_H
#define _DRAW_H

typedef struct PageInfo{
    int page_bytes;
    char * page_start_addr;
    char * page_end_addr;
    char * page_addr;
    struct PageInfo *prev;
    struct PageInfo *next;
}T_PageInfo ,*PT_PageInfo;

int PageOpr(char ** pcCurrentAddr,unsigned int * puiPageSize );
//int LoadText(char * name);
int ShowPage(char **pcCurrentAddr);
int ShowText(char * TextName);
#endif
