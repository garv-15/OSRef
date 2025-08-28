#include <stdio.h>
#include <stdlib.h>

int p, r;
int **alloc, **need, **max;
int *avail;

void printmatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void inputmatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

void calcNeed() {
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

int safeSeq() {
    int *work = (int *)malloc(r * sizeof(int));
    int *finish = (int *)malloc(p * sizeof(int));
    int *sequence = (int *)malloc(p * sizeof(int));

    // Initialize
    for (int i = 0; i < r; i++) work[i] = avail[i];
    for (int i = 0; i < p; i++) finish[i] = 0;

    int count = 0;
    while (count < p) {
        int found = 0;

        for (int i = 0; i < p; i++) {
            if (!finish[i]) {
                int alloc_f = 1;

                for (int j = 0; j < r; j++) {
                    if (need[i][j] > work[j]) {
                        alloc_f = 0;
                        break;
                    }
                }

                if (alloc_f) {
                    for (int j = 0; j < r; j++) {
                        work[j] += alloc[i][j];
                    }
                    sequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a safe state!\n");
            free(work);
            free(finish);
            free(sequence);
            return 0;
        }
    }

    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < p; i++) {
        printf("P%d ", sequence[i]);
    }
    printf("\n");

    free(work);
    free(finish);
    free(sequence);
    return 1;
}

/*
int requestResources(int pid, int request[]) {
    // 1. Check request ≤ need
    for (int j = 0; j < r; j++) {
        if (request[j] > need[pid][j]) {
            printf("Error: Process P%d has exceeded its maximum claim!\n", pid);
            return 0;
        }
    }

    // 2. Check request ≤ available
    for (int j = 0; j < r; j++) {
        if (request[j] > avail[j]) {
            printf("Process P%d must wait: resources not available.\n", pid);
            return 0;
        }
    }

    // 3. Pretend to allocate
    for (int j = 0; j < r; j++) {
        avail[j] -= request[j];
        alloc[pid][j] += request[j];
        need[pid][j] -= request[j];
    }

    // 4. Check if safe
    if (safeSeq()) {
        printf("Request granted to process P%d.\n", pid);
        return 1;
    } else {
        // Rollback
        for (int j = 0; j < r; j++) {
            avail[j] += request[j];
            alloc[pid][j] -= request[j];
            need[pid][j] += request[j];
        }
        printf("Request by process P%d cannot be granted (unsafe state).\n", pid);
        return 0;
    }
}
*/


int main() {
    printf("Enter number of processes: ");
    scanf("%d", &p);
    printf("Enter number of resources: ");
    scanf("%d", &r);

    // Allocate memory
    alloc = (int **)malloc(p * sizeof(int *));
    max   = (int **)malloc(p * sizeof(int *));
    need  = (int **)malloc(p * sizeof(int *));
    for (int i = 0; i < p; i++) {
        alloc[i] = (int *)malloc(r * sizeof(int));
        max[i]   = (int *)malloc(r * sizeof(int));
        need[i]  = (int *)malloc(r * sizeof(int));
    }
    avail = (int *)malloc(r * sizeof(int));

    // Input
    printf("Enter allocation matrix:\n");
    inputmatrix(alloc, p, r);
    printf("Enter maximum matrix:\n");
    inputmatrix(max, p, r);
    printf("Enter available resources:\n");
    for (int i = 0; i < r; i++) {
        scanf("%d", &avail[i]);
    }

    calcNeed();

    // Output
    printf("\nSystem Snapshot\n");
    printf("Allocation matrix:\n");
    printmatrix(alloc, p, r);
    printf("Maximum matrix:\n");
    printmatrix(max, p, r);
    printf("Available resources:\n");
    for (int i = 0; i < r; i++) {
        printf("%d ", avail[i]);
    }
    printf("\n");
    printf("Need Matrix:\n");
    printmatrix(need, p, r);
    safeSeq();
    

    /*
    int pid;
    printf("\nEnter process id making a request (0-%d): ", p-1);
    scanf("%d", &pid);
    int *request = (int *)malloc(r * sizeof(int));
    printf("Enter request vector: ");
    for (int i = 0; i < r; i++) scanf("%d", &request[i]);

    requestResources(pid, request);

    // cleanup
    for (int i = 0; i < p; i++) {
        free(alloc[i]); free(max[i]); free(need[i]);
    }
    free(alloc); free(max); free(need); free(avail); free(request);
    return 0;

    */
    return 0;
}
