// Martin Ganen-Villa
// U49246681
// This program implements a basic shell that contains the basic shell commands (cd, ls, path, etc.)

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>   // for fork(), execv()
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

char pathList[255][50]; // Array to globally contain the inputted paths
int numOfPaths = 0;     // Array to hold the number of inputted paths

// Error message
static void error(void) {
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

void path(char paths[][50], int n) {
    // Replace the current path list with the provided paths from index 1..n-1
    int i = 1;
    // Reset existing paths count before adding new ones
    numOfPaths = 0;
    while (i < n) {
        // copy each path string into the global pathList
        strcpy(pathList[i], paths[i]);
        numOfPaths++;
        i++;
    }
}

int cd(char dir[]) {
    // If it returns 0, there's an error
    if (chdir(dir) != 0) {
        return 0;
    }

    return 1;
}
int ls(char args[][50], int n) {
    printf("%d\n", n);
    // Run conditional if only ls is present
    if (n == 1) {
        int i = 1;
        for (i; i <= numOfPaths; i++) {
            char temp[50];
            strcpy(temp, pathList[i]);
            strcat(temp, "/ls"); // pathLists[0] = "/bin, after this line: /bin/ls/
            printf("===> %s\n", temp);
            
            // If files accessed successful
            if (access(temp, X_OK) == 0) {
                return 1;
            }
        }
        return 0;
    }
}

// Function to run external commaands
void execute_external(char args[][50], int n) {
    if (n <= 0) {
        // Return error if not enough arguments
        error();
        return;
    }

    int gt_pos = -1; // Position of ">" for output redirection
    int i = 0;

    // Scan arguments for ">"
    for (i = 0; i < n; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (gt_pos != -1) {
                // More than one ">" found → error
                error();
                return;
            }
            gt_pos = i;
        }
    }

    // Validate ">" usage: must be before the last argument (the filename)
    if (gt_pos != -1) {
        if(gt_pos == 0 || gt_pos != n-2) {
            // Invalid placement of ">"
            error();
            return;
        }
    }

    // Number of arguments to pass to exec (exclude ">" and filename if present)
    int main_args = n;
    if (gt_pos != -1) main_args = gt_pos;

    // If no command or no paths are defined, fail
    if (main_args <= 0 || numOfPaths <= 0) {
        error();
        return;
    }

    int found = 0;
    char fullpath[50];

    // Try each directory in pathList to see if command exists
    for (i = 1; i <= numOfPaths; i++) {
        strcpy(fullpath, pathList[i]);
        strcat(fullpath, "/");
        strcat(fullpath, args[0]);

        if (access(fullpath, X_OK) == 0) {
            // Found executable with execute permission
            found = 1;
            break;
        }
    }
    if (found == 0) {
        // Command not found in any path
        error();
        return;
    }

    // Build argument list for execv (NULL-terminated)
    char *args_to_execute[255];

    for(i = 0; i < main_args; i++) {
        args_to_execute[i] = strdup(args[i]);
    }
    args_to_execute[0] = strdup(fullpath);
    args_to_execute[main_args] = NULL;

    int outfd = -1; // File descriptor for output redirection
    if (gt_pos != -1) {
        // Open file for writing (truncate/create if needed)
        char *filename = args[gt_pos + 1];
        outfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfd < 0) {
            error();
            return;
        }
    }

    // Create child process
    pid_t pid = fork(); 
    
    // If fork failed, run
    if (pid < 0) {
        if (outfd != -1) {
            close(outfd);
        }
        error();
        return;
    }

    // If actively in the child process, run
    if (pid == 0) {
        // Child process: set up redirection if requested and then exec
        if (outfd != -1) {
            if (dup2(outfd, STDOUT_FILENO) < 0) {
                error();
                return;
            }
            close(outfd);
        }

        // Replace process with requested program
        execv(fullpath, args_to_execute);

        error();
        return;
    }
    if(outfd != -1) close(outfd);   // Close file descriptor (child inherited it)
    waitpid(pid, NULL, 0);          // Wait for child to finish    
}


