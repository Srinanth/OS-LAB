#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int is_hit(int frames[], int n, int page) {
    for (int i = 0; i < n; i++) if (frames[i] == page) return 1;
    return 0;
}

int get_optimal_index(int ref[], int n, int frames[], int frame_count, int current) {
    int res = -1, farthest = current;
    for (int i = 0; i < frame_count; i++) {
        int j;
        for (j = current; j < n; j++) {
            if (frames[i] == ref[j]) {
                if (j > farthest) { farthest = j; res = i; }
                break;
            }
        }
        if (j == n) return i;
    }
    return (res == -1) ? 0 : res;
}

void simulate(int ref[], int n, int frame_count) {
    int frames[7], faults;

    faults = 0; int fifo_ptr = 0;
    for(int i=0; i<frame_count; i++) frames[i] = -1;
    for(int i=0; i<n; i++) {
        if (!is_hit(frames, frame_count, ref[i])) {
            frames[fifo_ptr] = ref[i];
            fifo_ptr = (fifo_ptr + 1) % frame_count;
            faults++;
        }
    }
    printf("FIFO Faults: %d\n", faults);

    faults = 0; int time[7] = {0};
    for(int i=0; i<frame_count; i++) frames[i] = -1;
    for(int i=0; i<n; i++) {
        if (!is_hit(frames, frame_count, ref[i])) {
            int lru = 0;
            for(int j=1; j<frame_count; j++) if(time[j] < time[lru]) lru = j;
            frames[lru] = ref[i];
            time[lru] = i;
            faults++;
        } else {
            for(int j=0; j<frame_count; j++) if(frames[j] == ref[i]) time[j] = i;
        }
    }
    printf("LRU Faults: %d\n", faults);

    faults = 0;
    for(int i=0; i<frame_count; i++) frames[i] = -1;
    for(int i=0; i<n; i++) {
        if (!is_hit(frames, frame_count, ref[i])) {
            int idx = get_optimal_index(ref, n, frames, frame_count, i + 1);
            frames[idx] = ref[i];
            faults++;
        }
    }
    printf("Optimal Faults: %d\n", faults);
}

int main(int argc, char *argv[]) {
    if (argc < 3) return 1;
    int len = atoi(argv[1]), frames = atoi(argv[2]);
    int *ref = malloc(len * sizeof(int));
    srand(time(NULL));
    printf("Ref String: ");
    for(int i=0; i<len; i++) { ref[i] = rand() % 10; printf("%d ", ref[i]); }
    printf("\n");
    simulate(ref, len, frames);
    return 0;
}