#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
    int done;
};

int main() {
    int n, completed = 0, time = 0;
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    struct Process *arr = (struct Process *)malloc(n * sizeof(struct Process));
    for (int i = 0; i < n; i++) {
        arr[i].pid = i + 1;
        printf("Arrival Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].at);
        printf("Burst Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].bt);
        arr[i].rt = arr[i].bt;  // remaining time
        arr[i].done = 0;
    }

    int stat = 0, swt = 0;
    int ganttP[1000], ganttT[1000], gc = 0;
    int current = -2; // store current process in CPU (-2 means nothing yet)

    while (completed < n) {
        int idx = -1, min_rt = 99999;

        // Pick shortest remaining time among arrived processes
        for (int i = 0; i < n; i++) {
            if (!arr[i].done && arr[i].at <= time && arr[i].rt < min_rt && arr[i].rt > 0) {
                min_rt = arr[i].rt;
                idx = i;
            }
        }

        if (idx == -1) { // Idle
            if (current != -1) { // record idle change
                ganttP[gc] = -1;
                ganttT[gc] = time;
                gc++;
            }
            current = -1;
            time++;
            continue;
        }

        // If CPU changes process, log it in Gantt chart
        if (idx != current) {
            ganttP[gc] = (idx == -1) ? -1 : arr[idx].pid;
            ganttT[gc] = time;
            gc++;
            current = idx;
        }

        // Run process for 1 unit
        arr[idx].rt--;
        time++;

        // If process finishes
        if (arr[idx].rt == 0) {
            arr[idx].ct = time;
            arr[idx].tat = arr[idx].ct - arr[idx].at;
            arr[idx].wt = arr[idx].tat - arr[idx].bt;
            arr[idx].done = 1;
            stat += arr[idx].tat;
            swt += arr[idx].wt;
            completed++;
        }
    }

    // Record final completion time for Gantt chart
    ganttT[gc] = time;

    // Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               arr[i].pid, arr[i].at, arr[i].bt, arr[i].ct, arr[i].tat, arr[i].wt);
    }

    printf("\nAverage Turn Around Time: %.2f\n", (float)stat / n);
    printf("Average Waiting Time: %.2f\n", (float)swt / n);

    // Gantt chart execution order
    printf("\nExecution Order: Start(0) -> ");
    for (int i = 0; i < gc; i++) {
        if (ganttP[i] == -1)
            printf("Idle(%d)", ganttT[i+1]);
        else
            printf("P%d(%d)", ganttP[i], ganttT[i+1]);
        if (i < gc - 1)
            printf(" -> ");
    }
    printf("\n");

    free(arr);
    return 0;
}