void echo(char args[][50], int n) {
    int gt_pos = -1;                // index of ">" if present; -1 means none

    // Scan for a single ">" token
    for (int i = 1; i < n; i++) {
        if (strcmp(args[i], ">") == 0) {
            if (gt_pos != -1) { // found a second ">" → invalid
                error();
                return;
            }
            gt_pos = i;     // remember where ">" is
        }
    }

    // Validate redirection syntax if ">" was found:
    //  - there must be at least one word before ">" (i.e., some text to echo)
    //  - there must be exactly one filename after ">"
    if (gt_pos != -1) {
        if (gt_pos == 1 || gt_pos != n - 2) {
            error();
            return;
        }
    }

    // Number of tokens that belong to the message (exclude ">" and filename)
    int main_args = (gt_pos == -1) ? n : gt_pos;

    // Build the output line from args[1..main_args-1], separating by spaces
    char buffer[1024] = "";
    for (int i = 1; i < main_args; i++) {
        strcat(buffer, args[i]);
        if (i < main_args -1) strcat(buffer, " ");
    }

    if (gt_pos == -1) {
        // No redirection: print to stdout followed by newline (to match prompt layout)
        printf("%s\n", buffer);
        fflush(stdout);
    }

    else {
        // With redirection: open/create target file, truncate, and write the line + newline
        char *filename = args[gt_pos + 1];
        int outfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (outfd <  0) {
            error();
            return;
        }

        // Write message and newline; on any write error, emit generic shell error
        if (write(outfd, buffer, strlen(buffer)) < 0 || write(outfd, "\n", 1) < 0) {
            error();
            close(outfd);
            return;
        }
        close(outfd);
    }
}

int main(int argc, char* argv[]) {
    // Init input variables and sizes
    char* input;
    size_t len = 255;
    ssize_t nread;

    input = (char *)malloc(len * sizeof(char));

    // Run conditional if command line arguments are found
    if (argc > 1) {
        error();
        exit(0);
    }

    strcpy(pathList[1], "/bin"); // make the initial path /bin
    numOfPaths = 1;

    while (true) {
        // rush> constatnly prints
        printf("rush> ");
        fflush(stdout);
        nread = getline(&input, &len, stdin);   // Get input
        

        if (nread > 0 && input[nread-1] == '\n') input[nread-1] = '\0'; //Add null character to the end of the input
        
        char* word;
        char *p = input;
        char wordList[255][50]; //init wordList 2d array to contain inputs by list elements and character counts
        int i = 0;

        // Loop to separate the input and to pass them into wordList
        while ((word = strsep(&p, " ")) != NULL) {
            if (*word == '\0') continue;

            // Add words to array
            // copy token into wordList preserving null terminator
            strncpy(wordList[i], word, strlen(word));
            wordList[i][strlen(word)] = '\0';
            word = NULL;
            i++;
        }
        wordList[i][0] = '\0'; //set null character at end of array

        if (i == 0) {
            continue;
        }

        // Check for path  input
        if (strcmp(wordList[0], "path") == 0) {
            path(wordList, i);
        }

        // Check for exit input
        else if (strcmp(wordList[0], "exit") == 0) {
            if (i > 1) {
                // exit should not have arguments; print error on wrong usage
                error();
            }
            else {
                exit(0);
            }
        }

        // Check for cd input
        else if (strcmp(wordList[0], "cd") == 0) {
            if (i > 2 || i <= 1) {
                // cd requires exactly one argument
                error();
            }
            else {
                int success = cd(wordList[1]);
                if (success == 0) {
                    // chdir failed
                    error();
                }
            }
        }

        else if (strcmp(wordList[0], "echo") == 0) {
            echo(wordList, i);
        }

        // if any other input, check execute_external
        else {
            execute_external(wordList, i);
        }

        
    }
    
    // Deallocate memory
    free(input);

    return 0;

}