#ifndef    _NETWORK_H
#define    _NETWORK_H

typedef struct netentity{ 
	char * protocol;
	int server_fd;
	struct netentity * next;
}T_NetEntity , *P_NetEntity;

int NetworkRegister(P_NetEntity p_netentity);
int NetworkInit();
int TcpNetInit();
int GetSocketfd(int * server_fd ,char * protocol);
#endif



