/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
// #include <editline/readline.h>
// #include <editline/history.h>
#include "main.h"
#ifdef HAVE_LIBREADLINE
#include<readline/readline.h>
#include<readline/history.h>
#endif
#include "repl.h"
#include "brainfuck.h"

// char* rl_gets(char *s){
// 	char* input = readline(s);
// 	add_history(input);
// 	return input;
// }
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
int repl(char *given_env)
{
	char *cmd,prompt[255];
	quit_shell=0;
	int line_no=1,ret;
	//if(check_funny()) funny_shell_disclaimer(); else normal_shell_disclaimer();
	//printf("entering shell-interactive mode...\n");
	#ifdef HAVE_LIBREADLINE
	rl_variable_bind("blink-match-paren","on");
	#endif
	while(!quit_shell)
	{
		#ifdef HAVE_LIBREADLINE
		sprintf(prompt,"bf~%d~ ",line_no++);
		cmd=rl_gets(prompt);
		#else
		cmd=(char*)malloc(MAX_CMD);
		fgets(cmd,MAX_CMD,stdin);
		#endif
		ret=execute(cmd,given_env);
		#ifndef HAVE_LIBREADLINE
		if(cmd) free(cmd);
		#endif
	}
	return ret;
}
void show_help(){
	printf("brainfuck interpreter, by Fernando Iazeolla 2015(c)\n");
	printf(":q                ~ exit interpreter\n");
	printf(":h                ~ this help\n");
	printf(":l file           ~ load file\n");
	printf(":env              ~ show interpreter variables\n");
	printf(":keepstate yes|no ~ keep state environment array after command enter.\n");
}
int execute(char *s,char *given_env)
{
	if(!s) return 0;
	if((strcmp(s,":q"))==0) {quit_shell=1;}
	if((strcmp(s,":h"))==0) show_help();
	else{return brainfuck(s,given_env);}
	return 0;
}

