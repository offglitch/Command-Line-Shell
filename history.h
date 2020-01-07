#ifndef _HISTORY_H_
#define _HISTORY_H_

#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define HIST_MAX 100
#define ARG_MAX 131072

struct history_entry {
    int cmd_id;
    double run_time;
    char* cmd;
};

struct history_entry histories[HIST_MAX];

void add_history(struct history_entry entry);
void get_cmd(char* input, char* cmd);
void print_history();

#endif
