#ifndef _CONFIG_H_
#define _CONFIG_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#define OUT
#define IN

//create many memory spases.
////***************************************************************************************************************
////the function need any number of paraments,but the paraments number must be two times except fist one.so the fist
////parament is number of your input paraments(except itself).then the second parament is number of memory spaces
////that you want create,the unit is char.the next parament is address of pointer(**pp),means a pointer to a pointer
////eg:
////get_mem(4,100,&pointer1,200,&pointer2)
////***************************************************************************************************************
extern int get_mem(IN int n_values, ...);





//free memory spaces
////this function is uesd cooperatily with get_mam function.
////***************************************************************************************************
////this function recevie any number of paraments.
////the fist paraments is number of paraments.other paraments is pointer of needing to be free
////eg:
////free_mem(2,pointer1,pointer2);
////***************************************************************************************************
extern int free_mem(IN int n_values, ...);
#endif
