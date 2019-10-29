#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8888

int main(int argc, char **argv) 
{
	int sock;
	struct sockaddr_in my_addr;
	int len;
	char buf[100];
	char recbuf[100];
	if (argc < 2)
	{
		printf("Usage: %s <message>", argv[0]);
		exit(1);
	}

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket create error.\n");
		exit(1);
	}
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(PORT);
	//地址赋值
	if (inet_aton("127.0.0.1",(struct sockaddr *)&my_addr.sin_addr.s_addr) == 0)
	{
		perror("change address error.\n");
		exit(1);
	}

	//连接
	if (connect(sock,(struct sockaddr *)&my_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("connect error.\n");
		exit(1);
	}
	printf("argv[1]:%s,len:%d\n", argv[1], strlen(argv[1]));
	len = send(sock, argv[1], strlen(argv[1]) - 1,0);
	if (len < 0)
	{
		perror("send error.\n");
		exit(1);
	}
	sleep(1);
	close(sock);
}
