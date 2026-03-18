#include <stdio.h>

#define MAX 25
#define Q1_QUANTUM 2
#define Q2_QUANTUM 4
#define Q3_QUANTUM 6
#define AGING_LIMIT 6

// Structure to represent each order (process)
struct Order {
    int id;
    int arrivalTime;
    int prepTime;
    int remainingTime;
    int queueLevel;
    int waitingTime;
    int completionTime;
};

/*
    Function: execute
    Purpose : Executes orders in a given queue level using Round Robin scheduling
*/
void execute(struct Order orders[], int n, int level, int quantum, int *time) {

    for (int i = 0; i < n; i++) {

        // Check if order belongs to current queue, has work left, and has arrived
        if (orders[i].queueLevel == level &&
            orders[i].remainingTime > 0 &&
            orders[i].arrivalTime <= *time) {

            printf("\nTime %d: Processing Order %d in Queue %d",
                   *time, orders[i].id, level);

            // Decide execution time (min of remaining time and quantum)
            int execTime = (orders[i].remainingTime > quantum)
                           ? quantum
                           : orders[i].remainingTime;

            // Execute the order
            orders[i].remainingTime -= execTime;
            *time += execTime;

            // Update waiting time for other orders
            for (int j = 0; j < n; j++) {

                if (j != i &&
                    orders[j].remainingTime > 0 &&
                    orders[j].arrivalTime <= *time) {

                    orders[j].waitingTime += execTime;

                    // Aging: Promote if waiting too long
                    if (orders[j].waitingTime > AGING_LIMIT &&
                        orders[j].queueLevel > 1) {

                        orders[j].queueLevel--;
                        orders[j].waitingTime = 0;
                    }
                }
            }

            // Check if order is completed
            if (orders[i].remainingTime == 0) {
                orders[i].completionTime = *time;
                printf(" -> Completed");
            } else {
                printf(" -> Remaining: %d", orders[i].remainingTime);

                // Demotion to lower priority queue
                if (level < 3) {
                    orders[i].queueLevel++;
                }
            }
        }
    }
}

int main() {

    struct Order orders[MAX];
    int n = 25;

    // ---------------- Initialization ----------------
    for (int i = 0; i < n; i++) {

        orders[i].id = i + 1;

        // Staggered arrival times
        orders[i].arrivalTime = i % 5;

        // Preparation time between 2 and 8
        orders[i].prepTime = (i % 7) + 2;

        orders[i].remainingTime = orders[i].prepTime;
        orders[i].waitingTime = 0;

        // Assign initial queue levels
        if (i < 8)
            orders[i].queueLevel = 1;
        else if (i < 16)
            orders[i].queueLevel = 2;
        else
            orders[i].queueLevel = 3;
    }

    int time = 0;
    int completed = 0;

    printf("\n--- MLFQ Restaurant Order Scheduling ---\n");

    // ---------------- Scheduling Loop ----------------
    while (completed < n) {

        // Execute queues in priority order
        execute(orders, n, 1, Q1_QUANTUM, &time);
        execute(orders, n, 2, Q2_QUANTUM, &time);
        execute(orders, n, 3, Q3_QUANTUM, &time);

        // Count completed orders
        completed = 0;
        for (int i = 0; i < n; i++) {
            if (orders[i].remainingTime == 0)
                completed++;
        }
    }

    // ---------------- Metrics Calculation ----------------
    float totalWT = 0;
    float totalTAT = 0;

    printf("\n\nOrder Metrics:\n");
    printf("ID\tAT\tPT\tCT\tTAT\tWT\n");

    for (int i = 0; i < n; i++) {

        int tat = orders[i].completionTime - orders[i].arrivalTime;
        int wt  = tat - orders[i].prepTime;

        totalWT += wt;
        totalTAT += tat;

        printf("%d\t%d\t%d\t%d\t%d\t%d\n",
               orders[i].id,
               orders[i].arrivalTime,
               orders[i].prepTime,
               orders[i].completionTime,
               tat,
               wt);
    }

    // ---------------- Averages ----------------
    printf("\nAverage Waiting Time = %.2f", totalWT / n);
    printf("\nAverage Turnaround Time = %.2f\n", totalTAT / n);

    return 0;
}