#include <stdio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/shm.h>
#include "clientop.h"

int Log_Sign(ClientInfo *clientInfo)
{
	int                  ret=0;
	int                  sfd=0;
	int                  read_len=0;
	char                 buf[1024];
	ClientInfo           clientInfo_t;
	struct sockaddr_in serv_addr;
	
	memset(buf,0,sizeof(buf));
	memcpy(&clientInfo_t,clientInfo,sizeof(ClientInfo));
	sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("fun socket() err!");
		return ret;
	}
	memset(&serv_addr,0,sizeof(serv_addr));
	
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(POINT);
	inet_pton(AF_INET,SERIP,&serv_addr.sin_addr.s_addr);
	
	ret=connect(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(ret<0)
	{
		perror("fun connect() err!");
		return ret;
	}
	
	ret=write(sfd,clientInfo_t.name,strlen(clientInfo_t.name));
	if(ret<0)
	{
		perror("fun write() err!");
		return ret;
	}
	
	read_len=read(sfd,buf,sizeof(buf));
	if(read_len<0)
	{
		perror("fun read() err!");
		return ret;
	}
	
	clientInfo->tag=atoi(buf);
	clientInfo->sfd=sfd;
	
	return 0;
}

int SendMsg(ClientInfo clientInfo)
{
	int                  ret=0;
	int                  sfd=0;
	int                  read_len=0;
	char                 writeBuf[1024];
	char                 readBuf[1024];
	
	memset(writeBuf,0,sizeof(writeBuf));
	memset(readBuf,0,sizeof(readBuf));
	sfd=clientInfo.sfd;
	while(1)
	{
		
		printf("To>>:");
		if(fgets(writeBuf,sizeof(writeBuf),stdin)==NULL)
		{
			fprintf(stderr,"fun fgets() err!");
			return -1;
		}
		ret=write(sfd,writeBuf,strlen(writeBuf));
		if(ret<0)
		{
			perror("fun write() err!");
			return SEND_ERROR;
		}
		read_len=read(sfd,readBuf,sizeof(readBuf));
		if(read_len<0)
		{
			perror("fun read() err!");
			return READ_ERROR;
		}
		if (readBuf[1]==USER_ERR) 
		{
			fprintf(stderr,"User Not Exsist...\n");
			continue;
		}else if(readBuf[1]==USER_OK)
			{
				printf("Msg>>:");
				if(fgets(writeBuf,sizeof(writeBuf),stdin)==NULL)
				{
					fprintf(stderr,"fun fgets() err!");
					return -1;
				}
				ret=write(sfd,writeBuf,strlen(writeBuf));
				if(ret<0)
				{
					perror("fun write() err!");
					return SEND_ERROR;
				}
			}
	}
	return 0;
}

void *tfunc(void*arg)
{
	int                  ret=0;
	int                  sfd=0;
	int                  read_len=0;
	char                 readBuf[1024];
	ClientInfo           *pclientInfo=NULL;
	
	pclientInfo=(ClientInfo*)arg;
	sfd=pclientInfo->sfd;
	while(1)
	{
		memset(readBuf,0,sizeof(readBuf));
		read_len=read(sfd,readBuf,sizeof(readBuf));
		if(ret<0)
			{
				perror("fun read() err!");
				return (void*)READ_ERROR;
			}
		fprintf(stdout,"%s",readBuf);
	}
}

int ReciveMsg(ClientInfo clientInfo)
{
	
	pthread_t            tpid;
	
	pthread_create(&tpid,NULL,tfunc,(void*)&clientInfo);
	return 0;
}

int Quit()
{
	return 0;
}