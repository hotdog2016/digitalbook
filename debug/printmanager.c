#include <mydebug.h>
#include<stdio.h>
#include<stdarg.h>

PT_DebugOpr gpt_debugopr_head = NULL;

int DebugOprRegister(PT_DebugOpr pt_debugopr)
{
    PT_DebugOpr pt_current_debugopr = gpt_debugopr_head;
    if(pt_current_debugopr == NULL) 
    {
        gpt_debugopr_head = pt_debugopr; 
		gpt_debugopr_head->next = NULL;
        return 0;
    }
    while(pt_current_debugopr )
    {
        if(NULL == pt_current_debugopr->next)
        {
            pt_current_debugopr= pt_debugopr;
            pt_current_debugopr->next=NULL;
            return 0;
        }
        pt_current_debugopr=pt_current_debugopr->next;
    }
    printf("can't add!\n");
    return -1;
}

int DebugPrint(const char *Format, ...)
{
	PT_DebugOpr pt_current_debugopr = gpt_debugopr_head;
	va_list targ;
	char stringbuffer[1000];
	char *p_buffertmp = NULL;
	int stringnum = 0;
	va_start(targ,Format);
	stringnum=vsprintf(stringbuffer,Format,targ);
	va_end(targ); 
	stringbuffer[stringnum] = '\0';
	p_buffertmp = stringbuffer;
	while(p_buffertmp)
	{
		while(pt_current_debugopr)
		{
			if(pt_current_debugopr->iCanUse)
			{
				pt_current_debugopr->DebugPrint(p_buffertmp);
			}
			pt_current_debugopr = pt_current_debugopr->next;
		}
		return 0;
	}
	return 0;
}

static int ShowDebugOpr()
{
	PT_DebugOpr pt_current_debugopr = gpt_debugopr_head;
	while(pt_current_debugopr)
	{
		printf("DebugOpr :%s\n",pt_current_debugopr->name);
		pt_current_debugopr=pt_current_debugopr->next;
	}
	return 0;
}

int DebugInit()
{
	int err;
	err = SerialDebugRegister();
	if(err < 0)
	{
		printf("<%s>:line%d Error : Serial debug register error\n",__FILE__,__LINE__);
	}
	ShowDebugOpr();
	return 0;
}

