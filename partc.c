/**
 * @brief Program that uses exec to echo the middle argument(s)
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return 0 on success, 1 on error
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Keep main signature unchanged as required. */
int main(int argc, const char* argv[]) {
    // Check for required arguments: program, word, filename => argc == 3
    if (argc != 3) {
        // Match the expected test output exactly 
        printf("ERROR: No arguments\n");
        return 1;
    }

    const char *word = argv[1];
    const char *filename = argv[2];

    // Fork a child to run /usr/bin/grep 
    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed 
        perror("fork");
        return 2;
    }

    if (pid == 0) {
        /*child: prepare arguments for grep and exec it.
           Use -s to suppress error messages and -q to suppress output,
           so we only rely on grep's exit status. */ 
        char *const grep_args[] = {"/usr/bin/grep", "-s", "-q", (char *)word, (char *)filename, NULL};

        /* execv replaces the child process image with grep.
           If execv returns, it failed; exit with a distinct code. */
        execv("/usr/bin/grep", grep_args);

        // If execv fails, write a brief message to stderr and _exit.
        // Use _exit to avoid flushing parent's stdio buffers twice. 
        _exit(127);
    } else {
        // Parent: wait for child to finish and inspect exit status 
        int status = 0;
        pid_t w = waitpid(pid, &status, 0);
        if (w == -1) {
            perror("waitpid");
            return 2;
        }

        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code == 0) {
                // grep found the pattern
                printf("FOUND: %s\n", word);
                return 0;
            } else if (code == 1) {
                // grep did not find the pattern 
                printf("NOT FOUND: %s\n", word);
                return 0; // per instructions, report NOT FOUND but exit 0 
            } else if (code == 2) {
                // grep encountered an error, e.g., file doesn't exist
                printf("ERROR: %s doesn't exist\n", filename);
                return 2;
            } else {
                // Unexpected exit code from grep
                printf("ERROR: grep failed with code %d\n", code);
                return 2;
            }
        } else if (WIFSIGNALED(status)) {
            // Child terminated by a signal 
            printf("ERROR: grep terminated by signal %d\n", WTERMSIG(status));
            return 2;
        } else {
            // Other unexpected conditions 
            printf("ERROR: unexpected child status\n");
            return 2;
        }
    }
}
