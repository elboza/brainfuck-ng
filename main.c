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
	int print_env;
	int cin;
	int exec;
	int xfile;
	int dfile;
	int reversefuck;
	int tr_string;
	int tr_stdin;
	int tr_file;
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
void reset_actions(struct m_action *action){
	action->shell=0;
	action->file=0;
	action->stdin=0;
	action->data=0;
	action->print_env=0;
	action->cin=0;
	action->exec=0;
	action->xfile=0;
	action->dfile=0;
	action->reversefuck=0;
	action->tr_string=0;
	action->tr_stdin=0;
	action->tr_file=0;
}
void usage()
{
	printf("brainfuck-ng v%s\n",VERSION);
	printf("\nUSAGE: ");
	printf("bfng [options] [file]\n");
	printf("valid options:\n");
	printf("-i              --shell         interactive (shell mode)\n");
	printf("-h              --help          show this help\n");
	printf("-v              --version       prints brainfuck-ng version number\n");
	printf("-s              --stdin         read bf prog from stdin\n");
	printf("-d 'datum'      --data          set environment-array\n");
	printf("-o              --out           print env-array to stdout after computation\n");
	printf("-p              --print         same as -o but with newline added.\n");
	printf("-r              --reversefuck   switch 'ReverseFuck' mode on.\n");
	printf("-m size         --size          set the size (in bytes) of environment array.\n");
	printf("-x 'prog'       --exec          gets & execute bf prog\n");
	printf("-c              --cin           gets environment-array from stdin\n");
	printf("-b  file        --xfile         gets bf prog from file\n");
	printf("-a  file        --dfile         gets environment-array from file\n");
	printf("-t  'datum'     --tr            translate 'datum' string to brainfuck\n");
	printf("-S              --ts            translate stdin string to brainfuck\n");
	printf("-F  file        --tf            translate file to brainfuck\n");
	exit(1);
}
void usage_b()
{
	printf("brainfuck-ng v%s (c) Fernando Iazeolla \n",VERSION);
	printf("for help type: bfng --help\n");
}
void parse_args(int argc,char **argv,struct m_action *action,struct datas *dt)
{
	int c;
	reset_actions(action);
	//char *given_env=NULL;
	ks=0;
	while (1)
	{
		static struct option long_options[] =
		{
			{"exec",required_argument,0,'x'},
			{"shell",no_argument,0,'i'},
			{"help",no_argument,0,'h'},
			{"version",no_argument,0,'v'},
			{"stdin",no_argument,0,'s'},
			{"out",no_argument,0,'o'},
			{"print",no_argument,0,'p'},
			{"data",required_argument,0,'d'},
			{"cin",no_argument,0,'c'},
			{"xfile",required_argument,0,'b'},
			{"dfile",required_argument,0,'a'},
			{"reversefuck",no_argument,0,'r'},
			{"size",required_argument,0,'m'},
			{"tr",required_argument,0,'t'},
			{"ts",no_argument,0,'S'},
			{"tf",required_argument,0,'F'},
			{0,0,0,0,}

		};
		int option_index = 0;
		c = getopt_long (argc, argv, "vhisd:opx:cb:a:rm:t:SF:",long_options, &option_index);
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
				dt->given_env=optarg;
				ks=1;
				break;
			case 'o':
				action->print_env=1;
				break;
			case 'p':
				action->print_env=2;
				break;
			case 'r':
				action->reversefuck=1;
				break;
			case 'm':
				requested_size=atoi(optarg);
				break;
			case 'x':
				action->exec=1;
				dt->prog=optarg;
				break;
			case 'c':
				action->cin=1;
				ks=1;
				break;
			case 'a':
				action->dfile=1;
				dt->dfile=optarg;
				ks=1;
				break;
			case 'b':
				action->xfile=1;
				dt->xfile=optarg;
				break;
			case 't':
				action->tr_string=1;
				dt->prog=optarg;
				break;
			case 'S':
				action->tr_stdin=1;
				//dt->prog=optarg;
				break;
			case 'F':
				action->tr_file=1;
				dt->dfile=optarg;
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
	//return given_env;
}
void run (char *filename,char *given_env,struct mret *ret,int reversefuck){
	FILE *fp;
	long len;
	char *v,dd[FILENAME_LEN];
	if((fp=fopen(filename,"r"))==0) {printf("error opening file.\n");ret->ret=1;return;}
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	sprintf(dd,"%s %ld",filename,len);
	log_d(dd);
	v=(char*)calloc(len+1,sizeof(char));
	fread(v,1,len,fp);
	fclose(fp);
	brainfuck(v,given_env,ret,reversefuck);
	free(v);
}
void shell(char *given_env,struct mret *ret,int reversefuck){
	printf("entering shell-interactive mode...(type :h for help)\n");
	repl(given_env,ret,reversefuck);
	printf("Bye.\n");
}
void runstdin(char *given_env,struct mret *ret,int reversefuck){
	char *prog=(char*)calloc(STDIN_LEN,sizeof(char));
	if(!prog) die("error alloc memory");
	read(0,prog,STDIN_LEN);
	brainfuck(prog,given_env,ret,reversefuck);
	free(prog);
}
char* get_dfile (char *filename){
	FILE *fp;
	long len;
	char *v,dd[FILENAME_LEN];
	if((fp=fopen(filename,"r"))==0) {printf("error opening file.\n");return NULL;}
	fseek(fp,0L,SEEK_END);
	len=ftell(fp);
	fseek(fp,0L,SEEK_SET);
	sprintf(dd,"%s %ld",filename,len);
	log_d(dd);
	v=(char*)calloc(len+1,sizeof(char));
	fread(v,1,len,fp);
	fclose(fp);
	return v;
}
char* get_cin(){
	char *in=(char*)calloc(STDIN_LEN,sizeof(char));
	if(!in) die("error alloc memory");
	read(0,in,STDIN_LEN);
	return in;
}
void do_tr_string(char *str){
	printf("%s\n",tr_pretty_str(str));
}
void do_tr_stdin(){
	char *x;
	x=get_cin();
	printf("%s\n",tr_pretty_str(x));
}
void do_tr_file(char *filename){
	char *x;
	x=get_dfile(filename);
	printf("%s\n",tr_pretty_str(x));
}
int main(int argc,char **argv)
{
	struct m_action action;
	char filename[FILENAME_LEN];
	//char *given_env=NULL;
	requested_size=0;
	struct mret ret;
	ret.a=NULL;
	ret.ret=0;
	struct datas d;
	d.given_env=NULL;
	d.prog=NULL;
	d.dfile=NULL;
	d.xfile=NULL;
	parse_args(argc,argv,&action,&d);

	if(argc<2) usage_b();
	if(optind<argc) {strncpy(filename,argv[optind],FILENAME_LEN);action.file=1;}
	else{strncpy(filename,"<NULL>",FILENAME_LEN);action.file=0;}
	if(action.stdin+action.file+action.shell+action.xfile+action.tr_string+action.tr_stdin+action.tr_file>1){
		printf("argument incongruence input prog. see bfng -h\n");
		reset_actions(&action);
	}
	if(action.data+action.dfile+action.cin>1){
		printf("argument incongruence input env data. see bfng -h\n");
		reset_actions(&action);
	}
	if(action.cin){
		d.given_env=get_cin();
	}
	if(action.dfile){
		d.given_env=get_dfile(d.dfile);
	}
	if(action.xfile){
		run(d.xfile,d.given_env,&ret,action.reversefuck);
	}
	if(action.exec){
		brainfuck(d.prog,d.given_env,&ret,action.reversefuck);
	}
	if(action.stdin){
		runstdin(d.given_env,&ret,action.reversefuck);
	}
	if(action.file)
	{
		run(filename,d.given_env,&ret,action.reversefuck);
	}
	if(action.shell)
	{
		shell(d.given_env,&ret,action.reversefuck);
	}
	if(action.tr_string){
		do_tr_string(d.prog);
	}
	if(action.tr_stdin){
		do_tr_stdin();
	}
	if(action.tr_file){
		do_tr_file(d.dfile);
	}

	log_d("Bye.");
	if(action.print_env==1) printf("%s",ret.a);
	if(action.print_env==2) printf("%s\n",ret.a);
	if(ret.a && !d.given_env) free(ret.a);
	return ret.ret;
}
