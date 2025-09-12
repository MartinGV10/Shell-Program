#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
// #include <sys/wait.h>
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

static void error(void) {
    char message[] = "An error has occured";
    printf("%s", message);
    fflush(NULL);
}

int main(int argc, char* argv[]) {
    // char input[255];
    const char* env = getenv("PATH");
    char* input = NULL;
    size_t len = 0;
    ssize_t nread;

    if (argc > 1) {
        error();
        exit(0);
    }
    
    while (true) {
        printf("rush> ");
        fflush(stdout);
        nread = getline(&input, &len, stdin);
        
        if (nread > 0 && input[nread - 1] == '\n') {
            input[nread-1] = '\0';
        }
        
        char* work = strdup(input);

        if (strcmp(input, "cd") == 0) {
            chdir(input);
        }


        if (nread == -1) {
            break;
        }

        if (input[0] == '\0') {
            continue;
        }

        if (strcmp(input, "exit") == 0) {
            exit(0);
        }
    }

    free(input);

    return 0;

}