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
	printf("brainfuck v%s\n",VERSION);
	printf("\nUSAGE: ");
	printf("bf [options] [file]\n");
	printf("valid options:\n");
	printf("-i              --shell --interactive        interactive (shell mode)\n");
	printf("-h              --help                       show this help\n");
	printf("-v              --version                    prints ylang version number\n");
	exit(1);
}
void usage_b()
{
	printf("brainfuck-%s (c) Fernando Iazeolla \n",VERSION);
	printf("for help type: bf --help\n");
}
void parse_args(int argc,char **argv,struct m_action *action)
{
	int c;
	action->shell=0;
	action->file=0;
	while (1)
	{
		static struct option long_options[] =
		{
			{"interactive",no_argument,0,'i'},
			{"shell",no_argument,0,'i'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{0,0,0,0,}
			
		};
		int option_index = 0;
		c = getopt_long (argc, argv, "vhi",long_options, &option_index);
		if (c == -1) break;
		switch(c)
		{
			case 'i':
				action->shell=1;
				break;
			case 'v':
				usage_b();
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
}
void run (char *filename){
	FILE *fp;
	long len;
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
	brainfuck(v);
	free(v);
}
void shell(){
	printf("entering shell-interactive mode...(type -h for help)\n");
	repl();
	printf("Bye.\n");
}
int main(int argc,char **argv)
{
	struct m_action action;
	char filename[FILENAME_LEN];
	parse_args(argc,argv,&action);
	
	if(argc<2) usage_b();
	if(optind<argc) {strncpy(filename,argv[optind],FILENAME_LEN);action.file=1;}
	else{strncpy(filename,"<NULL>",FILENAME_LEN);action.file=0;}
	if(action.file)
	{
		run(filename);
	}
	if(action.shell)
	{
		shell();
	}
	
	log_d("Bye.");
	return 0;
}
