#include <stdio.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "serverop.h"




int main()
{
	int            sfd=0;
	int            cfd=0;
	int            ret=0;

	ret=MngServer_InitInfo();
	if(ret!=0)
	{
		fprintf(stderr,"fun MngServer_InitInfo() err:%d\n",ret);
		return ret;
	}
	
	ret=Listen(&cfd);
	if(ret!=0)
	{
		fprintf(stderr,"fun MngServer_InitInfo() err:%d\n",ret);
		return ret;
	}
	
	ret=create_th(cfd);
	if(ret!=0)
	{
		fprintf(stderr,"fun MngServer_InitInfo() err:%d\n",ret);
		return ret;
	}
	return 0;	
}