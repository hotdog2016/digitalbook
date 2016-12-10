#include <types.h>
#include <mydebug.h>
#include<stdio.h>
#include<stdarg.h>

PT_DebugOpr gpt_debugopr_head = NULL;

int DebugOprRegister(PT_DebugOpr ptDebugOpr)
{
    PT_DebugOpr current = gpt_debugopr_head;
    if(current == NULL) 
    {
		ptDebugOpr->next = NULL;
        gpt_debugopr_head = ptDebugOpr; 
        return 0;
    }
    while(current )
    {
        if(NULL == current->next)
        {

			ptDebugOpr->next = NULL;
            current->next = ptDebugOpr;
            return 0;
        }
        current=current->next;
    }
    printf("can't add!\n");
    return -1;
}

int DebugPrint(const char *Format, ...)
{
	PT_DebugOpr current = gpt_debugopr_head;
	va_list tArg;
	char cMsgBuf[1000];
	char *pcBuf = NULL;
	int stringnum = 0;
	va_start(tArg,Format);
	stringnum=vsprintf(cMsgBuf,Format,tArg);
	va_end(tArg); 
	cMsgBuf[stringnum] = '\0';
	pcBuf = cMsgBuf;
	while(pcBuf)
	{
		while(current)
		{
			if(current->iCanUse)
			{
				current->DebugPrint(pcBuf);
			}
			current = current->next;
		}
		return 0;
	}
	return 0;
}

static int ShowDebugOpr()
{
	PT_DebugOpr current = gpt_debugopr_head;
	while(current)
	{
		printf("DebugOpr :%s\n",current->name);
		current=current->next;
	}
	return 0;
}

int DebugInit()
{
	int iError;

//	iError = NetDebugRegister();
//	if(iError < 0)
//	{
//		printf("<%s>:line%d Error : Serial debug register iErroror\n",__FILE__,__LINE__);
//	}

	iError = SerialDebugRegister();
	if(iError < 0)
	{
		printf("<%s>:line%d Error : Serial debug register error\n",__FILE__,__LINE__);
	}
	ShowDebugOpr();
	return 0;
}

