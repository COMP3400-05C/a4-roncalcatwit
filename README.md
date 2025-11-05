[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Ss-46pr7)
# Process System Call Assignment

## Introduction

In this assignment, you will practice working with process system calls.

To get started, first clone from GitHub. You may need to "close" the existing folder first.

### Build Instructions

To build this project run the `make` command in the terminal. You must run this *every time you change a file*.

### Commit Instructions

After completing Part A, first add the file called `parta.c` to staging:

    git add parta.c

Then do the actual commit:

    git commit -m 'Completed Part A'

When you are ready to submit the assignment, run the following command:

    git push

## Instructions

### Background on Command-line Arguments

As an example, let's run the `echo` command. This command reads your command-line arguments and
prints them on the screen. For example:

    $ echo a
    a
    $ echo Hello
    Hello
    $ echo Hello World
    Hello World
    $ echo Hello   Brand    New  World
    Hello Brand New World

As can be seen above, any arguments to the `echo` command will be printed on the screen. Arguments
separated by spaces are passed individually, but when printed are separated by spaces.

### Task to Do

For this assignment, you are to write a program that acts like echo, but also capitalizes all
alphabet letters and prints it out. All of the arguments should be printed (separated by a single
comma).

For example:

    $ ./parta Hello
    HELLO

    $ ./parta Hello World
    HELLO,WORLD

If there are no arguments provided, print an error message and return 1 from main.

    $ ./parta
    ERROR: No arguments
    $ echo $?
    1

The second command (`echo $?`) checks if the main return value (aka the program exit status) is 1.

Once you are confident that the program runs, you can run the tests by:

    bats tests/parta.bats

## Part B

For this assignment, you are to write a program that uses the exec system call use the `echo`
program. This program will read N arguments, and use echo to print the middle 1 or 2 arguments. Do
NOT use the `printf` function or other stdio functions (unless for debugging purposes).

### Background on using exec()

The exec() system can be a bit tricky to use. There are multiple (!) different versions of this
system call, and the version we'll be using is `execv`. This means it takes a variable number of
arguments as an array (notice the "v").

Below is a C program that runs the `ls` command with the argument "-l":

    char* eargs[] = { "ls", "-l", NULL };
    int eret = execv("/bin/ls", eargs);
    if(eret = -1) {
        perror("exec");
        return 1;
    }

What the first line does is create an array of arguments (BTW, this is the same array that the main
function will receive). The 0th element is the name of the program ("ls"). Any command-line
arguments to be included are added after this, and lastly the array is terminated with `NULL`.

The second line does the actual system call. The first argument is the full path to the executable
file. Path indicates which directory the file is located and the name of the file.

Next, we check if the return value of `exec` was -1. If it is, there was an error, so we use a
special version of `puts` called `perror` which will *also* print the reason for the system call
failing.

> [!NOTE]
> The return value of exec is *not* the same as the exit status of the `ls` program. To get
> the exit status of `ls`, you need to use the `wait` system call in the parent (discussed next).

### Your Task

First check the number of arguments, and if none print an error message.

Then identify the middle argument if there are an odd number of arguments, or the middle 2 arguments
if there are an even number of arguments.

Last, use the exec system call to exec the command. Make sure you handle the `-1` return because
you may have typos in your arguments.

For example:

    $ ./partb ONE
    ONE

    $ ./partb ONE TWO THREE
    TWO

    $ ./partb ONE TWO THREE FOUR
    TWO THREE

Like Part A, if there are no arguments, print an error message and return 1 from main.

    $ ./partb
    ERROR: no arguments
    $ echo $?
    1

Once you are confident that the program runs, you can run the tests by:

    bats tests/partb.bats

## Part C

For this assignment, you are to write a program that uses the *fork-exec* flow. The parent
process will wait for the child process, which runs the program `grep` to complete. If the child
process is successful, print FOUND, if not, print an error message.

### Background of the grep command

In this part, you will use the `grep` command. This command searches for patterns (called "regular
expressions") in a file. If the pattern is found, `grep` program exists with status 0. If not, it
exists with a non-zero exit status.

For example,

    $ grep -s fox tests/file1.txt

Will exit with status 0 because the file contains the word `fox`.

On the other hand,

    $ grep -s deer tests/file1.txt

Will exit with status 1 because the word `deer` does not exist in the file.

### Background on using wait()

The wait system call allows a parent process to *wait* for the child process to exit. The child
process can produce an exit status in two ways: the `main` function returns an int value, or calls
the `exit` system call with the value.

THe parent process can collect the said value by using the `wait` system call.

### The Task

Write a program that will take a command line argument, which is the word to search for. First check
if you have two arguments. If not, then print a help message and exit with status 1 (return 1 from
main).

Once you have all the arguments, fork a child process and run the `/usr/bin/grep` command from the
child process. The argument to the `grep` program should be "-s" to make errors quiet, "-q" to make
the output also quiet, the word you want to search for, and the name of the file to search in. So
there are a total of 4 arguments (5 if you include the `NULL`).

If the word is found, the parent process will get an exit status of 0 and print that the word has
been found. If not, the parent process should print what the error is. View grep's manpage to find
out what exit status' exist. If the error indicated that the word can't be found, print "NOT FOUND"
with your own exit status of 0. If the error indicated that the file couldn't be found, print "ERROR" and exit
with status 2.

For example:

    $ ./partc fox tests/file1.txt
    FOUND: fox

    $ ./partc deer tests/file1.txt
    NOT FOUND: deer

    $ ./partc deer tests/no_such_file.txt
    ERROR: tests/no_such_file.txt doesn't exist
    $ echo $?
    2

Like Part A, if there are no arguments, return 1 from main.

    $ ./partc
    ERROR: no arguments
    $ echo $?
    1

The "tests" directory contains input files.

[!NOTE]
> There are two sets of command line arguments: your own, and grep's. You need to take your own
> command line arguments and create a new array of grep's command line arguments.
>
> There are also two sets of exist status: *your own exit status*, and the "grep" command's exit
> status.
>
> If you have extra output, like below, this means you didn't use the "-q" argument. Add it to
> your args list.

    $ ./partc deer tests/file1.txt
    The quick brown fox jumps over the lazy dog
    NOT FOUND: deer

Once you are confident that the program runs, you can run the tests by:

    bats tests/partc.bats

