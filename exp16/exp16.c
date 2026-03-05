#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmp(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}
void sstf(int req[], int n, int head) {
    int total = 0, curr = head, visited[10] = {0};
    for(int i=0; i<n; i++) {
        int min = 10000, idx = -1;
        for(int j=0; j<n; j++) {
            if(!visited[j] && abs(req[j]-curr) < min) { min = abs(req[j]-curr); idx = j; }
        }
        visited[idx] = 1; total += min; curr = req[idx];
    }
    printf("SSTF Total Movement: %d\n", total);
}

void look(int req[], int n, int head) {
    int sorted[10], total = 0, curr = head;
    for(int i=0; i<n; i++) sorted[i] = req[i];
    qsort(sorted, n, sizeof(int), cmp);
    int i = 0; while(i < n && sorted[i] < head) i++;
    for(int j=i; j<n; j++) { total += abs(sorted[j]-curr); curr = sorted[j]; }
    for(int j=i-1; j>=0; j--) { total += abs(sorted[j]-curr); curr = sorted[j]; }
    printf("LOOK Total Movement: %d\n", total);
}

void cscan(int req[], int n, int head) {
    int sorted[10], total = 0, curr = head;
    for(int i=0; i<n; i++) sorted[i] = req[i];
    qsort(sorted, n, sizeof(int), cmp);
    int i = 0; while(i < n && sorted[i] < head) i++;
    for(int j=i; j<n; j++) { total += abs(sorted[j]-curr); curr = sorted[j]; }
    total += abs(4999 - curr); curr = 0;
    for(int j=0; j<i; j++) { total += abs(sorted[j]-curr); curr = sorted[j]; }
    printf("CSCAN Total Movement: %d\n", total);
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    int head = atoi(argv[1]), req[10];
    srand(time(NULL));
    printf("Requests: ");
    for(int i=0; i<10; i++) { req[i] = rand() % 5000; printf("%d ", req[i]); }
    printf("\n");
    sstf(req, 10, head);
    look(req, 10, head);
    cscan(req, 10, head);
    return 0;
}