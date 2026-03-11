/*Use semaphores to solve the readers-writers problem with writers being given priority
over readers. */

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define R 5
#define W 3

int readcount = 0;
sem_t mutex, wrt, rqueue;

void *reader(void *arg)
{
    while (1)
    {
        int id = *(int *)arg;

        sleep(rand() % 3 + 1);

        sem_wait(&rqueue);
        sem_wait(&mutex);

        readcount++;

        if (readcount == 1)
            sem_wait(&wrt);

        sem_post(&mutex);
        sem_post(&rqueue);

        printf("Reader %d is reading\n", id);
        sleep(2);
        printf("Reader %d finished reading\n", id);

        sem_wait(&mutex);

        readcount--;

        if (readcount == 0)
            sem_post(&wrt);

        sem_post(&mutex);
    }

    return NULL;
}

void *writer(void *arg)
{
    while (1)
    {
        int id = *(int *)arg;

        sleep(rand() % 5 + 2);

        sem_wait(&rqueue);
        sem_wait(&wrt);

        printf("Writer %d is writing\n", id);
        sleep(3);
        printf("Writer %d finished writing\n", id);

        sem_post(&wrt);
        sem_post(&rqueue);
    }

    return NULL;
}

int main()
{
    srand(time(NULL));

    pthread_t r[R], w[W];
    int rid[R], wid[W];

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    sem_init(&rqueue, 0, 1);

    for (int i = 0; i < R; i++)
    {
        rid[i] = i;
        pthread_create(&r[i], NULL, reader, &rid[i]);
    }

    for (int i = 0; i < W; i++)
    {
        wid[i] = i;
        pthread_create(&w[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < R; i++)
        pthread_join(r[i], NULL);

    for (int i = 0; i < W; i++)
        pthread_join(w[i], NULL);

    return 0;
}