#include <types.h>
#include <mydebug.h>
#include <input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PT_InputOpr gp_inputOpr_head=NULL;
fd_set  g_select_queue;
int max_fd = 0;
int RegisterInputOpr (PT_InputOpr p_inputopr) 
{
    PT_InputOpr current;
    PT_InputOpr prev;
    if(gp_inputOpr_head == NULL)
    {
        gp_inputOpr_head = p_inputopr;
        return 0;
    }
    current = gp_inputOpr_head;
    while(current)
    {
       prev = current;
       current = current->next;
    }
    prev->next = p_inputopr;
    return 0;
    
}

int InitInputOpr()
{
    PT_InputOpr current;
    RegisterTouchScreen();
    RegisterStdin();
    current = gp_inputOpr_head;
    while(current)
    {
        current->Init();
        if(max_fd < current->fd)
            max_fd = current->fd;
        max_fd++; 
        current=current->next;
    }
        return 0;
}

PT_InputOpr GetInputOpr(char * name)
{
    PT_InputOpr current;
    current = gp_inputOpr_head;
    while(current)
    {
        if(!strcmp(name ,current->name ))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static int SetSelectMode(PT_InputOpr p_inputdev_head)
{
    struct timeval wait_time;
    PT_InputOpr current = p_inputdev_head;

    wait_time.tv_sec = 3;
    wait_time.tv_usec = 0;

    FD_ZERO(&g_select_queue);
    while(current)
    {
        FD_SET(current->fd,&g_select_queue);
        current=current->next;
    }
    select(max_fd+1,&g_select_queue,NULL,NULL,&wait_time);
	return 0;
}

int GetEvent()
{
    PT_InputOpr current = gp_inputOpr_head;

    SetSelectMode(current);

    while(current)
    {
        if(FD_ISSET(current->fd,&g_select_queue))
        {
            if(current->GetEvent())
            return current->event_type;
        }
            current=current->next;
    }
    return -1;
}
