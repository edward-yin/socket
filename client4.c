#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

int main(int argc,char *argv[])
{
	int sock,n;
	char recvline[BUF_SIZE],sendline[BUF_SIZE];
	struct sockaddr_in serv_addr;
	
	if (argc != 2){
		printf("usage: ./client2 <ipaddress>\n");
		exit(0);
	}
	while(1){
	bzero(&sendline,BUF_SIZE);
	bzero(&recvline,BUF_SIZE);
	if ((sock = socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("create socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1234);
	if (inet_pton(AF_INET,argv[1],&serv_addr.sin_addr) <= 0){
		printf("inet_pton error: %s\n",argv[1]);
		exit(0);
	}
	
	if (connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
		printf("connect error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}

	
	printf("send msg to server:\n");
	fgets(sendline,BUF_SIZE,stdin);
	if (send(sock,sendline,strlen(sendline),0) < 0){
		printf("send msg error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	
	}
	recv(sock,recvline,BUF_SIZE,0);
	printf("recv the echo:%s\n",recvline);
	

	}	
	close(sock);
	exit(0);
}

