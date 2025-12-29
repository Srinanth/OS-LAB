#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

struct shared_data {
    char str1[50];
    char str2[50];
    char str3[50];
    char result[200];
    int status;
     // 0: Empty, 1: Data Ready, 2: Result Ready
};
// just to flip lower to upper and viseversa
void flip_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (islower(str[i])) str[i] = toupper(str[i]);
        else if (isupper(str[i])) str[i] = tolower(str[i]);
    }
}

int main() {
    key_t key = ftok("shmfile", 65); //for the key
    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT); //for creating the shared  memory
    
    // Maps shared memory into process’s address space
    struct shared_data *shm = (struct shared_data*) shmat(shmid, (void*)0, 0);
     //here the(),0 says to attach to any memory instead of writing like shmat(shmid, (void*)0x50000000, 0)
    
    shm->status = 0; // Initialize status
    
    if (fork() > 0) {
        printf("Enter String 1: "); scanf("%s", shm->str1);
        printf("Enter String 2: "); scanf("%s", shm->str2);
        printf("Enter String 3: "); scanf("%s", shm->str3);
        
        shm->status = 1; // Signal Process 2
        
        // this waits till status is 2 in child
        while (shm->status != 2) sleep(1);
        
        flip_case(shm->result);
        printf("Final Output: %s\n", shm->result);
        
        // Detach and Remove
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        wait(NULL);
    } 
    else {
        // waits till status is 1
        while (shm->status != 1) sleep(1);
        
        // Concatenate
        strcpy(shm->result, shm->str1);
        strcat(shm->result, " ");
        strcat(shm->result, shm->str2);
        strcat(shm->result, " ");
        strcat(shm->result, shm->str3);
        
        shm->status = 2; // Signal Process 1
        
        shmdt(shm);
        exit(0);
    }
    return 0;
}