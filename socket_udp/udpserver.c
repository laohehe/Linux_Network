#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc, char **argv)
{
	struct sockaddr_in s_addr, c_addr;
	int sock, len, recdata = 0;
	socklen_t addr_len;
	char buf[128], buf2[128];
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)  //SOCK_DGRAM UDP协议
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf("create socket connected.\n");
	}

	memset(&s_addr, 0, sizeof(struct sockaddr_in));  //结构体变量初始化
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(8888);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (struct sockaddr *)&s_addr, sizeof(s_addr)) == -1)
	{
		perror("bind failed");
		exit(1);
	}
	else
	{
		printf("bind address to socket\n");
	}
	addr_len = sizeof(c_addr);
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		len = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&c_addr, &addr_len);
		if (len < 0) //接收出错
		{
			perror("recvfrom error\n");
			exit(errno);
		}
		buf[len] = '\0';
		printf("receive from %s:%d message:%s\n\r", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port), buf);
		recdata = atoi(buf);
		recdata++;             //自增1  返回给客户端
		sprintf(buf2, "%d", recdata);
		printf("buf2 : %s\n", buf2);
		sendto(sock, buf2, strlen(buf2), 0, (struct sockaddr *)&c_addr, addr_len);
	}

	//关闭socket
	close(sock);
	return 0;
}
