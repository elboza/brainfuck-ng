void repl(void);
int quit_shell;
#define MAX_CMD 1024
char* rl_gets (char *prompt);
void execute(char *s);