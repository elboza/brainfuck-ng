/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "main.h"
#include "brainfuck.h"
 
int brainfuck(char *v,char *given_env){
	char *a;
	if(!given_env){
		a=(char*)calloc(0,MAX_DIM);
	}
	else{
		a=given_env;
	}
	if(!a) die("error alloc memory");
	char *ip=v,*ptr=a;
	int x;
	while(*ip){
		switch(*ip){
			case '<':
				ptr--;if(ptr<a) ptr=a;
				break;
			case '>':
				ptr++;
				break;
			case '+':
				++*ptr;
				break;
			case '-':
				--*ptr;
				break;
			case '.':
				putchar(*ptr);
				break;
			case ',':
				*ptr=(char)getchar();
				break;
			case '@':
				//quit returning *ptr vaule
				return(*ptr);
			break;
			case '[':
				if(*ptr==0) {ip=find_next_paren(++ip);--ip;}
			break;
			case ']':
				if(*ptr!=0) {ip=find_prev_paren(--ip);--ip;}
			break;
			case ';':
			break;
			case ':':
				//printf("%d",*ptr);
				break;
			case '!':
				printf("%d ",*ptr);
				break;
			case '?':
				scanf("%d",&x);
				*ptr=(char)x;
				break;
			default:
				
			break;
		}
		ip++;
	}
	return *ptr;
 }
 char* find_next_paren(char *ip){
	int nest=1;
	while(nest){
		if(*ip=='[') {nest++;}
		if(*ip==']') {nest--;}
		++ip;
	}
	return --ip;
 }
 char* find_prev_paren(char *ip){
	 int nest=1;
	 while(nest){
		 if(*ip==']') {nest++;}
		 if(*ip=='[') {nest--;}
		 --ip;
	 }
	 return ++ip;
 }
