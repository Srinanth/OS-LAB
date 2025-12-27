#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid;

    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        printf("PCCSL407 ");
    }
    else {
        wait(NULL);
        printf("Operating Systems Lab\n");
    }

    return 0;
}

//without wait(NULL) the "os lab" prints first 