#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define CACHE_SIZE 5
#define REQUESTS 40
#define MENU_ITEMS 20

/* Function to print generated requests */
void printRequests(int req[]) {
    printf("Menu Requests:\n");
    for (int i = 0; i < REQUESTS; i++)
        printf("%d ", req[i]);
    printf("\n\n");
}

/* LRU (Least Recently Used) Cache Algorithm */
void LRU(int req[]) {

    int cache[CACHE_SIZE];   // Cache storage
    int time[CACHE_SIZE];    // Tracks usage time

    int hits = 0, misses = 0;

    /* Initialize cache and time */
    for (int i = 0; i < CACHE_SIZE; i++) {
        cache[i] = -1;
        time[i] = 0;
    }

    int t = 0; // Global time counter

    /* Process each request */
    for (int i = 0; i < REQUESTS; i++) {

        int found = 0;

        /* Check if request is already in cache (HIT) */
        for (int j = 0; j < CACHE_SIZE; j++) {
            if (cache[j] == req[i]) {
                hits++;
                t++;
                time[j] = t;   // Update recent usage time
                found = 1;
                break;
            }
        }

        /* If not found → MISS */
        if (!found) {

            misses++;

            /* Find Least Recently Used (LRU) index */
            int lru = 0;
            for (int j = 1; j < CACHE_SIZE; j++) {
                if (time[j] < time[lru])
                    lru = j;
            }

            /* Replace LRU item */
            cache[lru] = req[i];
            t++;
            time[lru] = t;
        }
    }

    /* Print results */
    printf("LRU Results\n");
    printf("Hits   : %d\n", hits);
    printf("Misses : %d\n\n", misses);
}

/* FIFO (First In First Out) Cache Algorithm */
void FIFO(int req[]) {

    int cache[CACHE_SIZE];   // Cache storage
    int hits = 0, misses = 0;

    /* Initialize cache */
    for (int i = 0; i < CACHE_SIZE; i++)
        cache[i] = -1;

    int front = 0; // Points to next replacement position

    /* Process each request */
    for (int i = 0; i < REQUESTS; i++) {

        int found = 0;

        /* Check for HIT */
        for (int j = 0; j < CACHE_SIZE; j++) {
            if (cache[j] == req[i]) {
                hits++;
                found = 1;
                break;
            }
        }

        /* If MISS → Replace oldest entry */
        if (!found) {

            misses++;

            cache[front] = req[i];
            front = (front + 1) % CACHE_SIZE; // Circular update
        }
    }

    /* Print results */
    printf("FIFO Results\n");
    printf("Hits   : %d\n", hits);
    printf("Misses : %d\n\n", misses);
}

/* Main Function */
int main() {

    int requests[REQUESTS];

    /* Seed random number generator */
    srand(time(0));

    /* Generate random requests (1 to MENU_ITEMS) */
    for (int i = 0; i < REQUESTS; i++)
        requests[i] = rand() % MENU_ITEMS + 1;

    /* Display requests */
    printRequests(requests);

    /* Apply cache algorithms */
    LRU(requests);
    FIFO(requests);

    return 0;
}