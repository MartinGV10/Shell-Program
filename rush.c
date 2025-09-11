#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
// #include <sys/wait.h>
#include <string.h>     // for strdup()
#include <fcntl.h>      // for open() flags like O_CREAT, O_WRONLY

int main(int argc, char* argv[]) {
    // char input[255];
    char* input = NULL;
    size_t len = 0;
    ssize_t nread;

    // char* copy = strdup(input);
    // char* text[]; 
    // char* p = copy;
    // char* tok;

    // const char* regex = "\"((?:\\\\.|[^\"\\\\])*)\"|'((?:\\\\.|[^'\\\\])*)'|(\\|\\||&&|>>|2>|>|<|\\||&)|((?:\\\\.|[^\\s\"'\\\\])+)";

    if (argc > 1) {
        printf("An error has occurred");
        return false;
    }

    while (true) {
        printf("rush> ");
        fflush(stdout);
        nread = getline(&input, &len, stdin);

        if (nread > 0 && input[nread - 1] == '\n') {
            input[nread-1] = '\0';
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



// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h> // For strdup

// int main() {
//     char *str = strdup("one,two, 3,four"); // Duplicate string as strsep modifies it
//     char *token;
//     char *head = str; // Keep a pointer to the original start for freeing

//     while ((token = strsep(&str, ",")) != NULL) {
//         printf("Token: \"%s\"\n", token);
//     }

//     free(head); // Free the duplicated string
//     return 0;
// }