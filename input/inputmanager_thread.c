#include <types.h>
#include <mydebug.h>
#include <input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static pthread_mutex_t gt_event_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  gt_event_cond  = PTHREAD_COND_INITIALIZER;
int event;

PT_InputOpr gp_inputOpr_head=NULL;
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

void *ThreadGetEvent(void * p_GetEventFuncAddr)
{
    int event_tmp;;
    int (*GetEvent)(void);
    GetEvent = (int (*)(void))p_GetEventFuncAddr;
    while(1)
    {
         event_tmp = GetEvent();
         if(event_tmp)
         {
             pthread_mutex_lock(&gt_event_mutex);
             event = event_tmp;
             pthread_cond_signal(&gt_event_cond);
             pthread_mutex_unlock(&gt_event_mutex);
         }
    }

}

int InitInputOpr()
{
    PT_InputOpr current;
    RegisterStdin();  
    RegisterTouchScreen();

    current = gp_inputOpr_head;

    while(current)
    {
        current->Init();
        pthread_create(&current->tid,NULL,ThreadGetEvent,(void *)current->GetEvent);
        DebugPrint("input name is : %s\n",current->name);
        current=current->next;
    }
        return 0;
}

int GetEvent()
{
    pthread_mutex_lock(&gt_event_mutex);
    pthread_cond_wait(&gt_event_cond,&gt_event_mutex);
    pthread_mutex_unlock(&gt_event_mutex);
    return event;
}
