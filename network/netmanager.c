#include <types.h>
#include <network.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static    PT_NetEntity    gp_netentity_head;

int NetworkRegister(PT_NetEntity p_netentity)
{
    PT_NetEntity current;
    current = gp_netentity_head;
    if(current == NULL) 
    {
		p_netentity->next = NULL;
        gp_netentity_head = p_netentity; 
        return 0;
    }
    while(current)
    {
        if(NULL == current->next)
        {
			p_netentity->next = NULL;
            current->next = p_netentity;
            return 0;
        }
        current=current->next;
    }
    return -1;
}

int ListLink(PT_NetEntity p_netentity)
{
	PT_NetEntity current;
	current = p_netentity;
	while(current)
	{
		printf("Net Protocol is : %s\n",current->protocol);
		current=current->next;
	}
	return 0;
}

int GetSocketfd(int * server_fd ,char * protocol)
{
	PT_NetEntity current;
	current = gp_netentity_head;

	while(current)
	{
		if( !strcmp(protocol , current->protocol))
		{
			*server_fd = current->server_fd;	
			return 0;
		}
		current = current->next;
	}
	printf("Can't support %s\n",protocol);
	return -1;
}

int NetworkInit()
{
	TcpNetInit();
	ListLink(gp_netentity_head);
	return 0;
}

