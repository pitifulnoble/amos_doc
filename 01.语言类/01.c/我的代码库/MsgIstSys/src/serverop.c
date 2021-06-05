#include <stdio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include<sys/types.h>
#include <arpa/inet.h>
#include<sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "serverop.h"

ClnPoint *P_clnPointList=NULL;

#define OUT

int AddNode(ClnPoint *Head,char *name,int cfd, OUT int *Tag);

int DeletNode(ClnPoint *Head,int tag);

int WriteNote(char *name);

int MngServer_InitInfo()
{
	int           fd=0;
	int           ret=0;
	char          fileName[64]={0};
	
	strcpy(fileName,getenv("HOME"));
	strcat(fileName,"/.MsgServerInfo");
	
	ret=access(fileName,F_OK);
	if(ret!=0)
	{
		fd=open(fileName,O_WRONLY|O_CREAT|O_TRUNC,0644);
		if(fd<0)
		{
			perror("fun open() err!");
			return FILE_OPEN_FAIL;
		}
		ret=write(fd,"0",1);
		if(ret<0)
		{
			perror("fun write() err!");
			return ret;
		}
		close(fd);
	}
	
		

		
	return 0;
}

void *tfunc(void*arg)
{
	return NULL;
}

int Listen( int *cfd)
{
	int                 ret=0;
	int                 cfd_t=0;
	int                 sfd=0;
	struct sockaddr_in  serv_addr, client_addr;
	socklen_t           addr_len=0;
	int                 read_len=0;
	pthread_t           tpid;
	char                buf[64];
	char                readBuf[1024];
	char                writeBuf[1024];
	int                 tag=0;
	NodeInfo            nodeInfo;
	
	memset(&serv_addr,0,sizeof(serv_addr));
	memset(&client_addr,0,sizeof(serv_addr));
	
	
	serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(POINT);
  serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  
  sfd=socket(AF_INET,SOCK_STREAM,0);
	if(sfd<0)
	{
		perror("fun socket() err!");
		return ret;
	}
	ret=bind(sfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if(ret<0)
	{
		perror("fun bind() err!");
		return ret;
	}
	//configuer listen...
	listen(sfd,128);
	printf("Waitting connect...\n");
  while(1){
		memset(readBuf,0,sizeof(readBuf));
		memset(writeBuf,0,sizeof(writeBuf));
		memset(buf,0,sizeof(buf));
		memset(&nodeInfo,0,sizeof(nodeInfo));
		
		//wait connect
		addr_len=sizeof(client_addr);
		cfd_t=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);
		if(cfd_t<0)
		{
			perror("fun accept() err!");
			return ret;
		}
		printf("accept from:%s\n",inet_ntop(AF_INET,(void*)(&client_addr.sin_addr.s_addr),&buf,sizeof(buf)));
		read_len=read(cfd_t,readBuf,sizeof(readBuf));
		if(read_len<0)
		{
			perror("fun read() err!");
			return ret;
		}
		ret=WriteNote(readBuf);
		if(ret!=0)
		{
			perror("fun WriteNote() err!");
			return ret;
		}
		
		ret=AddNode(P_clnPointList,readBuf,cfd_t,&tag);
		printf("Tag>>:%d\n",tag);
		if(ret!=0)
		{
			perror("fun AddNode() err!");
			return ret;
		}
		
		sprintf(writeBuf,"%d\0",tag);
		write(cfd_t,writeBuf,strlen(writeBuf));
		
		strcpy(nodeInfo.name,readBuf);
		nodeInfo.tag=tag;
		nodeInfo.cfd=cfd_t;
		
		pthread_create(&tpid,NULL,tfunc,(void*)&nodeInfo);
	}
	return 0;
}


int create_th(int cfd)
{
	return 0;
}


int AddNode(ClnPoint *Head,char *name,int cfd, int *Tag)
{
	ClnPoint         *pclnPoint;
	int              i=0;
	int              tag=0;
	ClnPoint         *pHead=NULL;
	ClnPoint         *pTmp=NULL;
	
	pHead=Head;
	pTmp=pHead;
	pclnPoint=(ClnPoint*)malloc(sizeof(ClnPoint));
	memset(pclnPoint,0,sizeof(ClnPoint));
	
	strcpy(pclnPoint->name,name);
	pclnPoint->cfd=cfd;
	if(pHead!=NULL)
	{
		for(i=0;;i++)
		{
			if(pTmp->next!=NULL)
			{
				pTmp=pTmp->next;
				continue;
			}
			break;
		}
		tag=pTmp->tag;
		pclnPoint->tag=tag+1;
		pclnPoint->prev=pTmp;
		pTmp->next=pclnPoint;
	}else if(pHead==NULL)
		{
			pclnPoint->tag=1;
			Head=pclnPoint;
		}
	
	*Tag=pclnPoint->tag;
	
	return 0;
}

int DeletNode(ClnPoint *Head,int tag)
{
	return 0;
}

int WriteNote(char *name)
{
	int              NumClient=0;
	int              i=0;
	FILE             *fd=NULL;
	char             FileBuf[64]={0};
	char             fileName[20]={0};
	
	strcpy(fileName,getenv("HOME"));
	strcat(fileName,"/.MsgServerInfo");
	
	fd=fopen(fileName,"a+");
	if(fd==NULL)
	{
		fprintf(stderr,"fun fopen() err:%d\n",FILE_OPEN_FAIL);
		return FILE_OPEN_FAIL;
	}
	
	if(fgets(FileBuf,sizeof(FileBuf),fd)==NULL)
	{
		fprintf(stderr,"fun fgets() err:%d\n",FILE_ERROR);
		return FILE_ERROR;
	}
	if(FileBuf[strlen(FileBuf)-1]=='\n')
	{
		FileBuf[strlen(FileBuf)-1]='\0';
	}
	
	NumClient=atoi(FileBuf);
	
	//Sig In
	//write Client name
	for(i=0;i<NumClient;i++)
	{
		if(fgets(FileBuf,sizeof(FileBuf),fd)==NULL)
		{
			fprintf(stderr,"fun fgets() err:%d\n",FILE_ERROR);
			return FILE_ERROR;
		}
		if(FileBuf[strlen(FileBuf)-1]=='\n')
		{
			FileBuf[strlen(FileBuf)-1]='\0';
		}
		if(strcmp(name,FileBuf)==0)
		{
			break;
		}
	}
	
	if(i==NumClient)
	{
		fprintf(fd,"\n%s",name);
	}else if(i<NumClient)
		{
			goto End;
		}
	fclose(fd);
	fd=fopen(fileName,"r+");
	if(fd==NULL)
	{
		fprintf(stderr,"fun fopen() err:%d\n",FILE_OPEN_FAIL);
		return FILE_OPEN_FAIL;
	}
	//reset Client Num
	//fseek(fd,0,SEEK_SET);
	if(NumClient!=9)
	{
		fprintf(fd,"%d",NumClient+1);
	}else if (NumClient==9) 
		{
			fprintf(fd,"%d\n",NumClient+1);
		}
End:
	fclose(fd);
	return 0;
}





