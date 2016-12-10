#include <types.h>
#include<mydebug.h>
#include<network.h>
#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define                           CYCLIC_BUFFER_SIZE               1024
#define                           CYCLIC_BUFFER_SIZE               1024

static T_DebugOpr gtNetDebugOpr;
static int giWriteBytes=0 , giReadBytes=0;
static char  gcCyclicBuffer[CYCLIC_BUFFER_SIZE];
//int server_fd;
static int writeBuffer(char* pcBuf , char const * pcSource , int iBufSizes)
{
	int i;
	for(i=0 ; i<iBufSizes ; i++)
	{
		if((giWriteBytes+1) % CYCLIC_BUFFER_SIZE == giReadBytes)
		{
			printf("Cyclic buffer is full\n");
			return -1;
		}
			strncpy(&pcBuf[giWriteBytes] , &pcSource[i] ,1);	
			giWriteBytes = (giWriteBytes + 1) % CYCLIC_BUFFER_SIZE;
	}
	return 0;
}

static int readBuffer(char const * pcBuf , int iBufSizes)
{
	int i;
	int iServerFd;
	GetSocketfd(&iServerFd,"TCP");
	for(i=0 ; i<iBufSizes ; i++)
	{
		if(giReadBytes == giWriteBytes)
		{
			printf("Cyclic is empty\n");
			return -1;
		}
		send(iServerFd, &pcBuf[giReadBytes], 1, 0);
		giReadBytes = (giReadBytes + 1) % CYCLIC_BUFFER_SIZE;
	}
	return 0;
}

static int NetDebugPrint(char *pcDebugMsg)
{
	int iMsgSizes = 0; 

	iMsgSizes = strlen(pcDebugMsg);

	if(pcDebugMsg)	
	{
		writeBuffer(gcCyclicBuffer , pcDebugMsg , iMsgSizes);
		readBuffer(gcCyclicBuffer ,iMsgSizes);
		return 0;
	}
	else{
		printf("<%s>%d Error:Net print error\n",__FILE__,__LINE__);
		return -1;
	}
}

static T_DebugOpr gtNetDebugOpr = {
	.name = "NetDebug",
	.iCanUse = 1,
	.DebugPrint = NetDebugPrint,
};

int NetDebugRegister()
{
	return DebugOprRegister(&gtNetDebugOpr);	
}


