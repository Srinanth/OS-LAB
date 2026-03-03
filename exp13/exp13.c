#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5   // Number of philosophers

sem_t forks[N];  // Semaphore for each fork

void* philosopher(void* arg) {
    int id = (int)arg;
    int left = id;
    int right = (id + 1) % N;

    while (1) {
        // Thinking
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        // Pick up forks asymmetrically
        if (id % 2 == 0) {
            // Even philosopher: pick right first
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
        } else {
            // Odd philosopher: pick left first
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
        }

        // Eating
        printf("Philosopher %d is eating\n", id);
        sleep(2);

        // Put down forks
        sem_post(&forks[left]);
        sem_post(&forks[right]);
    }
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    // Initialize forks
    for (int i = 0; i < N; i++)
        sem_init(&forks[i], 0, 1);

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Join threads (never ends)
    for (int i = 0; i < N; i++)
        pthread_join(philosophers[i], NULL);

    return 0;
}