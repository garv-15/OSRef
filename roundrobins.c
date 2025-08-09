#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, rt, ct, tat, wt;
    int done;
};

int main() {
    int n, tq;
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    struct Process *arr = (struct Process *)malloc(n * sizeof(struct Process));
    for (int i = 0; i < n; i++) {
        arr[i].pid = i + 1;
        printf("Arrival Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].at);
        printf("Burst Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].bt);
        arr[i].rt = arr[i].bt; // remaining time
        arr[i].done = 0;
    }

    int stat = 0, swt = 0;
    int ganttP[1000], ganttS[1000], ganttE[1000], gc = 0;

    int time = 0, completed = 0;
    int queue[1000], front = 0, rear = 0;
    int inQueue[n];
    for (int i = 0; i < n; i++) inQueue[i] = 0;

    // Push first arrivals at time 0
    for (int i = 0; i < n; i++) {
        if (arr[i].at == 0) {
            queue[rear++] = i;
            inQueue[i] = 1;
        }
    }

    if (rear == 0) { // idle until first process arrives
        int first_at = 99999, idx = -1;
        for (int i = 0; i < n; i++) {
            if (arr[i].at < first_at) {
                first_at = arr[i].at;
                idx = i;
            }
        }
        time = first_at;
        queue[rear++] = idx;
        inQueue[idx] = 1;
    }

    while (completed < n) {
        if (front == rear) { // queue empty → idle until next arrival
            int next_at = 99999, idx = -1;
            for (int i = 0; i < n; i++) {
                if (!arr[i].done && arr[i].at > time && arr[i].at < next_at) {
                    next_at = arr[i].at;
                    idx = i;
                }
            }
            time = next_at;
            queue[rear++] = idx;
            inQueue[idx] = 1;
            continue;
        }

        int idx = queue[front++];
        int start_time = time;

        if (arr[idx].rt <= tq) {
            time += arr[idx].rt;
            arr[idx].rt = 0;
            arr[idx].ct = time;
            arr[idx].tat = arr[idx].ct - arr[idx].at;
            arr[idx].wt = arr[idx].tat - arr[idx].bt;
            arr[idx].done = 1;
            stat += arr[idx].tat;
            swt += arr[idx].wt;
            completed++;
        } else {
            time += tq;
            arr[idx].rt -= tq;
        }

        // Record Gantt chart entry
        ganttP[gc] = arr[idx].pid;
        ganttS[gc] = start_time;
        ganttE[gc] = time;
        gc++;

        // Add new arrivals that came during execution
        for (int i = 0; i < n; i++) {
            if (!arr[i].done && !inQueue[i] && arr[i].at <= time) {
                queue[rear++] = i;
                inQueue[i] = 1;
            }
        }

        // If not finished, requeue
        if (!arr[idx].done) {
            queue[rear++] = idx;
        }
    }

    // Table
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               arr[i].pid, arr[i].at, arr[i].bt, arr[i].ct, arr[i].tat, arr[i].wt);
    }

    printf("\nAverage Turn Around Time: %.2f\n", (float)stat / n);
    printf("Average Waiting Time: %.2f\n", (float)swt / n);

    // Execution order with start-end times
    printf("\nExecution Order: Start(%d) -> ", ganttS[0]);
    for (int i = 0; i < gc; i++) {
        printf("P%d[%d-%d]", ganttP[i], ganttS[i], ganttE[i]);
        if (i < gc - 1)
            printf(" -> ");
    }
    printf("\n");

    free(arr);
    return 0;
}
