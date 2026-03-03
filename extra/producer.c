#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5

// buffer
int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

// semaphores
sem_t mutex;
sem_t empty;
sem_t full;

// ---------------- PRODUCER ----------------
void *producer(void *arg)
{
    int item,val;

    while (1)
    {
        // produce item
        item = rand() % 10;
         sem_getvalue(&mutex,&val);
        // wait if buffer is full	
        if(val == 0)
	printf("producer waiting\n");
       	sem_wait(&empty);
        

        // lock critical section
        sem_wait(&mutex);
        printf("producer producing\n");
	sleep(4);
        // add item to buffer
        buffer[in] = item;
        printf("Producer produced: %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
	
        // unlock
        sem_post(&mutex);

        // increase filled slots
        sem_post(&full);

        sleep(1);
    }
}

// ---------------- CONSUMER ----------------
void *consumer(void *arg)
{
    int item,val;

    while (1)
    {
    
    	sem_getvalue(&mutex,&val);
        // wait if buffer is full	
        if(val == 0)
    		printf("consumer waiting\n");
    		
        sem_wait(&full);
	
        // lock critical section
        sem_wait(&mutex);
	printf("consumer consuming\n");
	sleep(4);
        // remove item
        item = buffer[out];
        printf("Consumer consumed: %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
	
        // unlock
        sem_post(&mutex);

        // increase empty slots
        sem_post(&empty);

        sleep(3);
    }
}

// ---------------- MAIN ----------------
int main()
{
    pthread_t p, c;

    // initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    // create threads
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    // wait for threads
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    // destroy semaphores
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}