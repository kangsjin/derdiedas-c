#define BUFFER_SIZE 1024
#define TEMP_FILE "terminal_output.tmp"

void clear_screen();
void handle_sigint(int sig);
void restore_terminal_output(const char *filename);
void save_terminal_output(const char *filename);