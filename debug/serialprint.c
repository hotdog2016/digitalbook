#include<mydebug.h>
#include<stdio.h>

static T_DebugOpr gt_serial_debugopr;

static int SerialDebugPrint(char *debug_msg)
{
	if(debug_msg)	
	{
		printf("%s",debug_msg);
		return 0;
	}
	else{
		printf("<%s>%d Error:Serial print error\n",__FILE__,__LINE__);
		return -1;
	}
}

static T_DebugOpr gt_serial_debugopr = {
	.name = "SerialDebug",
	.iCanUse = 1,
	.DebugPrint = SerialDebugPrint,
};

int SerialDebugRegister()
{
	return DebugOprRegister(&gt_serial_debugopr);	
}
