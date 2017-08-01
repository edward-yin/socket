#include<netinet/in.h>                         // for sockaddr_in  
#include<sys/types.h>                          // for socket  
#include<sys/socket.h>                         // for socket  
#include<stdio.h>                              // for printf  
#include<stdlib.h>                             // for exit  
#include<string.h>                             // for bzero  
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("usage: ./client2 <ipaddress>\n");
		exit(0);
	}
	
	struct sockaddr_in serv_addr;
	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(1234);
	if (inet_pton(AF_INET,argv[1],&serv_addr.sin_addr) <= 0){
		printf("inet_pton error: %s\n",argv[1]);
		exit(0);
	}
	
	int clnt_sock;
	if ((clnt_sock = socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("create socket error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	socklen_t servlen = sizeof(serv_addr);
	
	if (connect(clnt_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1){
		printf("connect error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	char *filename = "t.txt";
	char buffer[BUF_SIZE];
	FILE *fp = fopen(filename,"wb");
	if(fp == NULL){
		printf("open file error: %s(errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	int len,wlen;
	while(len = recv(clnt_sock,buffer,BUF_SIZE,0)){
		if(len < 0){
			printf("recv error: %s(errno:%d)\n",strerror(errno),errno);
			exit(0);
		}
		wlen = fwrite(buffer,sizeof(char),len,fp);
		if (wlen<len){
			printf("file write error: %s(errno:%d)\n",strerror(errno),errno);
			exit(0);
		}
		bzero(buffer,BUF_SIZE);
	}
	
	printf("file transfer succeed!\n");
	
	fclose(fp);
	close(clnt_sock);
	return 0;
}

