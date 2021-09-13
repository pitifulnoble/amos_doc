/*
 *author: noble;
 *time:2018-06-09
 *vision 1.0
 *file name my_mem.c
 *des:
 *memory management
 *

 cover function:
 get_mem,free_mem

*/
#include"my_mem.h"
//create many memory spases.
//***************************************************************************************************************
//the function need any number of paraments,but the paraments number must be two times except fist one.so the fist
//parament is number of your input paraments(except itself).then the second parament is number of memory spaces
//that you want create,the unit is char.the next parament is address of pointer(**pp),means a pointer to a pointer
//eg:
//get_mem(4,,100,&point1,200,&point2)
//***************************************************************************************************************
int get_mem(IN int n_values, ...)
{
	if(n_values%2!=0)
	{
		fprintf(stderr,"error: number of values error!");
		return -1;
	}

	va_list var_arg;
	va_start(var_arg,n_values);

	for(;n_values>0;n_values-=2){
		char *p1=NULL;//pointer to memory space
		int len1=0;   //the lenth of space
		//read paraments that record size
		len1=va_arg(var_arg,int);
		//create memory spaces
		p1=(char*)malloc(len1);
		if(p1==NULL)
		{
			fprintf(stderr,"error: malloc error!\n");
			return -1;
		}
		memset(p1,0,len1);//memory create over
		*(va_arg(var_arg,char**))=p1;
		//from now the function create space successful
	}
	va_end(var_arg);
	return 0;
}

//free memory spaces
//this function is uesd cooperatily with get_mam function.
//***************************************************************************************************
//this function recevie any number of paraments.
//the fist paraments is number of paraments.other paraments is pointer of needing to be free
//***************************************************************************************************
int free_mem(IN int n_values, ...)
{
	va_list var_arg;
	va_start(var_arg,n_values);

	for(;n_values>0;n_values--){
		char *p=NULL;//the point that will be free
		p=va_arg(var_arg,char*);//assign p variable
		if(p==NULL)
		{
			fprintf(stderr,"error: pointer is NULL!\n");
			return -1;
		}
		free(p);
	}

	va_end(var_arg);
	return 0;
}

/*
int main()
{
	char *p1=NULL;
	char *p2=NULL;
	get_mem(4,100,&p1,200,&p2);
	strcpy(p1,"ABC");
	strcpy(p2,"ZXC");
	printf("%s\n%s\n",p1,p2);
	free_mem(2,p1,p2);
	return 0;
}
*/
