#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
// #include <sys/wait.h>
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

int main(int argc, char* argv[]) {
    char input[255];

    if (argc > 1) {
        printf("An error has occurred");
        return false;
    }

    while (true) {
        printf("rush> ");
        scanf("%s", input);
        fflush(stdout);

        if (strcmp(input, "exit") == 0) {
            return false;
        }
    }

    return 0;

}


