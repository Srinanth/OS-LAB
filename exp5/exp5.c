#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid= fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        printf("CHILD (PID %d).\n", getpid());

        char *args[] = {"./add", "10", "20", NULL};
        execvp(args[0], args);

        perror("execvp failed");
        exit(1);
    }
    else {
        printf("PARENT waits for PID %d.\n", pid);
        
        wait(NULL);
        
        printf("Child finished execution.\n");
    }

    return 0;
}