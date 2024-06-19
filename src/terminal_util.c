
#include "../include/terminal_util.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void save_terminal_output(const char *filename) {
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "tput smcup; clear > %s", filename);
    system(command);
}

void restore_terminal_output(const char *filename) {
    char command[BUFFER_SIZE];
    snprintf(command, sizeof(command), "tput rmcup < %s", filename);
    system(command);
}

void clear_screen() {
    printf("\033[H\033[J");
}

void handle_sigint(int sig) {
    restore_terminal_output(TEMP_FILE);
    remove(TEMP_FILE);
    exit(0);
}
