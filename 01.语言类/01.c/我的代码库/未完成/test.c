#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"my_head.h"
#define OUT
#define IN
//**************************************************************************************
//des:
//This function analyze similaly below string:
//"key1 = value1"
//"key2 =       value2"
//"key3  = value3"
//"key4     =     "
//"key5     ="
//find value by key
//
//keyvaluebuf : IN source env string
//keybuf      : IN key
//valuebuf    : OUT string of value
//valuebuflen : OUT lenth of value
//**************************************************************************************
int getKeyByValue(IN char *keyvaluebuf,IN char *keybuf,OUT char **valuebuf,OUT int *valuebuflen)
{
	if(keyvaluebuf == NULL || keybuf == NULL || valuebuf == NULL || valuebuflen == NULL)
	{
		fprintf(stderr,"keyvaluebuf || keybuf || valuefuf || valuebuflen is NULL !");
		return -1;
	}
	char *key=NULL;
	char *start_value=NULL;
	char *end_value=NULL;
	char *value=NULL;//it is value.
	int lenth_value=0;
	//if keybuf in keyvaluebuf?
	key = strstr(keyvaluebuf,keybuf);
	if(key == NULL)
	{
		fprintf(stderr,"key not exist!");
		return -1;
	}
	//if keyvaluebuf have character '='
	start_value=key+strlen(keybuf);
	for(;isspase(value);value++){
		;
	}
	if(start_value != '='){
		fprintf(stderr,"error : has no '='!");
		return -1;
	}
	else
	{
		start_value++;
	}
	//get valuebuf
	end_value=start_value;
	for(;end_value!='\n';end_value++)
	{
		;
	}
	*end_value = '\0';//replace '\n' to '\0'
	get_no_space_string(start_value,&value);//get value.
	//cont valuebuflen
	lenth_value=strlen(value);
	//assign values
	*valuebuflen=lenth_value;
	*valuebuf = value;
	return 0;
}
