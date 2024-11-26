#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LENGTH 1024
#define TEMP_COMPILED_PROGRAM "./temp_program.out"

// Function to run a shell command and display its output
void run_command(const char *command, const char *tool_name) {
    printf("Running %s analysis...\n", tool_name);
    FILE *fp = popen(command, "r");
    if (fp == NULL) {
        perror("Failed to run command");
        exit(1);
    }

    char output[4096];
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("%s", output);
    }
    pclose(fp);
    printf("\n");
}

// Function to compile the program for Valgrind analysis
void compile_program(const char *filename) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "gcc -g %s -o %s", filename, TEMP_COMPILED_PROGRAM);
    if (system(command) != 0) {
        fprintf(stderr, "Failed to compile the program. Ensure it compiles without errors.\n");
        exit(1);
    }
    printf("Program compiled successfully for Valgrind analysis.\n\n");
}

void perform_clang_analysis(const char *filename) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "clang --analyze %s", filename);
    run_command(command, "Clang Static Analyzer");
}

void perform_cppcheck_analysis(const char *filename) {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "cppcheck --enable=all --inconclusive --std=c99 %s", filename);
    run_command(command, "Cppcheck");
}

void perform_valgrind_analysis() {
    char command[MAX_COMMAND_LENGTH];
    snprintf(command, sizeof(command), "valgrind --leak-check=full --track-origins=yes ./%s", TEMP_COMPILED_PROGRAM);
    run_command(command, "Valgrind");
}

void cleanup() {
    remove(TEMP_COMPILED_PROGRAM);
    printf("Temporary files cleaned up.\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    printf("Starting advanced bug analysis on %s...\n\n", filename);

    // Step 1: Run Clang Static Analyzer
    perform_clang_analysis(filename);

    // Step 2: Run Cppcheck
    perform_cppcheck_analysis(filename);

    // Step 3: Compile and Run Valgrind
    compile_program(filename);
    perform_valgrind_analysis();

    // Cleanup
    cleanup();

    printf("Bug analysis completed.\n");
    return 0;
}
