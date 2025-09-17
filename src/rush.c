#define _GNU_SOURCE
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

// char arguments(char* line, char*** argv_out) {

// }

int main(int argc, char* argv[]) {
    // char input[255];
    char* input;
    size_t len = 255;
    ssize_t nread;

    input = (char *)malloc(len * sizeof(char));

    if (argc > 1) {
        error();
        exit(0);
    }
    
    while (true) {
        printf("rush> ");
        fflush(stdout);
        nread = getline(&input, &len, stdin);
        
        if (strcmp(input, "exit\n") == 0) {
            exit(0);
        }

        if (nread > 0 && input[nread-1] == '\n') input[nread-1] = '\0';
        
        char* word;
        char *p = input;
        char wordList[255][50];
        int i = 0;
        while ((word = strsep(&p, " ")) != NULL) {
            if (*word == '\0') continue;

            // Add words to array
            strncpy(wordList[i], word, sizeof(wordList[i] - 1));
            wordList[i][sizeof(wordList[i] - 1)] = '\0';
            i++;
        }
        
    }
    
    free(input);

    return 0;

}