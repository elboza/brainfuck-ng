int repl(char *given_env);
int quit_shell;
#define MAX_CMD 1024
char* rl_gets (char *prompt);
int execute(char *s,char *given_env);