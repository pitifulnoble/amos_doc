#ifndef _SERVER_MSG_H_
#define _SERVER_MSG_H_

#define POINT   8001
#define SERIP "127.0.0.1"
#define LOG_IN  50
#define SIN_IN  60

#define USER_OK        '1'
#define USER_ERR       '2'
#define FILE_OPEN_FAIL 80
#define FILE_ERROR     81
#define SENT_ERROR     82
#define READ_ERROR     83

struct clnPoint
{
	char name[64];
	int  tag;       //unique tag
	int  cfd;  
	struct clnPoint *next;
	struct clnPoint *prev;
};
typedef struct clnPoint ClnPoint;

extern ClnPoint *P_clnPointList;

typedef struct _NodeInfo
{
	char name[64];
	int  tag;
	int  cfd;
}NodeInfo;


//**********************************************************
//*open user recode file
//*
//**********************************************************
int MngServer_InitInfo();

int Listen(int *cfd);

int create_th(int cfd);


#endif