#include <stdio.h>
#include "gtkApp.h"

extern int yyparse();

void init_shell_gui(int argc, char **argv) {
	create_app(argc, argv);
}

int main(int argc, char **argv) {
    init_shell_gui(argc, argv);
    while (1) {		//Questa porzione andrà rediretta sulla GUI
        printf("myshell> ");
        fflush(stdout);
        yyparse();
    } 
    return 0;
}
