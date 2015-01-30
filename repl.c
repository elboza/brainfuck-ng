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
void repl()
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
		#ifdef HAVE_LIBREADLINE
		sprintf(prompt,"bf~%d~ ",line_no++);
		cmd=rl_gets(prompt);
		#else
		cmd=(char*)malloc(MAX_CMD);
		fgets(cmd,MAX_CMD,stdin);
		#endif
		execute(cmd);
		#ifndef HAVE_LIBREADLINE
		if(cmd) free(cmd);
		#endif
	}
	
}
void show_help(){
	printf("brainfuck interpreter, by Fernando Iazeolla 2015(c)\n");
	printf(":q        exit interpreter\n");
	printf(":h        this help\n");
	printf(":l file   load file\n");
	printf(":env      show interpreter variables\n");
}
void execute(char *s)
{
	if(!s) return;
	if((strcmp(s,":q"))==0) {quit_shell=1;}
	if((strcmp(s,":h"))==0) show_help();
	else{brainfuck(s);}
	
}

