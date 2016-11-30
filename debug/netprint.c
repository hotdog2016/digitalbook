#include<mydebug.h>
#include<network.h>
#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/socket.h>
#define                           CYCLIC_BUFFER_SIZE               1024

static T_DebugOpr gt_net_debugopr;
static int write=0 , read=0;
static char  g_cyclic_buffer[CYCLIC_BUFFER_SIZE];
int server_fd;
static int writeBuffer(char* buffer , char const * source , int buffer_size)
{
	int i;
	for(i=0 ; i<buffer_size ; i++)
	{
		if((write+1) % CYCLIC_BUFFER_SIZE == read)
		{
			printf("Cyclic buffer is full\n");
			return -1;
		}
			strncpy(&buffer[write] , &source[i] ,1);	
			write = (write + 1) % CYCLIC_BUFFER_SIZE;
	}
	return 0;
}

static int readBuffer(char const * buffer , int buffer_size)
{
	int i;
	int server_fd;
	GetSocketfd(&server_fd,"TCP");
	for(i=0 ; i<buffer_size ; i++)
	{
		if(read == write)
		{
			printf("Cyclic is empty\n");
			return -1;
		}
		send(server_fd, &buffer[read], 1, 0);
		read = (read + 1) % CYCLIC_BUFFER_SIZE;
	}
	return 0;
}

static int NetDebugPrint(char *debug_msg)
{
	int string_len = 0; 

	string_len = strlen(debug_msg);

	if(debug_msg)	
	{
		writeBuffer(g_cyclic_buffer , debug_msg , string_len);
		readBuffer(g_cyclic_buffer ,string_len);
		return 0;
	}
	else{
		printf("<%s>%d Error:Net print error\n",__FILE__,__LINE__);
		return -1;
	}
}

static T_DebugOpr gt_net_debugopr = {
	.name = "NetDebug",
	.iCanUse = 1,
	.DebugPrint = NetDebugPrint,
};

int NetDebugRegister()
{
	return DebugOprRegister(&gt_net_debugopr);	
}


