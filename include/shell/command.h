#ifndef COMMAND_H
#define COMMAND_H

typedef struct {
    int argc;
    int capacity;
    char **args;
} SimpleCommand;

SimpleCommand *createSimpleCommand(char *arg);
void addArgument(SimpleCommand *cmd, char *arg);
void executeCommand(SimpleCommand *cmd);
void freeCommand(SimpleCommand *cmd);

#endif
