#include<stdio.h>
#include<unistd.h>
#include<time.h>

void main(){
    time_t ts = time(NULL);
    printf("system time: %s",ctime(&ts));
    printf("PID %d\n",getpid());
    while(1){}
}


// cat /proc/<PID>/stat | awk -v ticks=$(getconf CLK_TCK) '{print "User: " $14/ticks "s, Kernel: " $15/ticks "s"}'
// the getconf CLK_TCK gets the systime 

