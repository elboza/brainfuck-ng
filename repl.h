void repl(char *given_env,struct mret *ret,int reversefuck);
int quit_shell;
#define MAX_CMD 1024
char* rl_gets (char *prompt);
void execute(char *s,char *given_env,struct mret *ret,int *reversefuck);
int ks;