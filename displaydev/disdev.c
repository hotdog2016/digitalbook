#include <types.h>
#include <disdev.h>
#include<string.h>
#include<stdio.h>

static PT_DisDevINFO p_disdevinfo_head = NULL;

int RegisterDisDev(PT_DisDevINFO ptDisDevInfo)
{
    PT_DisDevINFO current = NULL;
    PT_DisDevINFO prev = NULL;
    if(p_disdevinfo_head == NULL)
    {
        p_disdevinfo_head = ptDisDevInfo;
        p_disdevinfo_head->next = NULL;
        return 0;
    }
    current = p_disdevinfo_head;
    while(current)
    {
       prev = current;
       current=current->next;
    }
    prev->next = ptDisDevInfo;
    prev->next->next = NULL;
    return 0;
}

PT_DisDevINFO GetDevInfo(char *dev_name)
{
    PT_DisDevINFO current;
    int check;
    current = p_disdevinfo_head;
    if(NULL == current)
        return NULL;
    while(current)
    {
        check = strcmp(dev_name , current->name);
        if(0 == check)
            return current;
        current=current->next;
    }
    return NULL;
}

int DisplayInit()
{
    FBInit();
    return 0;
}
