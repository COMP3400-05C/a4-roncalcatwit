#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>  // for execv
#include <stdio.h>   // for perror
#include <string.h>  // for string operations

/**
 * @brief Program that uses exec to echo the middle argument(s)
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, 1 on error
 */
int main(int argc, const char* argv[]) {
    // Check if there are no arguments (argc == 1 means only program name)
    if (argc == 1) {
        write(STDERR_FILENO, "ERROR: No arguments\n", 19);
        return 1;
    }

    // Calculate the middle position(s)
    int num_args = argc - 1;  // Don't count program name
    int mid1, mid2;
    
    if (num_args % 2 == 0) {  // Even number of arguments
        mid1 = num_args / 2;
        mid2 = mid1 + 1;
    } else {  // Odd number of arguments
        mid1 = (num_args + 1) / 2;
        mid2 = -1;  // Not used for odd number of arguments
    }

    // Prepare arguments for execv
    char* eargs[4];  // Maximum of 4 elements: "echo", arg1, arg2, NULL
    eargs[0] = "echo";
    eargs[1] = (char*)argv[mid1];  // First (or only) middle argument
    
    if (mid2 != -1) {
        eargs[2] = (char*)argv[mid2];  // Second middle argument
        eargs[3] = NULL;
    } else {
        eargs[2] = NULL;
    }

    // Execute echo command
    int eret = execv("/bin/echo", eargs);
    
    // If execv returns, it means there was an error
    if (eret == -1) {
        perror("exec");
        return 1;
    }

    return 0;
}
