#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    int a, b, c; 
    pid_t pid;
    
    pipe(fd);
    
    printf("Enter value for a: "); scanf("%d", &a);
    printf("Enter value for b: "); scanf("%d", &b);
    printf("Enter value for c: "); scanf("%d", &c);

    pid = fork();
    // fd[0] is for Reading, fd[1] is for Writing

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    
    if (pid > 0) {
        int b_sqr, four_ac;     
        double result;   
        close(fd[1]); 
        
        // Parent evaluates b^2
        b_sqr = b * b;
        printf("Parent Calculated b^2: %d\n", b_sqr);
        // Read 4ac from Child
        read(fd[0], &four_ac, sizeof(four_ac));
        close(fd[0]);
        
        int prod = b_sqr * four_ac;
        
        if (prod < 0) {
            printf("Negative val \n");
        } else {
            result = sqrt(prod);
            printf("Final Result sqrt of %d * %d : %.2f\n", b_sqr, four_ac, result);
        }
        wait(NULL);
    } 
    else {
        int val;
        
        close(fd[0]);
        
        // Child evaluates 4ac
        val = 4 * a * c;
        printf("Child  Calculated 4ac: %d\n", val);
        
        // Send result to parent
        write(fd[1], &val, sizeof(val));
        
        close(fd[1]);
    }

    return 0;
}


// When fork() is called, the OS splits reality. Now you have two independent processes (Parent and Child) running at the exact same time.
//      The OS Scheduler decides who gets the CPU. It might run the Parent first, or the Child first—we don't know.

// If the Child runs first: It calculates 4ac, writes it to the pipe, and exits. The data sits in the pipe waiting.
//      When the Parent finally gets to read(), it sees the data, grabs it, and continues instantly.

// If the Parent runs first (Most Common): The Parent calculates b^2. Then it hits the read() line. The Parent looks into the pipe and sees it is empty.

// Because the pipe is empty, the OS pauses (blocks) the Parent process completely.
//      The Parent goes to "sleep." It cannot move to the next line of code. It is stuck at read().

//Since the Parent is asleep, the OS gives the CPU to the Child. The Child calculates 4ac and uses write() to put the value into the pipe.

//As soon as data hits the pipe, the OS sends a signal to the Parent: "Wake up! There is data now."

// The Parent wakes up, reads the value 4ac from the pipe, and only then proceeds to the final subtraction line.