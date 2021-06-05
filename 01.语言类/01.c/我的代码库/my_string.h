#ifndef _CONFIG_H_
#define _CONFIG_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include"my_mem.h"
#define OUT
#define IN
//**********************************************************************************
////Take the pointer that point to fist address of the string and reverse the string
////**********************************************************************************
extern int inverse(char *str);

//*********************************************************************************************
////get two variables,one sourse string, one pointer to int number,
////count the number of no spaces characters,and assign to the int number
////*********************************************************************************************
extern int get_no_space_count(IN const char *str,OUT int *cnt_p);

//*********************************************************************************************
////count number of substring in dest string.
////*********************************************************************************************
extern int get_num_sub(IN const char*dest, IN const char*sub);

//repleace substring
////***********************************************************************************
////src: source string
////dst: need to be create new string
////sub: old substring that will be changede
////new_sub: new substring that will be replace old one.
////warning:
////Not suggest the src and dst is one point
////the dst is point to heat , so you need to free it by yourself
////***********************************************************************************
extern int replaceSubstr(IN char *src, OUT char **dst, IN char *sub ,IN char *new_sub);

//***********************************************************************************
////get two variables,one sourse string,one pointer's address
////(des pointer can point to soures)
////script the spaces of string,and make des_str point to it.
////
////notise: if *des_str!=sourse_str.the des_str is point to heap.that means you need
////to free it by yourself.
////***********************************************************************************
extern int get_no_space_string(IN const char *src_str,char OUT **des_str);

#endif
