#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 3
#define NUM_CONSUMERS 4
#define TOTAL_ORDERS 30

/* Structure to represent an order */
typedef struct {
    int order_id;
    int taker_id;
} Order;

/* Shared circular buffer */
Order buffer[BUFFER_SIZE];

int head = 0;   // Index for removing items
int tail = 0;   // Index for adding items
int count = 0;  // Current number of items in buffer

/* Global counters */
int orders_placed = 0;
int orders_cooked = 0;
int next_order_id = 1;

/* Semaphores */
sem_t empty;  // Tracks empty slots
sem_t full;   // Tracks filled slots
sem_t mutex;  // Ensures mutual exclusion

/* Function to print buffer status */
void print_buffer_state(const char *actor, const char *action)
{
    printf("%s: %s | Buffer [%d/%d]\n",
           actor, action, count, BUFFER_SIZE);
}

/* Producer Thread: Order Taker */
void *order_taker(void *arg)
{
    int id = *(int *)arg;
    free(arg);  // Free allocated memory for thread ID

    while (1) {

        /* Check if all orders are placed */
        sem_wait(&mutex);
        if (orders_placed >= TOTAL_ORDERS) {
            sem_post(&mutex);
            break;
        }

        /* Generate a new order */
        int my_order = next_order_id++;
        orders_placed++;
        sem_post(&mutex);

        /* Simulate time to take order */
        usleep((rand() % 300 + 100) * 1000);

        /* Wait for an empty slot in buffer */
        sem_wait(&empty);

        /* Enter critical section */
        sem_wait(&mutex);

        /* Add order to buffer */
        buffer[tail].order_id = my_order;
        buffer[tail].taker_id = id;
        tail = (tail + 1) % BUFFER_SIZE;
        count++;

        print_buffer_state("OrderTaker", "Added Order");

        /* Exit critical section */
        sem_post(&mutex);

        /* Signal that a new item is available */
        sem_post(&full);
    }

    printf("OrderTaker %d exiting\n", id);
    return NULL;
}

/* Consumer Thread: Chef */
void *chef(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    while (1) {

        /* Wait for available item */
        sem_wait(&full);

        /* Enter critical section */
        sem_wait(&mutex);

        /* Check if all orders are cooked */
        if (orders_cooked >= TOTAL_ORDERS) {
            sem_post(&mutex);
            sem_post(&full);  // Wake other waiting threads
            break;
        }

        /* Remove order from buffer */
        Order o = buffer[head];
        head = (head + 1) % BUFFER_SIZE;
        count--;

        orders_cooked++;

        print_buffer_state("Chef", "Cooking Order");

        /* Exit critical section */
        sem_post(&mutex);

        /* Signal that a slot is free */
        sem_post(&empty);

        /* Simulate cooking time */
        usleep((rand() % 500 + 200) * 1000);
    }

    printf("Chef %d exiting\n", id);
    return NULL;
}

/* Main Function */
int main()
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];

    /* Initialize semaphores */
    sem_init(&empty, 0, BUFFER_SIZE);  // Initially all slots are empty
    sem_init(&full, 0, 0);             // No items initially
    sem_init(&mutex, 0, 1);            // Binary semaphore

    /* Create producer threads */
    for (int i = 0; i < NUM_PRODUCERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&producers[i], NULL, order_taker, id);
    }

    /* Create consumer threads */
    for (int i = 0; i < NUM_CONSUMERS; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&consumers[i], NULL, chef, id);
    }

    /* Wait for all producers to finish */
    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    /* Wake up consumers so they can exit */
    for (int i = 0; i < NUM_CONSUMERS; i++)
        sem_post(&full);

    /* Wait for all consumers to finish */
    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    /* Destroy semaphores */
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    /* Final output */
    printf("\nTotal Orders Placed: %d\n", orders_placed);
    printf("Total Orders Cooked: %d\n", orders_cooked);

    return 0;
}