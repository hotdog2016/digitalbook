#include <types.h>
#include<mydebug.h>
#include<stdio.h>

static T_DebugOpr gtSerialDebugOpr;

static int SerialDebugPrint(char *pcDebugMsg)
{
	if(pcDebugMsg)	
	{
		printf("%s",pcDebugMsg);
		return 0;
	}
	else{
		printf("<%s>%d Error:Serial print error\n",__FILE__,__LINE__);
		return -1;
	}
}

static T_DebugOpr gtSerialDebugOpr = {
	.name = "SerialDebug",
	.iCanUse = 1,
	.DebugPrint = SerialDebugPrint,
};

int SerialDebugRegister()
{
	return DebugOprRegister(&gtSerialDebugOpr);	
}
