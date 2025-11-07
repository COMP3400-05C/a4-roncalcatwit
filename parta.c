#include <stdio.h>
#include <ctype.h>

/**
 * Main program that behaves like a simplified echo:
 * - Reads command-line arguments (argv)
 * - Prints all arguments in uppercase, separated by a single comma (no spaces)
 *
 * @param argc Number of command-line arguments (including program name)
 * @param argv Array of argument strings; argv[0] is program name
 * @return 0 on success; 1 if no arguments were provided (argc <= 1)
 */
int main(int argc, const char* argv[]) {
    // If no user-provided arguments, print error and return 1
    if (argc <= 1) {
        printf("ERROR: No arguments");
        return 1;
    }

    // Iterate over each user argument (skip argv[0])
    for (int i = 1; i < argc; i++) {
        const char *s = argv[i];
        // Print each character in uppercase
        for (size_t j = 0; s[j] != '\0'; j++) {
            // cast to unsigned char to avoid undefined behavior for negative chars
            putchar((char) toupper((unsigned char) s[j]));
        }
        // Print comma between arguments, but not after the last one
        if (i < argc - 1) {
            putchar(',');
        }
    }

    // No trailing newline to match expected test output
    return 0;
}