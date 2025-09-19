#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
// #include <sys/wait.h>
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

char pathList[255][50];

static void error(void) {
    char message[] = "An error has occured";
    printf("%s", message);
    fflush(NULL);
}

void path(char paths[][50]) {
    int i = 1;
    while (paths[i][0] != '\0') {
        printf("This is path [%d] -> %s\n", i, paths[i]);
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
    if (n == 1) {
        
    }
}


int main(int argc, char* argv[]) {
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
        printf("%d\n", i);
        wordList[i][0] = '\0';

        // for (int j = 0; j < i; j++) {
        //     printf("Words -> %s\n", wordList[j]);
        // }

        if (strcmp(wordList[0], "path") == 0) {
            path(wordList);
        }

        if (strcmp(wordList[0], "exit") == 0) {
            if (i > 1) {
                printf("There's an error\n");
            }
            else {
                exit(0);
            }
        }

        if (strcmp(wordList[0], "ls") == 0) {
            printf("This is a ls\n");
        }

        if (strcmp(wordList[0], "cd") == 0) {
            if (i > 2 || i <= 1) {
                printf("Error\n");
            }
            else {
                int success = cd(wordList[1]);
                if (success == 0) {
                    printf("There is an Error\n");
                }
            }
        }

        
    }
    
    free(input);

    return 0;

}