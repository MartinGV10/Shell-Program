#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
// #include <sys/wait.h>
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

int main(int argc, char* argv[]) {
    // int rc = fork();
    // printf("rush> ");
    char input[50];

    while (true) {
        printf("rush> ");
        scanf("%s", input);
        if (strcmp(input, "exit") == 0) {
            return false;
        }
    }



    // for (int i = 1; i < argc; i++) {

    //     if (strcmp(argv[i], "exit") == 0) {
    //         return 0;
    //     }

    //     if (argc > 1) {
    //         for (int i = 1; i < argc; i++) {
    //             printf("rush> %s\n", argv[i]);
    //         }
    //     }
    // }
    return 0;

}


