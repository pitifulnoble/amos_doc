#ifndef _SERVER_MSG_H_
#define _SERVER_MSG_H_

#define POINT   8001
#define SERIP   "127.0.0.1"
#define LOG_IN  50
#define SIN_IN  60


#define USER_OK        '1'
#define USER_ERR       '2'
#define FILE_OPEN_FAIL 80
#define FILE_ERROR     81
#define SEND_ERROR     82
#define READ_ERROR     83

typedef struct _clientInfo
{
	char name[64];
	int  tag;
	int  sfd;
}ClientInfo;

int Log_Sign(ClientInfo *clientInfo);

int SendMsg(ClientInfo clientInfo);

int ReciveMsg(ClientInfo clientInfo);

int Quit();






#endif