#include <stdio.h>
#include <sys/socket.h>//linux平台下头文件
#include <error.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8888

//struct in_addr
//{
//	u_long s_addr;
//};
//
//struct sockaddr_in
//{
//	unsigned short sin_family;    //协议簇，类似于socket调用中的domain，一般为AF_INET
//	unsigned short int sin_port;  //绑定的TCP/UDP协议的端口号
//	struct in_addr sin_addr;      //指定所绑定的IP地址
//	unsigned char sin_zero[8];    //填充0目的使sizeof(sockaddr_in) == sizeof(sockaddr)
//};

int main()
{
	printf("hello from SocketStream_Demo!\n");

	int sock, new_sock;
	struct sockaddr_in my_addr, client_addr;
	int len;
	char buf[100];
	char buf2[128];
	int recdata = 0;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)//创建一个socket套接字
	{
		perror("socket create error!\n");
		exit(1);
	}
	printf("sock:%d\n", sock);
	memset(&my_addr, 0, sizeof(my_addr));  //内存初始化
	my_addr.sin_family = AF_INET;     //IPv4协议簇，最为常用
	my_addr.sin_port = htons(PORT);  //Host to Network Short
	my_addr.sin_addr.s_addr = INADDR_ANY;  //0.0.0.0  本机
	if (bind(sock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1)   //把一个地址族中的特定地址赋给socket
	{
		perror("bind error!\n");
		exit(1);
	}

	if (listen(sock, 5) < 0) //开启监听，最多支持5个可以排队的连接数
	{
		perror("listen error!\n");  //将上一个函数发生错误的原因输出到标准设备(stderr)
		exit(1);
	}
	while (1)
	{
		len = sizeof(struct sockaddr);
		if ((new_sock = accept(sock, (struct sockaddr*)&client_addr, &len)) < 0)
		{
			perror("accept error!\n");
			exit(1);
		}
		else
		{
			printf("server:get connection from %s, port %d socket %d \n", inet_ntoa(client_addr.sin_addr),
				ntohs(client_addr.sin_port), new_sock); //ntohs: Network to Host Short
		}

		len = recv(new_sock, buf, 100, 0);
		if (len < 0)
		{
			printf("recv error!\n");
			exit(1);
		}
		else if (len == 0)
		{
			printf("the client quit!\n");
			exit(1);
		}
		else
		{
			buf[len] = '\0';
			printf("receive message: %s \n", buf);
			recdata = atoi(buf);
			recdata++;
			sprintf(buf2, "%d", recdata);
			if ((send(new_sock, buf2, strlen(buf2), 0)) < 0)
			{
				perror("send data failed\n");
			}
		}
	}
	close(sock);
	close(new_sock);
	return 0;
}
