#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

#define MAX 100

struct msg_buffer {
    long msg_type;
    char msg_text[MAX];
};

void reverse_string(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char temp = str[i];
        str[i] = str[n - i - 1];
        str[n - i - 1] = temp;
    }
}

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;
    pid_t pid;

    key = ftok("demn", 65);  //ftok creates unique key for for the queue , this is only for naming it btw so we have to create  queue later.
    msgid = msgget(key, 0666 | IPC_CREAT); 
    // msgget sys calls to the kernel and IPC_CREAT is a flag to check whether we can create a queue or not,
    //  if queue doesnt exist the ipc_creat allows the fn to create the queue.

    pid = fork();

    if (pid > 0) {
        printf("Enter a string: ");
        fgets(message.msg_text, MAX, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = 0; 
        
        char original[MAX];
        strcpy(original, message.msg_text);
        // there are types while sending mesgs, for child its 1 for parent its 2.

        // Send to child using the type 1
        message.msg_type = 1;
        msgsnd(msgid, &message, sizeof(message), 0); // the 0 is a flag tells to block(sleep) until the operation can be compleeted
        // and the msgsnd blocks only if there are problems like when message queue is full 

        // here the parent waits till child responds , the type is 2 
        msgrcv(msgid, &message, sizeof(message), 2, 0);

        printf("Received reversed string: %s\n", message.msg_text);

        // Compare
        if (strcmp(original, message.msg_text) == 0)
            printf("Result: It is a Palindrome\n");
        else
            printf("Result: Not a Palindrome\n");

        msgctl(msgid, IPC_RMID, NULL); // this fn is used to control the queue. and the ipc rmid removes the queue
        wait(NULL);
    } 
    else {
        // Receive message of Type 1
        msgrcv(msgid, &message, sizeof(message), 1, 0);
        
        reverse_string(message.msg_text);
        
        // Send back as Type 2
        message.msg_type = 2;
        msgsnd(msgid, &message, sizeof(message), 0);
        exit(0);
    }
    return 0;
}