#include <stdio.h>
#include <pthread.h>

/* Mutex locks representing shared resources */
pthread_mutex_t order;
pthread_mutex_t vehicle;
pthread_mutex_t gps;
pthread_mutex_t bag;

/*
    Thread Function: delivery
    Each delivery partner acquires all resources
    in the SAME ORDER to avoid deadlock
*/
void* delivery(void* arg)
{
    int id = *(int*)arg;

    /* Acquire resources in fixed order */
    pthread_mutex_lock(&order);
    pthread_mutex_lock(&vehicle);
    pthread_mutex_lock(&gps);
    pthread_mutex_lock(&bag);

    /* Critical Section */
    printf("Delivery Partner %d completed delivery\n", id);

    /* Release resources in reverse order */
    pthread_mutex_unlock(&bag);
    pthread_mutex_unlock(&gps);
    pthread_mutex_unlock(&vehicle);
    pthread_mutex_unlock(&order);

    return NULL;
}

int main()
{
    pthread_t t[4];
    int id[4] = {1, 2, 3, 4};

    /* Initialize mutexes */
    pthread_mutex_init(&order, NULL);
    pthread_mutex_init(&vehicle, NULL);
    pthread_mutex_init(&gps, NULL);
    pthread_mutex_init(&bag, NULL);

    /* Create threads */
    for (int i = 0; i < 4; i++)
        pthread_create(&t[i], NULL, delivery, &id[i]);

    /* Wait for all threads to finish */
    for (int i = 0; i < 4; i++)
        pthread_join(t[i], NULL);

    return 0;
}