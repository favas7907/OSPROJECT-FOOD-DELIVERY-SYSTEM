#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Mutex locks representing shared resources */
pthread_mutex_t order;
pthread_mutex_t vehicle;
pthread_mutex_t gps;
pthread_mutex_t bag;

/*
    Thread DP1:
    Holds Order → waits for Vehicle
*/
void* DP1(void* arg)
{
    pthread_mutex_lock(&order);
    printf("DP1 holds Order\n");

    sleep(1);

    printf("DP1 waiting for Vehicle\n");
    pthread_mutex_lock(&vehicle);

    printf("DP1 got Vehicle\n");

    pthread_mutex_unlock(&vehicle);
    pthread_mutex_unlock(&order);

    return NULL;
}

/*
    Thread DP2:
    Holds Vehicle → waits for GPS
*/
void* DP2(void* arg)
{
    pthread_mutex_lock(&vehicle);
    printf("DP2 holds Vehicle\n");

    sleep(1);

    printf("DP2 waiting for GPS\n");
    pthread_mutex_lock(&gps);

    printf("DP2 got GPS\n");

    pthread_mutex_unlock(&gps);
    pthread_mutex_unlock(&vehicle);

    return NULL;
}

/*
    Thread DP3:
    Holds GPS → waits for Bag
*/
void* DP3(void* arg)
{
    pthread_mutex_lock(&gps);
    printf("DP3 holds GPS\n");

    sleep(1);

    printf("DP3 waiting for Delivery Bag\n");
    pthread_mutex_lock(&bag);

    printf("DP3 got Bag\n");

    pthread_mutex_unlock(&bag);
    pthread_mutex_unlock(&gps);

    return NULL;
}

/*
    Thread DP4:
    Holds Bag → waits for Order
*/
void* DP4(void* arg)
{
    pthread_mutex_lock(&bag);
    printf("DP4 holds Delivery Bag\n");

    sleep(1);

    printf("DP4 waiting for Order\n");
    pthread_mutex_lock(&order);

    printf("DP4 got Order\n");

    pthread_mutex_unlock(&order);
    pthread_mutex_unlock(&bag);

    return NULL;
}

int main()
{
    pthread_t t1, t2, t3, t4;

    /* Initialize mutexes */
    pthread_mutex_init(&order, NULL);
    pthread_mutex_init(&vehicle, NULL);
    pthread_mutex_init(&gps, NULL);
    pthread_mutex_init(&bag, NULL);

    /* Create threads */
    pthread_create(&t1, NULL, DP1, NULL);
    pthread_create(&t2, NULL, DP2, NULL);
    pthread_create(&t3, NULL, DP3, NULL);
    pthread_create(&t4, NULL, DP4, NULL);

    /* Give time for deadlock to occur */
    sleep(5);

    /* Deadlock message */
    printf("\nSystem is in Deadlock (All threads are waiting indefinitely)\n");

    return 0;
}