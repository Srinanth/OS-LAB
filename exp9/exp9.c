#include <stdio.h>

#define MAX 20
#define INF 99999
#define Q 3

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;

    int start;
    int completion;
    int turnaround;
    int waiting;
    int response;
} Process;

//To print, common fn for all

void printTable(Process p[], int n, float *avgWT) {
    float sumWT = 0, sumTAT = 0, sumRT = 0, sumCT = 0;

    printf("\nPID\tAT\tBT\tST\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++) {
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
        p[i].response = p[i].start - p[i].arrival;

        sumWT += p[i].waiting;
        sumTAT += p[i].turnaround;
        sumRT += p[i].response;
        sumCT += p[i].completion;

        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid,
               p[i].arrival,
               p[i].burst,
               p[i].start,
               p[i].completion,
               p[i].turnaround,
               p[i].waiting,
               p[i].response);
    }

    *avgWT = sumWT / n;

    printf("Average Waiting Time     = %.2f\n", sumWT / n);
    printf("Average Turnaround Time  = %.2f\n", sumTAT / n);
    printf("Average Response Time    = %.2f\n", sumRT / n);
    printf("Average Completion Time  = %.2f\n", sumCT / n);
}

//FCFS

float fcfs(Process p[], int n) {
    int time = 0;
    float avgWT;

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;

        p[i].start = time;
        time += p[i].burst;
        p[i].completion = time;
    }

    printf("\n=== FCFS ===");
    printTable(p, n, &avgWT);
    return avgWT;
}

//SRTF

float srtf(Process p[], int n) {
    int time = 0, completed = 0;
    float avgWT;
    int started[MAX] = {0};

    while (completed < n) {
        int idx = -1, min = INF;

        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 &&
                p[i].remaining < min) {
                min = p[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        if (!started[idx]) {
            p[idx].start = time;
            started[idx] = 1;
        }

        p[idx].remaining--;
        time++;

        if (p[idx].remaining == 0) {
            p[idx].completion = time;
            completed++;
        }
    }

    printf("\n=== SRTF ===");
    printTable(p, n, &avgWT);
    return avgWT;
}

//Proiority

float priority_np(Process p[], int n) {
    int time = 0, completed = 0;
    int done[MAX] = {0};
    float avgWT;

    while (completed < n) {
        int idx = -1, maxPr = -INF;

        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time &&
                p[i].priority > maxPr) {
                maxPr = p[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].start = time;
        time += p[idx].burst;
        p[idx].completion = time;

        done[idx] = 1;
        completed++;
    }

    printf("\n=== PRIORITY (NON-PREEMPTIVE) ===");
    printTable(p, n, &avgWT);
    return avgWT;
}

// ROund robin

float roundRobin(Process p[], int n) {
    int time = 0, completed = 0;
    int queue[MAX], front = 0, rear = 0;
    int visited[MAX] = {0};
    int started[MAX] = {0};
    float avgWT;

    for (int i = 0; i < n; i++) {
        if (p[i].arrival == 0) {
            queue[rear++] = i;
            visited[i] = 1;
        }
    }

    while (completed < n) {
        if (front == rear) {
            time++;
            for (int i = 0; i < n; i++) {
                if (!visited[i] && p[i].arrival <= time) {
                    queue[rear++] = i;
                    visited[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];

        if (!started[idx]) {
            p[idx].start = time;
            started[idx] = 1;
        }

        int exec = (p[idx].remaining > Q) ? Q : p[idx].remaining;
        p[idx].remaining -= exec;
        time += exec;

        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival <= time) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }

        if (p[idx].remaining > 0)
            queue[rear++] = idx;
        else {
            p[idx].completion = time;
            completed++;
        }
    }

    printf("\n=== ROUND ROBIN (Q = 3) ===");
    printTable(p, n, &avgWT);
    return avgWT;
}


int main() {
    int n;
    Process p[MAX], temp[MAX];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nProcess P%d\n", p[i].pid);
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        printf("Priority: ");
        scanf("%d", &p[i].priority);
        p[i].remaining = p[i].burst;
    }

    float wt_fcfs, wt_srtf, wt_prio, wt_rr;

    for (int i = 0; i < n; i++) temp[i] = p[i];
    wt_fcfs = fcfs(temp, n);

    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].remaining = p[i].burst;
    }
    wt_srtf = srtf(temp, n);

    for (int i = 0; i < n; i++) temp[i] = p[i];
    wt_prio = priority_np(temp, n);

    for (int i = 0; i < n; i++) {
        temp[i] = p[i];
        temp[i].remaining = p[i].burst;
    }
    wt_rr = roundRobin(temp, n);

    printf("\n=== FINAL COMPARISON ===\n");
    printf("FCFS WT = %.2f\n", wt_fcfs);
    printf("SRTF WT = %.2f\n", wt_srtf);
    printf("Priority WT = %.2f\n", wt_prio);
    printf("Round Robin WT = %.2f\n", wt_rr);

    float min = wt_fcfs;
    char best[] = "FCFS";

    if (wt_srtf < min) { min = wt_srtf; sprintf(best, "SRTF"); }
    if (wt_prio < min) { min = wt_prio; sprintf(best, "Priority"); }
    if (wt_rr < min) { min = wt_rr; sprintf(best, "Round Robin"); }

    printf("\nAlgorithm with minimum average waiting time: %s\n", best);

    return 0;
}
