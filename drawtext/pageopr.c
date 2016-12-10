#include <types.h>
#include <draw.h>
#include <stdio.h>
#include <mydebug.h>
#include <stdlib.h>
#include <input.h>
#include <sys/select.h>
#include <unistd.h>

static PT_PageInfo gp_page_info_head=NULL;

static PT_PageInfo CreatOnePage(char * pcCurrentAddr,UINT32 uiPageSize)
{

    PT_PageInfo ptPageInfo;
    ptPageInfo=(PT_PageInfo)malloc(sizeof(T_PageInfo));
    if(NULL == ptPageInfo)
    {
        return NULL;
    }

    ptPageInfo->prev = NULL;
    ptPageInfo->next = NULL;
    ptPageInfo->page_end_addr = pcCurrentAddr;
    ptPageInfo->page_bytes  = uiPageSize;
    ptPageInfo->page_start_addr = pcCurrentAddr - uiPageSize;
    return ptPageInfo;
}

static int AddPagetoList(PT_PageInfo ptPageInfo)
{
    PT_PageInfo current;
    PT_PageInfo prev;

    if(NULL == gp_page_info_head)
    {
       gp_page_info_head = ptPageInfo; 
       gp_page_info_head->next = NULL;
       gp_page_info_head->prev = ptPageInfo;
       return 0;
    }else{
            current = gp_page_info_head;
            while(current)
            {
                prev=current;
                current = current->next;
            }
            prev->next = ptPageInfo;
            ptPageInfo->prev = prev;
            return 0;
    }
    return -1;
}

static PT_PageInfo CatchPage(char * pcCurrentAddr)
{
    PT_PageInfo current=NULL;
    current = gp_page_info_head;
    while(current)
    {
        if(pcCurrentAddr == current->page_end_addr)
            return current;
        current=current->next;
    }
    return NULL;
}

static int NextPage(PT_PageInfo ptPageInfo,char ** pcCurrentAddr)
{
    if(NULL == ptPageInfo->next)
        return 0;
    *pcCurrentAddr = ptPageInfo->page_end_addr;
    return 0;
}

static int PrevPage(PT_PageInfo ptPageInfo,char ** pcCurrentAddr)
{
    *pcCurrentAddr = ptPageInfo->prev->page_start_addr;
    return 0;
}

static int Choice(PT_PageInfo ptPageInfo,char ** pcCurrentAddr)
{
    char event = 0;
    event = GetEvent();
    DebugPrint("PageOpr : event = %d\n",event);
    switch(event)
    {
        case NEXTPAGE : 
            return (NextPage(ptPageInfo , pcCurrentAddr));
        case PREVPAGE : 
            return (PrevPage(ptPageInfo , pcCurrentAddr));
        case QUIT     :
            exit(0);
    }
    return 1;
}

int PageOpr(char ** pcCurrentAddr,UINT32 * puiPageSize )
{
    int check;
    PT_PageInfo ptPageInfo;
    ptPageInfo = CatchPage(*pcCurrentAddr);

    if(NULL ==  ptPageInfo)
    {
        ptPageInfo = CreatOnePage(*pcCurrentAddr,*puiPageSize);
        if(NULL==ptPageInfo)
        {
            DebugPrint("Creat page error!\n");
            return -1;
        }

        check=AddPagetoList(ptPageInfo);
        if(check < 0)
        {
            DebugPrint("Add page error\n");
            return -1;
        }
    }

    *puiPageSize = 0;
    check = Choice(ptPageInfo,pcCurrentAddr);
    return check ;
}

