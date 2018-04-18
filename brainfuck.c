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

void brainfuck(char *v,char *given_env,struct mret *ret,int reversefuck){
	char *a,bf;
	int len;
	if(!given_env){
		if(!requested_size){
			a=(char*)calloc(MAX_DIM,sizeof(char));
			len=MAX_DIM;
		}
		else{
			a=(char*)calloc(requested_size,sizeof(char));
			len=requested_size;
		}
	}
	else{
		a=given_env;
		len=strlen(a);
	}
	if(!a) die("error alloc memory");
	char *ip=v,*ptr=a;
	int x;
	while(*ip){
		if(reversefuck) bf=reversefuck_translate(*ip); else bf=*ip;
		switch(bf){
			case '<':
				ptr--;if(ptr<a) ptr=a;
				break;
			case '>':
				ptr++;if(ptr>a+len) ptr=a+len;
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
				//quit returning *ptr value
				ret->ret=*ptr;
				ret->a=a;
				return;
			break;
			case '[':
				if(*ptr==0) {ip=find_next_paren(++ip,reversefuck);--ip;}
			break;
			case ']':
				if(*ptr!=0) {ip=find_prev_paren(--ip,reversefuck);--ip;}
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
	ret->ret=*ptr;
	ret->a=a;
 }
 char* find_next_paren(char *ip,int reversefuck){
	int nest=1;
	char bf;
	while(nest){
		if(reversefuck) bf=reversefuck_translate(*ip); else bf=*ip;
		if(bf=='[') {nest++;}
		if(bf==']') {nest--;}
		++ip;
	}
	return --ip;
 }
 char* find_prev_paren(char *ip,int reversefuck){
	int nest=1;
	char bf;
	while(nest){
		if(reversefuck) bf=reversefuck_translate(*ip); else bf=*ip;
		if(bf==']') {nest++;}
		if(bf=='[') {nest--;}
		--ip;
	}
	return ++ip;
 }
char reversefuck_translate(char c){
	 char x=0;
	 switch(c){
		 case '<':
			 x='>';
			 break;
		 case '>':
			 x='<';
			 break;
		 case '+':
			 x='-';
			 break;
		 case '-':
			 x='+';
			 break;
		 case '[':
			 x=']';
			 break;
		 case ']':
			 x='[';
			 break;
		 case '.':
			 x=',';
			 break;
		 case ',':
			 x='.';
			 break;
		 case '!':
			 x='?';
			 break;
		 case '?':
			 x='!';
			 break;
		 default:
			 x=c;
			 break;
	 }
	 return x;
}

char* tr_char(char c){
	int div=c/10;
	int r=c%10;
	char *x=(char*)calloc(20+div+r+5,sizeof(char));
	char *p;
	int n;
	if(!x) return "";
	p=x;
	for(n=0;n<10;n++){*p++='+';}
	*p++='[';*p++='>';
	for(n=0;n<div;n++){*p++='+';}
	*p++='<';*p++='-';*p++=']';*p++='>';
	for(n=0;n<r;n++){*p++='+';}
	*p++='.';*p++='[';*p++='-';*p++=']';
	
	return x;
}
char* tr_string(char *s){
	char *str;
	str=(char*)calloc(5,sizeof(char));
	int n;
	for(n=0;n<strlen(s);n++){
		str=strcat(str,tr_char(s[n]));
	}
	return str;
}
char* tr_pretty_str(char *s){
	return tr_string(s);
}
