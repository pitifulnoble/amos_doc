#define  _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <time.h>

#include <string.h>
#include <locale.h>
#include <stdlib.h>

#include "itcast_asn1_der.h"
#include "itcastderlog.h"
#include "keymng_msg.h"

/*
typedef struct _Teacher
{
	char	name[64];
	int		age;
	char	*p;
	int		plen;
}Teacher;

*/

int Teacher_Encode_(Teacher *pstruct, ITCAST_ANYBUF **tmpNode)
{
	int                     ret = 0;
	ITCAST_ANYBUF           *pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF           *pTmpBuf = NULL;
	ITCAST_ANYBUF           *pOutData = NULL;

	unsigned char           *tmpOut = NULL;
	int                     tmpOutLen = 0;

	//Encode name
	//char_buf to ANYBUF
	DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->name, 64);
	if (ret != 0) {
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err:%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &pHeadBuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pHeadBuf;

	//Encode age
	ret = DER_ItAsn1_WriteInteger(pstruct->age, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	pTmpBuf = pTmpBuf->next;

	//Encode p
	ret = EncodeChar(pstruct->p, pstruct->plen, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func EncodeChar() err:%d \n", ret);
		return ret;
	}
	pTmpBuf = pTmpBuf->next;

	//Encode plen
	ret = DER_ItAsn1_WriteInteger(pstruct->plen, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func EncodeChar() err:%d \n", ret);
		return ret;
	}

	//TLV struct
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}
	
	//Indirect assignment
	DER_ITCAST_FreeQueue(pHeadBuf);
	*tmpNode = pOutData;
	
	return 0;
}

int Teacher_Decode_(unsigned char *p, int len, Teacher **pstruct)
{
	int                         ret = 0;
	ITCAST_ANYBUF               *pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF               *pTmpDABuf = NULL;
	ITCAST_ANYBUF               *pOutData = NULL;
	ITCAST_ANYBUF               *inAnyBuf = NULL;
	int                         tmpNum = 0;
	Teacher                     *pTmpStru = NULL;

	inAnyBuf = (ITCAST_ANYBUF*)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func Teacher_Decode_() malloc err；%d \n", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF));
	inAnyBuf->pData = (unsigned char*)malloc(len);
	if (inAnyBuf->pData == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func Teacher_Decode_() malloc err；%d \n", ret);
		return ret;
	}
	inAnyBuf->dataLen = len;
	memcpy(inAnyBuf->pData, p, len);

	//analyze struct
	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(inAnyBuf);
		fprintf(stderr, "func DER_ItAsn1_ReadSequence() err；%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);
	pTmp = pHead;

	//allocation struct
	pTmpStru = (Teacher*)malloc(sizeof(Teacher));
	if (pTmpStru == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func Teacher_Decode_() malloc err；%d \n", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(Teacher));

	//Decode name
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		fprintf(stderr, "func DER_ItAsn1_ReadPrintableString() err；%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->name, pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf);

	//Decode age
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->age));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err；%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//Decode p
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		fprintf(stderr, "func DER_ItAsn1_ReadPrintableString() err；%d \n", ret);
		return ret;
	}
	pTmpStru->p = (unsigned char*)malloc(pTmpDABuf->dataLen + 1);
	if (pTmpStru->p == NULL) {
		ret = KeyMng_MallocErr;
		DER_ITCAST_FreeQueue(pTmpDABuf);
		DER_ITCAST_FreeQueue(pHead);
		fprintf(stderr, "func Teacher_Decode_() malloc err；%d \n", ret);
		return ret;
	}
	memcpy(pTmpStru->p, pTmpDABuf->pData, pTmpDABuf->dataLen);
	pTmpStru->p[pTmpDABuf->dataLen] = '\0';
	pTmp = pTmp->next;
	DER_ITCAST_FreeQueue(pTmpDABuf);

	//Decode plen
	ret = DER_ItAsn1_ReadInteger(pTmp, &pTmpStru->plen);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		fprintf(stderr, "func DER_ItAsn1_ReadPrintableString() err；%d \n", ret);
		return ret;
	}

	*pstruct = pTmpStru;
	DER_ITCAST_FreeQueue(pHead);
	return 0;
}


