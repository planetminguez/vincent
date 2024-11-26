#include <clang-c/Index.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print diagnostics from LibClang
void print_diagnostics(CXTranslationUnit translation_unit) {
    int num_diagnostics = clang_getNumDiagnostics(translation_unit);
    for (int i = 0; i < num_diagnostics; ++i) {
        CXDiagnostic diagnostic = clang_getDiagnostic(translation_unit, i);
        CXString message = clang_formatDiagnostic(diagnostic, clang_defaultDiagnosticDisplayOptions());
        fprintf(stderr, "%s\n", clang_getCString(message));
        clang_disposeString(message);
        clang_disposeDiagnostic(diagnostic);
    }
}

// Visitor function for traversing AST
enum CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (!clang_Location_isFromMainFile(location)) {
        return CXChildVisit_Continue;
    }

    CXCursorKind kind = clang_getCursorKind(cursor);
    CXString cursor_spelling = clang_getCursorSpelling(cursor);
    CXString type_spelling = clang_getTypeSpelling(clang_getCursorType(cursor));
    unsigned int line, column;

    clang_getSpellingLocation(location, NULL, &line, &column, NULL);

    if (kind == CXCursor_VarDecl) {
        printf("Variable Declaration: '%s' of type '%s' at line %u, column %u\n",
               clang_getCString(cursor_spelling),
               clang_getCString(type_spelling),
               line, column);
        // Check if the variable is uninitialized
        if (!clang_Cursor_isNull(clang_Cursor_getDefinition(cursor))) {
            printf("  Warning: Variable '%s' may be uninitialized at declaration.\n", clang_getCString(cursor_spelling));
        }
    }

    clang_disposeString(cursor_spelling);
    clang_disposeString(type_spelling);

    return CXChildVisit_Recurse;
}

// Analyze the C file using LibClang
void analyze_with_libclang(const char *filename) {
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit translation_unit = clang_parseTranslationUnit(
        index, filename, NULL, 0, NULL, 0, CXTranslationUnit_None);

    if (!translation_unit) {
        fprintf(stderr, "Error: Unable to parse the translation unit.\n");
        clang_disposeIndex(index);
        return;
    }

    print_diagnostics(translation_unit);

    CXCursor root_cursor = clang_getTranslationUnitCursor(translation_unit);
    clang_visitChildren(root_cursor, visitor, NULL);

    clang_disposeTranslationUnit(translation_unit);
    clang_disposeIndex(index);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file-to-analyze.c>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    printf("Analyzing file: %s\n", filename);
    analyze_with_libclang(filename);
    return 0;
}
