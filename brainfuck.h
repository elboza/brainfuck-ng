void brainfuck(char *v,char *given_env,struct mret *ret,int reversefuck);
char* find_next_paren(char *ip,int reversefuck);
char* find_prev_paren(char *ip,int reversefuck);
char reversefuck_translate(char c);
char* tr_char(char c);
char* tr_string(char *s);
char* tr_pretty_str(char *s);
