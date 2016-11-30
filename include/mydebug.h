#ifndef _DEBUG_H
#define _DEBUG_H

typedef struct DebugOpr{
	char *name;
	int iCanUse;
	int (*DebugPrint)(char *string);
	struct DebugOpr *next;
}*PT_DebugOpr,T_DebugOpr;

int DebugOprRegister(PT_DebugOpr pt_debugopr);
int DebugPrint(const char *format, ...);
int DebugInit();
int SerialDebugRegister();
int NetDebugRegister();
#endif
