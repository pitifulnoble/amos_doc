#include <stdio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "clientop.h"


int Usage()
{
    int nSel = -1;
    
    system("clear");    
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n  /*     1.LOG IN                                              */");
    printf("\n  /*     2.SIN IN                                              */");
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n\n  Chioce:");
    scanf("%d", &nSel);
    while(getchar() != '\n');
    system("clear");
    printf("YouName:");
    return nSel;
}

int main()
{
	int                ret=0;
	char               name[64]={0};
	int                usage=0;
	ClientInfo         clientInfo;
	
	memset(&clientInfo,0,sizeof(ClientInfo));
	Usage();
	if(fgets(name,sizeof(name),stdin)==NULL)
	{
		fprintf(stderr,"fun fgets() err!");
		return 0;
	}
	if(name[strlen(name)-1]=='\n')
	{
		name[strlen(name)-1]='\0';
	}
	
	strcpy(clientInfo.name,name);
	printf("main() 1\n");
	ret=Log_Sign(&clientInfo);
	if(ret!=0)
	{
		fprintf(stderr,"fun Log_sign() err:%d\n",ret);
		return ret;
	}
	printf("name>>:%s\n",clientInfo.name);
	printf("tag>>:%d\n",clientInfo.tag);
	printf("sfd>>:%d\n",clientInfo.sfd);

	ReciveMsg(clientInfo);
	if(ret!=0)
	{
		fprintf(stderr,"fun Log_sign() err:%d\n",ret);
		return ret;
	}
	
	SendMsg(clientInfo);
	if(ret!=0)
	{
		fprintf(stderr,"fun Log_sign() err:%d\n",ret);
		return ret;
	}
	
	return 0;	
}