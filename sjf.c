#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, ct, tat, wt;
    int done;
};

int main() {
    int n, completed = 0, time = 0; // number of completed process
    // total time so far
    printf("Enter Number of Processes: ");
    scanf("%d", &n);

    struct Process *arr = (struct Process *)malloc(n * sizeof(struct Process));
    for (int i = 0; i < n; i++) {
        arr[i].pid = i + 1;
        printf("Arrival Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].at);
        printf("Burst Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].bt);
        arr[i].done = 0;
    }
    // sum of turn around time and waiting time
    int stat = 0, swt = 0;
    int gantt_proc[50], gantt_time[50], gc = 0; // gantt entries
    while (completed < n) {
        int idx = -1, min_bt = 99999; //initializing minimum burst to a high number for calculations
        for (int i = 0; i < n; i++) {
            if (!arr[i].done && arr[i].at <= time && arr[i].bt < min_bt) {
                min_bt = arr[i].bt;
                idx = i;
            }
        }
        if (idx == -1) {
            int next_at = 99999; // initializing arrival to a high number for comparison adjustmnet
            for (int i = 0; i < n; i++) {
                if (!arr[i].done && arr[i].at > time && arr[i].at < next_at) {
                    next_at = arr[i].at;
                }
            }
            gantt_proc[gc] = -1;    // if no task, set process in gantt as -1
            time = next_at;    // jump time
            gantt_time[gc] = time; //set ending as start of next process
            gc++; //increment gantt entries
            continue;
        }

        gantt_proc[gc] = arr[idx].pid; //filling gantt chart
        time += arr[idx].bt;
        gantt_time[gc] = time;
        gc++;

        arr[idx].ct = time;
        arr[idx].tat = arr[idx].ct - arr[idx].at;
        arr[idx].wt = arr[idx].tat - arr[idx].bt;
        arr[idx].done = 1;

        stat += arr[idx].tat;
        swt += arr[idx].wt;
        completed++;
    }

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n",
               arr[i].pid, arr[i].at, arr[i].bt, arr[i].ct, arr[i].tat, arr[i].wt);
    }

    printf("\nAverage Turn Around Time: %.2f\n", (float)stat / n);
    printf("Average Waiting Time: %.2f\n", (float)swt / n);

    printf("\nExecution Order: Start(0) -> ");
    for (int i = 0; i < gc; i++) {
        if (gantt_proc[i] == -1)
            printf("Idle(%d)", gantt_time[i]); //if cpu is idle, print idle
        else
            printf("P%d(%d)", gantt_proc[i], gantt_time[i]);

        if (i < gc - 1)
            printf(" -> "); 
    }

    printf("\n");
    return 0;
}
