#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

/* Mutex locks */
pthread_mutex_t order;
pthread_mutex_t vehicle;
pthread_mutex_t gps;
pthread_mutex_t bag;

/* ================= DEADLOCK PART ================= */

/* DP1: Order → Vehicle */
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

/* DP2: Vehicle → GPS */
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

/* DP3: GPS → Bag */
void* DP3(void* arg)
{
    pthread_mutex_lock(&gps);
    printf("DP3 holds GPS\n");
    sleep(1);

    printf("DP3 waiting for Bag\n");
    pthread_mutex_lock(&bag);

    printf("DP3 got Bag\n");

    pthread_mutex_unlock(&bag);
    pthread_mutex_unlock(&gps);
    return NULL;
}

/* DP4: Bag → Order */
void* DP4(void* arg)
{
    pthread_mutex_lock(&bag);
    printf("DP4 holds Bag\n");
    sleep(1);

    printf("DP4 waiting for Order\n");
    pthread_mutex_lock(&order);

    printf("DP4 got Order\n");

    pthread_mutex_unlock(&order);
    pthread_mutex_unlock(&bag);
    return NULL;
}

/* ================= DEADLOCK-FREE PART ================= */

typedef struct {
    int id;
    bool use_order;
    bool use_vehicle;
    bool use_gps;
    bool use_bag;
} Delivery;

void* safe_delivery(void* arg)
{
    Delivery* d = (Delivery*)arg;

    /* Fixed order locking */
    if (d->use_order)
        pthread_mutex_lock(&order);

    if (d->use_vehicle)
        pthread_mutex_lock(&vehicle);

    if (d->use_gps)
        pthread_mutex_lock(&gps);

    if (d->use_bag)
        pthread_mutex_lock(&bag);

    printf("Delivery Partner %d completed delivery\n", d->id);

    /* Reverse unlocking */
    if (d->use_bag)
        pthread_mutex_unlock(&bag);

    if (d->use_gps)
        pthread_mutex_unlock(&gps);

    if (d->use_vehicle)
        pthread_mutex_unlock(&vehicle);

    if (d->use_order)
        pthread_mutex_unlock(&order);

    return NULL;
}

/* ================= MAIN FUNCTION ================= */

int main()
{
    int choice;

    /* Initialize mutexes */
    pthread_mutex_init(&order, NULL);
    pthread_mutex_init(&vehicle, NULL);
    pthread_mutex_init(&gps, NULL);
    pthread_mutex_init(&bag, NULL);

    printf("1. Demonstrate Deadlock\n");
    printf("2. Demonstrate Deadlock-Free Solution\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        pthread_t t1, t2, t3, t4;

        pthread_create(&t1, NULL, DP1, NULL);
        pthread_create(&t2, NULL, DP2, NULL);
        pthread_create(&t3, NULL, DP3, NULL);
        pthread_create(&t4, NULL, DP4, NULL);

        sleep(5);

        printf("\nSystem is in Deadlock (All threads are waiting indefinitely)\n");
    }
    else if (choice == 2)
    {
        pthread_t t[4];
        Delivery d[4];

        /* Input */
        for (int i = 0; i < 4; i++) {
            d[i].id = i + 1;
            int o, v, g, b;

            printf("\nEnter resources for DP%d (1/0):\n", i + 1);
            printf("Order: "); scanf("%d", &o);
            printf("Vehicle: "); scanf("%d", &v);
            printf("GPS: "); scanf("%d", &g);
            printf("Bag: "); scanf("%d", &b);

            d[i].use_order = o;
            d[i].use_vehicle = v;
            d[i].use_gps = g;
            d[i].use_bag = b;
        }

        /* Create threads */
        for (int i = 0; i < 4; i++)
            pthread_create(&t[i], NULL, safe_delivery, &d[i]);

        /* Join */
        for (int i = 0; i < 4; i++)
            pthread_join(t[i], NULL);

        printf("\nNo Deadlock: All deliveries completed successfully\n");
    }

    return 0;
}
