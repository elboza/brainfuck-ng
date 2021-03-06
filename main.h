/*
 * author: Fernando Iazeolla
 * license: GPLv2
 */

#ifndef MAIN_H
#define MAIN_H

//#define DEBUG 1
#define DEBUG 0
#define MAX_DIM 1024*30
#define STDIN_LEN 1024*30
#define HAVE_LIBREADLINE
//#undef HAVE_LIBREADLINE
#define FILENAME_LEN 255
#define VERSION "0.2.1"

void log_d(char *s);
void die(char *s);
struct mret{
	int ret;
	char *a;
	int size;
};
struct datas{
	char *given_env;
	char *prog;
	char *xfile;
	char *dfile;
};
void run (char *filename,char *given_env,struct mret *ret,int reversefuck);
int requested_size;

#endif
