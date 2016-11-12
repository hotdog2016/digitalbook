#include <draw.h>
#include <stdio.h>
#include <mydebug.h>
#include <stdlib.h>
#include <input.h>
#include <sys/select.h>
#include <unistd.h>

static P_PageInfo gp_page_info_head=NULL;

static P_PageInfo CreatOnePage(unsigned char * current_addr,unsigned int page_bytes)
{

    P_PageInfo p_page_tmp;
    p_page_tmp=(P_PageInfo)malloc(sizeof(T_PageInfo));
    if(NULL == p_page_tmp)
    {
        return NULL;
    }

    p_page_tmp->prev = NULL;
    p_page_tmp->next = NULL;
    p_page_tmp->page_end_addr = current_addr;
    p_page_tmp->page_bytes   = page_bytes;
    p_page_tmp->page_start_addr = current_addr - page_bytes;
    return p_page_tmp;
}

static int AddPagetoList(P_PageInfo p_page_tmp)
{
    P_PageInfo current;
    P_PageInfo prev;

    if(NULL == gp_page_info_head)
    {
       gp_page_info_head = p_page_tmp; 
       gp_page_info_head->next = NULL;
       gp_page_info_head->prev = p_page_tmp;
       return 0;
    }else{
            current = gp_page_info_head;
            while(current)
            {
                prev=current;
                current = current->next;
            }
            prev->next = p_page_tmp;
            p_page_tmp->prev = prev;
            return 0;
    }
    return -1;
}

static P_PageInfo CatchPage(unsigned char * current_addr)
{
    P_PageInfo current=NULL;
    current = gp_page_info_head;
    while(current)
    {
        if(current_addr == current->page_end_addr)
            return current;
        current=current->next;
    }
    return NULL;
}

static int NextPage(P_PageInfo p_page_tmp,unsigned char ** current_addr)
{
    if(NULL == p_page_tmp->next)
        return 0;
    *current_addr = p_page_tmp->page_end_addr;
    return 0;
}

static int PrevPage(P_PageInfo p_page_tmp,unsigned char ** current_addr)
{
    *current_addr = p_page_tmp->prev->page_start_addr;
    return 0;
}

static int Choice(P_PageInfo p_page_tmp,unsigned char ** current_addr)
{
    char event = 0;
    event = GetEvent();
    DebugPrint("PageOpr : event = %d\n",event);
    switch(event)
    {
        case NEXTPAGE : 
            return (NextPage(p_page_tmp , current_addr));
        case PREVPAGE : 
            return (PrevPage(p_page_tmp , current_addr));
        case QUIT     :
            exit(0);
    }
    return 1;
}

int PageOpr(unsigned char ** current_addr,unsigned int * page_bytes )
{
    int check;
    P_PageInfo p_page_tmp;
    p_page_tmp = CatchPage(*current_addr);

    if(NULL ==  p_page_tmp)
    {
        p_page_tmp = CreatOnePage(*current_addr,*page_bytes);
        if(NULL==p_page_tmp)
        {
            DebugPrint("Creat page error!\n");
            return -1;
        }

        check=AddPagetoList(p_page_tmp);
        if(check < 0)
        {
            DebugPrint("Add page error\n");
            return -1;
        }
    }

    *page_bytes = 0;
    check = Choice(p_page_tmp,current_addr);
    return check ;
}

