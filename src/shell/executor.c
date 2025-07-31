#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"

void executeCommand(SimpleCommand *cmd) {
    cmd->args[cmd->argc] = NULL;  // null-terminate for execvp
    if (cmd->argc == 0) return;

    if (strcmp(cmd->args[0], "exit") == 0) {
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0) {
        execvp(cmd->args[0], cmd->args);
        perror("execvp");
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
}
