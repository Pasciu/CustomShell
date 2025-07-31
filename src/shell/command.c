#include "command.h"
#include <stdlib.h>
#include <string.h>

SimpleCommand *createSimpleCommand(char *arg) {
    SimpleCommand *cmd = malloc(sizeof(SimpleCommand));
    cmd->argc = 0;
    cmd->capacity = 4;
    cmd->args = malloc(sizeof(char*) * cmd->capacity);
    addArgument(cmd, arg);
    return cmd;
}

void addArgument(SimpleCommand *cmd, char *arg) {
    if (cmd->argc >= cmd->capacity) {
        cmd->capacity *= 2;
        cmd->args = realloc(cmd->args, sizeof(char*) * cmd->capacity);
    }
    cmd->args[cmd->argc++] = arg;
}

void freeCommand(SimpleCommand *cmd) {
    for (int i = 0; i < cmd->argc; ++i) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd);
}
