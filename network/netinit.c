#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_ntoa()函数的头文件
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <network.h>

#define		SERVER_PORT                    6666
#define  	MSG_BUFFER_SIZE                1024
#define  	IP_BUFFER_SIZE                 16

int server_fd , new_fd;
struct sockaddr_in server_addr , client_addr;
char * server_ip = "192.168.1.121";
char msg_buffer[MSG_BUFFER_SIZE];
char ip_buffer[IP_BUFFER_SIZE];
T_NetEntity gt_tcpnet;

static int TcpServerInit()
{
	int err;
	server_fd = socket(AF_INET, SOCK_STREAM , 0);

	if(server_fd < 0)
	{
		printf("socket error!\n");
		return -1;
	}

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family         = AF_INET;
	server_addr.sin_addr.s_addr    = inet_addr(server_ip) ;
	server_addr.sin_port           = htons(SERVER_PORT);

    err = bind(server_fd, (struct sockaddr *)&server_addr,sizeof(struct sockaddr));
	if(err < 0)
	{
		printf("bind error!\n");
		return -1;
	}

	err = listen(server_fd , 2);
	if(err < 0)
	{
		printf("listen error!\n");
		return -1;
	}

	return 0;

}

static int TcpNetConnect()
{	
	int err;
	struct sockaddr_in *p_ipaddr=NULL;
	int sin_size;

	sin_size = sizeof(struct sockaddr_in);

	err = TcpServerInit();

	if(err < 0)
	{
		printf("Server Init error\n");
		return -1;
	}

	new_fd=accept(server_fd, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);

	if(new_fd < 0)
	{
		fprintf(stderr,"Acceot Error:%s\n",strerror(errno));
		return -1;
	}

	p_ipaddr = (struct sockaddr_in *)&client_addr;

	if(NULL == inet_ntop(AF_INET ,&p_ipaddr->sin_addr, ip_buffer , IP_BUFFER_SIZE))
	{
		printf("Can't get client addr\n");
		return -1;
	}

//	while(1)
//	{
//		err = recv(new_fd , msg_buffer , MSG_BUFFER_SIZE , 0);
//		if(err < -1)
//		{
//			printf("recv error\n");
//			return -1;
//		}
//		printf("recv msg : %s",msg_buffer);
//	}
	return 0;
}

int TcpNetInit()
{
	int err;
	TcpNetConnect();
	gt_tcpnet.protocol  = "TCP";
	gt_tcpnet.server_fd = new_fd;
	err = NetworkRegister(&gt_tcpnet);
	if(err<0)
	{
		printf("<%s> %d Error:NetworkRegister error\n",__FILE__ , __LINE__);
	}
	return 0;
}

