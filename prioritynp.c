#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid, at, bt, ct, tat, wt, prio;
    int done;
};

//struct for processes, done for process completion and 
// prio for priority

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
        //lower number has higher priority in the processes
        printf("Priority for Process %d (low has higher priority): ", i + 1);
        scanf("%d", &arr[i].prio);
        arr[i].done = 0;
    }

    int stat = 0, swt = 0; //sum of turn around times and waiting times
    int gant_proc[70], gant_time[70], gc = 0;
    //initializing gantt entries for time and process id
    //initializing count (gc) to 0
    while (completed < n) {
        int idx = -1, high_p = 999; 
        //high p set to help in initial comparision;

        for (int i = 0; i < n; i++) {
            if (!arr[i].done && arr[i].at <= time && arr[i].prio < high_p) {
                high_p = arr[i].prio;
                idx = i;
            }
        }

        if (idx == -1) {
            int next_at = 999;
            //high next_at set to help in initial comparison
            for (int i = 0; i < n; i++) {
                if (!arr[i].done && arr[i].at > time && arr[i].at < next_at) {
                    next_at = arr[i].at;
                }
            }
            //gantt entries if process is idle (entry is filled with -1)
            gant_proc[gc] = -1;   
            time = next_at;
            gant_time[gc] = time;
            gc++;
            continue;
        }

        gant_proc[gc] = arr[idx].pid;
        time += arr[idx].bt;
        gant_time[gc] = time;
        gc++;
        //filling process parameters for table
        arr[idx].ct = time;
        arr[idx].tat = arr[idx].ct - arr[idx].at;
        arr[idx].wt = arr[idx].tat - arr[idx].bt;
        arr[idx].done = 1;

        stat += arr[idx].tat;
        swt += arr[idx].wt;
        completed++;
    }

    // Table
    printf("\nPID\tAT\tBT\tPRIO\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               arr[i].pid, arr[i].at, arr[i].bt, arr[i].prio,
               arr[i].ct, arr[i].tat, arr[i].wt);
    }

    printf("\nAverage Turn Around Time: %.2f\n", (float)stat / n);
    printf("Average Waiting Time: %.2f\n", (float)swt / n);


    printf("\nExecution Order: Start(0) -> ");
    for (int i = 0; i < gc; i++) {
        if (gant_proc[i] == -1)
            printf("Idle(%d)", gant_time[i]);
        else
            printf("P%d(%d)", gant_proc[i], gant_time[i]);

        if (i < gc - 1)
            printf(" -> ");
    }
    printf("\n");

}

