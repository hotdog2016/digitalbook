#include <disdev.h>
#include<string.h>
#include<stdio.h>

static P_DisDevINFO p_disdevinfo_head = NULL;

int RegisterDisDev(P_DisDevINFO p_disdevinfo)
{
    P_DisDevINFO current = NULL;
    P_DisDevINFO prev = NULL;
    if(p_disdevinfo_head == NULL)
    {
        p_disdevinfo_head = p_disdevinfo;
        p_disdevinfo_head->next = NULL;
        return 0;
    }
    current = p_disdevinfo_head;
    while(current)
    {
       prev = current;
       current=current->next;
    }
    prev->next = p_disdevinfo;
    prev->next->next = NULL;
    return 0;
}
P_DisDevINFO GetDevInfo(char *dev_name)
{
    P_DisDevINFO current;
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
