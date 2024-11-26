#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Function to check for uninitialized variables
void check_uninitialized_variables(const char *line, int line_number) {
    if (strstr(line, "int ") || strstr(line, "float ") || strstr(line, "char ")) {
        if (!strchr(line, '=') && strchr(line, ';')) {
            printf("Warning: Possible uninitialized variable on line %d: %s\n", line_number, line);
        }
    }
}

// Function to check for unused variables
void check_unused_variables(const char *line, const char *filename, int line_number) {
    // Naive approach: Check for variable declarations without usage (requires improvement)
    char variable[MAX_LINE_LENGTH];
    if (sscanf(line, " int %s ;", variable) == 1) {
        printf("Note: Variable '%s' declared on line %d may be unused.\n", variable, line_number);
    }
}

// Main function to analyze a file
void analyze_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        check_uninitialized_variables(line, line_number);
        check_unused_variables(line, filename, line_number);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file-to-analyze.c>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    printf("Analyzing file: %s\n", filename);
    analyze_file(filename);
    return 0;
}