void Teacher_Free_(Teacher *pStruct)
{
	if (pStruct == NULL) {
		return;
	}
	if (pStruct->p) {
		free(pStruct->p);
	}

	free(pStruct);
}


/*
typedef struct _MsgKey_Req
{
	int				cmdType;		//报文命令码
	char			clientId[12];	//客户端编号
	char			AuthCode[16];	//认证码
	char			serverId[12];	//服务器端I编号
	char			r1[64];		//客户端随机数

}MsgKey_Req;
*/

int MsgKey_Req_Encode_(MsgKey_Req *pstruct, ITCAST_ANYBUF **tmpNode)
{
	int                     ret = 0;
	ITCAST_ANYBUF           *pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF           *pTmpBuf = NULL;
	ITCAST_ANYBUF           *pOutData = NULL;

	unsigned char           *tmpOut = NULL;
	int                     tmpOutLen = 0;

	//Encode cmdType
	ret = DER_ItAsn1_WriteInteger(pstruct->cmdType, &pHeadBuf);
	if (ret != 0) {
		fprintf(stderr, "func DER_ItAsn1_WriteInteger() err；%d \n", ret);
		return ret;
	}
	pTmpBuf = pHeadBuf;

	//Encode clientId[12]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->clientId, 12);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode AuthCode[16]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->AuthCode, 16);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode serverId[12]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->serverId, 12);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode r1[64]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->r1, 64);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	
	//Encode TLV struct
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadBuf);

	//Indirect assignment
	*tmpNode = pOutData;

	return 0;
}

int MsgKey_Req_Decode_(unsigned char *p, int len, MsgKey_Req **pstruct)
{
	int                         ret = 0;
	ITCAST_ANYBUF               *pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF               *pTmpDABuf = NULL;
	ITCAST_ANYBUF               *pOutData = NULL;
	ITCAST_ANYBUF               *inAnyBuf = NULL;
	int                         tmpNum = 0;
	MsgKey_Req                  *pTmpStru = NULL;

	//restore struct
	inAnyBuf = (ITCAST_ANYBUF*)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF));
	inAnyBuf->pData = (unsigned char*)malloc(len);
	if (inAnyBuf->pData == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memcpy(inAnyBuf->pData, p, len);
	inAnyBuf->dataLen = len;

	//analyze TLV struct
	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(inAnyBuf);
		fprintf(stderr, "func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);
	pTmp = pHead;

	//Create struct
	pTmpStru = (MsgKey_Req*)malloc(sizeof(MsgKey_Req));
	if (pTmpStru == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(MsgKey_Req));

	//Decode cmdType
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->cmdType));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//Decode clientId
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->clientId, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Decode AuthCode[16]
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->AuthCode, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Decode serverId[12]
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->serverId, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Decode r1[64]
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->r1, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Indirect assignment
	DER_ITCAST_FreeQueue(pHead);
	*pstruct = pTmpStru;

	return 0;
}

void MsgKey_Req_Free_(MsgKey_Req *pstruct)
{
	if (pstruct == NULL) {
		return;
	}
	free(pstruct);
}



/*

typedef struct  _MsgKey_Res
{
	int					rv;				//返回值
	char				clientId[12];	//客户端编号
	char				serverId[12];	//服务器编号
	unsigned char		r2[64];			//服务器端随机数
}MsgKey_Res;
*/

