%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

extern int yylex(void);
void yyerror(const char *s) {
    fprintf(stderr, "Errore: %s\n", s);
}
%}

%code requires {
    #include "command.h"
}

%union {
    char *string;
    SimpleCommand *scmd;
}

%token <string> WORD
%token PIPE
%token NEWLINE

%type <scmd> command

%%

input:
      /* vuoto */
    | input line
    ;

line:
      command NEWLINE {
          executeCommand($1);
          freeCommand($1);
      }
    | NEWLINE
    ;

command:
      WORD {
          $$ = createSimpleCommand($1);
      }
    | command WORD {
          addArgument($1, $2);
          $$ = $1;
      }
    ;

%%
