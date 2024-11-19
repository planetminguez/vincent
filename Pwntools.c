#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BUFFER 1024

// Function to run an external process and interact with it
void interact_with_process(const char *program, const char *input) {
    int pipe_in[2], pipe_out[2];

    // Create pipes for communication
    if (pipe(pipe_in) == -1 || pipe(pipe_out) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process: run the target program
        // Redirect stdin, stdout to the pipes
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[0]);
        close(pipe_in[1]);
        close(pipe_out[0]);
        close(pipe_out[1]);

        execlp(program, program, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process: communicate with the child
        close(pipe_in[0]);