int MsgKey_Res_Encode_(MsgKey_Res *pstruct, ITCAST_ANYBUF **tmpNode)
{
	int                     ret = 0;
	ITCAST_ANYBUF           *pTmp = NULL, *pHeadBuf = NULL;
	ITCAST_ANYBUF           *pTmpBuf = NULL;
	ITCAST_ANYBUF           *pOutData = NULL;

	unsigned char           *tmpOut = NULL;
	int                     tmpOutLen = 0;

	//Encode rv
	ret = DER_ItAsn1_WriteInteger(pstruct->rv, &(pHeadBuf));
	if (ret != 0) {
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmpBuf = pHeadBuf;

	//Encode clientId[12]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->clientId, 12);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode serverId[12]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->serverId, 12);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode r2[64]
	ret = DER_ITCAST_String_To_AnyBuf(&pTmp, pstruct->r2, 12);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ITCAST_String_To_AnyBuf() err；%d \n", ret);
		return ret;
	}
	ret = DER_ItAsn1_WritePrintableString(pTmp, &(pTmpBuf->next));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		DER_ITCAST_FreeQueue(pTmp);
		fprintf(stderr, "func DER_ItAsn1_WritePrintableString() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);
	pTmpBuf = pTmpBuf->next;

	//Encode TLV struct
	ret = DER_ItAsn1_WriteSequence(pHeadBuf, &pOutData);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadBuf);
		fprintf(stderr, "func DER_ItAsn1_WriteSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pHeadBuf);

	//Indirect assignment
	*tmpNode = pOutData;

	return 0;
}

int MsgKey_Res_Decode_(unsigned char *p, int len, MsgKey_Res **pstruct)
{
	int                         ret = 0;
	ITCAST_ANYBUF               *pTmp = NULL, *pHead = NULL;
	ITCAST_ANYBUF               *pTmpDABuf = NULL;
	ITCAST_ANYBUF               *pOutData = NULL;
	ITCAST_ANYBUF               *inAnyBuf = NULL;
	int                         tmpNum = 0;
	MsgKey_Res                  *pTmpStru = NULL;

	//restore struct
	inAnyBuf = (ITCAST_ANYBUF*)malloc(sizeof(ITCAST_ANYBUF));
	if (inAnyBuf == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memset(inAnyBuf, 0, sizeof(ITCAST_ANYBUF));
	inAnyBuf->pData = (unsigned char*)malloc(len);
	if (inAnyBuf->pData == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memcpy(inAnyBuf->pData, p, len);
	inAnyBuf->dataLen = len;

	//analyze TLV struct
	ret = DER_ItAsn1_ReadSequence(inAnyBuf, &pHead);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(inAnyBuf);
		fprintf(stderr, "func DER_ItAsn1_ReadSequence() err:%d \n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(inAnyBuf);
	pTmp = pHead;

	//Create struct
	pTmpStru = (MsgKey_Res*)malloc(sizeof(MsgKey_Res));
	if (pTmpStru == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	memset(pTmpStru, 0, sizeof(MsgKey_Res));

	//Decode rv
	ret = DER_ItAsn1_ReadInteger(pTmp, &(pTmpStru->rv));
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	pTmp = pTmp->next;

	//Decode clientId
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->clientId, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Decode serverId[12]
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->serverId, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Decode r2[64]
	ret = DER_ItAsn1_ReadPrintableString(pTmp, &pTmpDABuf);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHead);
		free(pTmpStru);
		fprintf(stderr, "func DER_ItAsn1_ReadInteger() err:%d \n", ret);
		return ret;
	}
	strncpy(pTmpStru->r2, pTmpDABuf->pData, pTmpDABuf->dataLen);
	DER_ITCAST_FreeQueue(pTmpDABuf);
	pTmp = pTmp->next;

	//Indirect assignment
	DER_ITCAST_FreeQueue(pHead);
	*pstruct = pTmpStru;

	return 0;
}

void MsgKey_Res_Free_(MsgKey_Res *pstruct)
{
	if (pstruct == NULL) {
		return;
	}
	free(pstruct);
}

int MsgEncode(void *pStruct,int	type,unsigned char	**outData,int*outLen) {
	ITCAST_ANYBUF            *pHeadbuf = NULL, *pTemp = NULL;
	ITCAST_ANYBUF            *pOutData = NULL;
	int                      ret = 0;
	unsigned char            *tmpOut = NULL;
	int                      tmpOutLen = 0;

	if (pStruct == NULL || outData == NULL || outLen == NULL) {
		ret = KeyMng_ParamErr;
		fprintf(stderr,"func MsgEncode() err:%d check pStruct == NULL || outData == NULL || outLen == NULL\n",ret);
		return ret;
	}

	//Encode type
	ret = DER_ItAsn1_WriteInteger(type, &pHeadbuf);
	if (ret != 0) {
		fprintf(stderr,"func DER_ItAsn1_WriteInteger() err；%d \n", ret);
		return ret;
	}

	//Encode struct by type
	switch(type)
	{
		case ID_MsgType_Teacher:
			ret = Teacher_Encode_((Teacher*)pStruct, &(pHeadbuf->next));
			if (ret != 0) {
				fprintf(stderr, "func Teacher_Encode_() err；%d \n", ret);
				return ret;
			}
			break;
		case ID_MsgKey_Req:
			ret = MsgKey_Req_Encode_((MsgKey_Req*)pStruct, &(pHeadbuf->next));
			if (ret != 0) {
				fprintf(stderr, "func MsgKey_Req_Encode_() err；%d \n", ret);
				return ret;
			}
			break;
		case ID_MsgKey_Res:
			ret = MsgKey_Res_Encode_((MsgKey_Res*)pStruct, &(pHeadbuf->next));
			if (ret != 0) {
				fprintf(stderr, "func MsgKey_Res_Encode_() err；%d \n", ret);
				return ret;
			}
			break;
		default:
			ret = -2;
			fprintf(stderr,"error type\n");
			break;
	}

	//TLV all
	ret = DER_ItAsn1_WriteSequence(pHeadbuf, &pOutData);
	if (ret != 0) {
		DER_ITCAST_FreeQueue(pHeadbuf);
		fprintf(stderr, "func DER_ItAsn1_WriteSequence() err；%d \n", ret);
		return ret;
	}

	//storage allocation
	*outData = (unsigned char*)malloc(pOutData->dataLen);
	if (*outData == NULL) {
		DER_ITCAST_FreeQueue(pHeadbuf);
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgEncode() malloc err；%d \n", ret);
		return ret;
	}
	//indirect assignment
	memcpy(*outData, pOutData->pData, pOutData->dataLen);
	*outLen = pOutData->dataLen;

	DER_ITCAST_FreeQueue(pHeadbuf);
	DER_ITCAST_FreeQueue(pOutData);
	return 0;
}

int MsgDecode(unsigned char *inData,int inLen,void **pStruct,int *type /*out*/) {
	ITCAST_ANYBUF                 *pHeadBuf = NULL, *pTmp = NULL;
	int                           ret = 0;
	unsigned long                 itype = 0;

	if (inData == NULL || pStruct == NULL || type == NULL) {
		ret = KeyMng_ParamErr;
		fprintf(stderr, "func MsgDecode() err:%d check pStruct == NULL || outData == NULL || outLen == NULL\n", ret);
		return ret;
	}

	//bit to ITCAST_ANYBUF
	pTmp = (ITCAST_ANYBUF*)malloc(sizeof(ITCAST_ANYBUF));
	if (pTmp == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgDecode() err:%d malloc error\n", ret);
		return ret;
	}
	//pTmp->next = NULL;
	//pTmp->prev = NULL;
	memset(pTmp, 0, sizeof(ITCAST_ANYBUF));
	pTmp->pData = (unsigned char*)malloc(inLen);
	if (pTmp->pData == NULL) {
		ret = KeyMng_MallocErr;
		fprintf(stderr, "func MsgDecode() err:%d malloc error\n", ret);
		return ret;
	}
	memcpy(pTmp->pData, inData, inLen);
	pTmp->dataLen = inLen;
	
	//annalase biggst struct
	ret = DER_ItAsn1_ReadSequence(pTmp, &pHeadBuf);
	if (ret != 0) {
		fprintf(stderr, "func MsgDecode:DER_ItAsn1_ReadSequence() err:%d malloc error\n", ret);
		return ret;
	}
	DER_ITCAST_FreeQueue(pTmp);

	//analyze tag
	ret = DER_ItAsn1_ReadInteger(pHeadBuf, &itype);
	if (ret != 0) {
		fprintf(stderr, "func MsgDecode:DER_ItAsn1_ReadInteger() err:%d malloc error\n", ret);
		return ret;
	}

	//analyze continue by tags
	switch (itype)
	{
	case ID_MsgType_Teacher:
		ret = Teacher_Decode_(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (Teacher**)pStruct);
		if (ret != 0) {
			fprintf(stderr, "func MsgDecode:Teacher_Decode_() err:%d\n", ret);
			return ret;
		}
		break;
	case ID_MsgKey_Req:
		ret = MsgKey_Req_Decode_(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (MsgKey_Req**)pStruct);
		if (ret != 0) {
			fprintf(stderr, "func MsgDecode:MsgKey_Req_Decode_() err:%d\n", ret);
			return ret;
		}
		break;
	case ID_MsgKey_Res:
		ret = MsgKey_Res_Decode_(pHeadBuf->next->pData, pHeadBuf->next->dataLen, (MsgKey_Res**)pStruct);
		if (ret != 0) {
			fprintf(stderr, "func MsgDecode:MsgKey_Res_Decode_() err:%d\n", ret);
			return ret;
		}
		break;
	default:
		ret = 3;
		fprintf(stderr, "MsgDecode:itype error %d\n",ret);
		break;
	}

	*type = itype;
	DER_ITCAST_FreeQueue(pHeadBuf);
	return 0;
}

int MsgMemFree(void **point, int type) {
	if (point == NULL) {
		return 0;
	}
	if (type == 0) {
		if (*point != NULL) {
			free(*point);
		}
		*point = NULL;
	}
	else
	{
		switch (type)
		{
		case ID_MsgType_Teacher:
			Teacher_Free_(*point);
			*point = NULL;
			break;
		case ID_MsgKey_Req:
			MsgKey_Req_Free_(*point);
			*point = NULL;
			break;
		case ID_MsgKey_Res:
			MsgKey_Res_Free_(*point);
			*point = NULL;
			break;
		default:
			fprintf(stderr, "type error\n");
			break;
		}
	}
	return 0;
}

#if 0
//test Teacher
int main()
{
	int ret = 0;
	Teacher noble;
	unsigned char* outChar = NULL;
	int outLen = 0, type = 0;
	Teacher *OutStruc = NULL;
	memset(&noble, 0, sizeof(Teacher));
	strcpy(noble.name,"WangYuqing");
	noble.age = 21;
	noble.p = (char*)malloc(100);
	strcpy(noble.p, "Hello World!");
	noble.plen = 12;

	ret=MsgEncode((void*)&noble, ID_MsgType_Teacher, &outChar, &outLen);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgEncode() err:%d malloc error\n", ret);
		return ret;
	}
	ret=MsgDecode(outChar, outLen, (void*)&OutStruc, &type);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgDecode() err:%d malloc error\n", ret);
		return ret;
	}

	MsgMemFree((void**)&OutStruc, 1);
	MsgMemFree((void**)&outChar, 0);
	return 0;
}
#endif

#if 0
//test MsgKey_Req
int main()
{
	int ret = 0;
	MsgKey_Req key;
	unsigned char* outChar = NULL;
	int outLen = 0, type = 0;
	MsgKey_Req *OutStruc = NULL;
	memset(&key, 0, sizeof(MsgKey_Req));
	key.cmdType = 200;
	strcpy(key.clientId, "1234567");
	strcpy(key.AuthCode, "7777777");
	strcpy(key.serverId, "0987654");
	strcpy(key.r1, "000000");
	ret = MsgEncode((void*)&key, ID_MsgKey_Req, &outChar, &outLen);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgEncode() err:%d malloc error\n", ret);
		return ret;
	}
	ret = MsgDecode(outChar, outLen, (void*)&OutStruc, &type);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgDecode() err:%d malloc error\n", ret);
		return ret;
	}

	MsgMemFree((void**)&OutStruc, 1);
	MsgMemFree((void**)&outChar, 0);
	return 0;
}
#endif

#if 0
//test MsgKey_Res
int main()
{
	int ret = 0;
	MsgKey_Res key;
	unsigned char* outChar = NULL;
	int outLen = 0, type = 0;
	MsgKey_Res *OutStruc = NULL;
	memset(&key, 0, sizeof(MsgKey_Res));
	key.rv = 200;
	strcpy(key.clientId, "8080");
	strcpy(key.serverId, "80");
	strcpy(key.r2, "123456");

	ret = MsgEncode((void*)&key, ID_MsgKey_Res, &outChar, &outLen);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgEncode() err:%d malloc error\n", ret);
		return ret;
	}
	ret = MsgDecode(outChar, outLen, (void*)&OutStruc, &type);
	if (ret != 0) {
		fprintf(stderr, "func main:MsgDecode() err:%d malloc error\n", ret);
		return ret;
	}

	MsgMemFree((void**)&OutStruc, 1);
	MsgMemFree((void**)&outChar, 0);
	return 0;
}
#endif