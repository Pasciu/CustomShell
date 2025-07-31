/*Questa opzione avvisa flex che bisogna lavorare su un singolo file.*/
%option noyywrap

%{
#include "parser.tab.h"	// Contiene le definizioni dei Token (PIPE, WORD, ...)
#include <string.h>
#include <stdlib.h>
%}

%%
[ \t]+				; // ignora uno o più spazi
\n              	{	return NEWLINE;	}
"|"                	{ 	return PIPE; }
[^ \t\n|]+         	{ 	yylval.string = strdup(yytext); 
						return WORD;
					}
%%
