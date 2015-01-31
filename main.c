/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include "main.h"
#include "brainfuck.h"
#include "repl.h"

struct m_action{
	int file;
	int shell;
	int stdin;
	int data;
};
void log_d(char *s){
	if(!s) return;
	if(!DEBUG) return;
	printf("%s\n",s);
}
void die(char *s){
	printf("%s\n",s);
	exit(1);
}
void usage()
{
	printf("brainfuck-ng v%s\n",VERSION);
	printf("\nUSAGE: ");
	printf("bfng [options] [file]\n");
	printf("valid options:\n");
	printf("-i              --shell --interactive        interactive (shell mode)\n");
	printf("-h              --help                       show this help\n");
	printf("-v              --version                    prints brainfuck-ng version number\n");
	printf("-s              --stdin                      read fron stdin\n");
	printf("-d              --data                       set environment-array\n");
	exit(1);
}
void usage_b()
{
	printf("brainfuck-ng v%s (c) Fernando Iazeolla \n",VERSION);
	printf("for help type: bfng --help\n");
}
char* parse_args(int argc,char **argv,struct m_action *action)
{
	int c;
	action->shell=0;
	action->file=0;
	action->stdin=0;
	action->data=0;
	char *given_env=NULL;
	while (1)
	{
		static struct option long_options[] =
		{
			{"interactive",no_argument,0,'i'},
			{"shell",no_argument,0,'i'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{"stdin",no_argument,0,'s'},
			{"data",required_argument,0,'d'},
			{0,0,0,0,}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "vhisd:",long_options, &option_index);
		if (c == -1) break;
		switch(c)
		{
			case 'i':
				action->shell=1;
				break;
			case 'v':
				usage_b();
				break;
			case 's':
				action->stdin=1;
				break;
			case 'd':
				action->data=1;
				given_env=optarg;
				break;
			case 'h':
			case '?':
				usage();
				break;
			default:
				usage_b();
				break;
		}
		
	}
	return given_env;
}
int run (char *filename,char *given_env){
	FILE *fp;
	long len;
	int ret;
	char *v,dd[FILENAME_LEN];
	if((fp=fopen(filename,"r"))==0) die("error opening file.");
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	sprintf(dd,"%s %ld",filename,len);
	log_d(dd);
	v=(char*)malloc(len);
	fread(v,1,len,fp);
	fclose(fp);
	ret=brainfuck(v,given_env);
	free(v);
	return ret;
}
int shell(char *given_env){
	int ret;
	printf("entering shell-interactive mode...(type :h for help)\n");
	ret=repl(given_env);
	printf("Bye.\n");
	return ret;
}
int runstdin(char *given_env){
	int ret;
	char *prog=(char*)malloc(1024);
	if(!prog) die("error alloc memory");
	read(0,prog,1024);
	ret=brainfuck(prog,given_env);
	free(prog);
	return ret;
}
int main(int argc,char **argv)
{
	struct m_action action;
	char filename[FILENAME_LEN],*given_env;
	int ret=0;
	given_env=parse_args(argc,argv,&action);
	
	if(argc<2) usage_b();
	if(optind<argc) {strncpy(filename,argv[optind],FILENAME_LEN);action.file=1;}
	else{strncpy(filename,"<NULL>",FILENAME_LEN);action.file=0;}
	if(action.stdin+action.file+action.shell>1){
		printf("argument error. see bfng -h\n");
		action.stdin=0;
		action.shell=0;
		action.file=0;
	}
	if(action.stdin){
		ret=runstdin(given_env);
	}
	if(action.file)
	{
		ret=run(filename,given_env);
	}
	if(action.shell)
	{
		ret=shell(given_env);
	}
	
	log_d("Bye.");
	return ret;
}
