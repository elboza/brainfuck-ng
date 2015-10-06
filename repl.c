/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
// #include <editline/readline.h>
// #include <editline/history.h>
#include "main.h"
#include "config.h"
#ifdef HAVE_LIBREADLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif
#include "repl.h"
#include "brainfuck.h"

extern char *strtok_r(char *, const char *, char **);
//for linux u can use __strtok_r
extern char *strdup(const char *s);

// char* rl_gets(char *s){
// 	char* input = readline(s);
// 	add_history(input);
// 	return input;
// }
char *ltrim(char *s)
{
	while(isspace(*s)) s++;
	return s;
}

char *rtrim(char *s)
{
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

char *trim(char *s)
{
	return rtrim(ltrim(s));
}
#ifdef HAVE_LIBREADLINE
char* rl_gets (char *prompt)
{
	static char *line_read = (char *)NULL;
	/* If the buffer has already been allocated,
	 *    return the memory to the free pool. */
	if (line_read)
	{
		free (line_read);
		line_read = (char *)NULL;
	}

	/* Get a line from the user. */
	line_read = (char *)readline (prompt);

	/* If the line has any text in it,
	 *    save it on the history. */
	if (line_read && *line_read)
		add_history (line_read);

	return (line_read);
}
#endif
void repl(char *given_env,struct mret *ret,int reversefuck)
{
	char *cmd,prompt[255];
	quit_shell=0;
	int line_no=1;
	//if(check_funny()) funny_shell_disclaimer(); else normal_shell_disclaimer();
	//printf("entering shell-interactive mode...\n");
	#ifdef HAVE_LIBREADLINE
	rl_variable_bind("blink-match-paren","on");
	#endif
	while(!quit_shell)
	{
		sprintf(prompt,"bfng~%d~ ",line_no++);
		#ifdef HAVE_LIBREADLINE
		cmd=rl_gets(prompt);
		#else
		printf("%s",prompt);
		cmd=(char*)malloc(MAX_CMD);
		fgets(cmd,MAX_CMD,stdin);
		cmd[strlen(cmd) - 1] = '\0';
		#endif
		execute(cmd,given_env,ret,&reversefuck);
		if(ks==1){given_env=ret->a;}
		if(ks==0){if(given_env){free(given_env);given_env=NULL;}}
		#ifndef HAVE_LIBREADLINE
		if(cmd) free(cmd);
		#endif
	}
}
void show_help(){
	printf("brainfuck-ng interpreter, by Fernando Iazeolla 2015(c)\n");
	printf(":q                ~ exit interpreter\n");
	printf(":h                ~ this help\n");
	printf(":l file           ~ load file\n");
	//printf(":env              ~ show interpreter variables\n");
	printf(":ks (yes|no)      ~ keep state environment array after command enter.\n");
	printf(":p                ~ print environment-array to stdout\n");
	printf(":z                ~ print last return value\n");
	printf(":r (yes|no)       ~ enable/disable/show reverse-fuck mode.\n");
	printf(":set datum        ~ set datum as enviroment array string (and keep state (:ks yes))\n");
	printf(":tr datum         ~ translate datum string to brainfuck\n");
}
void execute(char *s,char *given_env,struct mret *ret,int *reversefuck)
{
	char *ns=NULL,*s2=NULL;
	if(!s) return;
	if((strcmp(s,":q"))==0) {quit_shell=1;}
	if((strcmp(s,":h"))==0) {show_help();}
	if((strcmp(s,":ks"))==0) {(ks?printf("ks=yes\n"):printf("ks=no\n"));}
	if((strcmp(s,":r"))==0) {(*reversefuck?printf("reverse-fuck=yes\n"):printf("reverse-fuck=no\n"));}
	if((strcmp(s,":p"))==0) {if(given_env) printf("%s\n",given_env);}
	if((strcmp(s,":z"))==0) {printf("%d\n",ret->ret);}
	if(*s==':'){
		//s2=strdup(s);
		s2=(char*)malloc(MAX_CMD+1);
		if(s2!=NULL){
			strncpy(s2,s,MAX_CMD-1);
			strtok_r(s2," ",&ns);
			if((strcmp(s2,":ks"))==0){
				if(ns){
					if((strcmp(ns,"yes"))==0) ks=1;
					if((strcmp(ns,"no"))==0) ks=0;
				}
			}
			if((strcmp(s2,":r"))==0){
				if(ns){
					if((strcmp(ns,"yes"))==0) *reversefuck=1;
					if((strcmp(ns,"no"))==0) *reversefuck=0;
				}
			}
			if((strcmp(s2,":set"))==0){
				if(ns){
					ks=1;
					given_env=strdup(ns);
				}
			}
			if((strcmp(s2,":tr"))==0){
				if(ns){
					printf("%s\n",tr_pretty_str(strdup(ns)));
				}
			}
			if((strcmp(s2,":l"))==0){if(ns){run(trim(ns),given_env,ret,*reversefuck);}}
			if(s2!=NULL) free(s2);
		}
	}
	brainfuck(s,given_env,ret,*reversefuck);
}
