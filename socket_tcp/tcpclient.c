#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8888
int main(int argc, char **argv)
{
    printf("hello from tcpclient!\n");
	int sock;
	struct sockaddr_in my_addr;
	int len;
	char buf[100];
	char recbuf[100];
	if (argc <2)
	{
		printf("Usage: %s <ip>\n", argv[0]);
		exit(1);
	}
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket create error!\n");
		exit(1);
	}
	memset(&my_addr, 0, sizeof(my_addr));  //内存初始化
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	if (inet_aton(argv[1], (struct in_addr*)&my_addr.sin_addr.s_addr) == 0)
	{
		perror("change error!\n");
		exit(1);
	}
	if (connect(sock,(struct sockaddr*)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		printf("connect error!\n");
		exit(1);
	}

	printf("connected!\n");   //连接成功
	printf("Input data to send:\n");
	fgets(buf, 100, stdin);
	//strcpy(buf, "456");
	printf("%s\n", buf);
	printf("strlen(buf):%d\n", strlen(buf));
	len = send(sock,buf,strlen(buf)-1,0);
	printf("len:%d\n", len);
	if (len < 0)
	{
		perror("send error!\n");
		exit(1);
	}
	sleep(1);
	len = recv(sock, recbuf, 100, 0);
	recbuf[len] = '\0';
	if (len < 0)
	{
		perror("recv error!\n");
		exit(1);
	}
	printf("the reccived data from server is %s\n", recbuf);
	close(sock);
    return 0;
}
