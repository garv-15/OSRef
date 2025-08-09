#include <stdio.h> 
#include <stdlib.h>
// all processes are represented in struct with all timings as variables
struct Process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
};

int main()
{
    int n;
    printf("Enter Number of Processes: ");
    scanf("%d", &n);
    struct Process *arr = (struct Process *)malloc(n*sizeof(struct Process));
    for (int i = 0; i < n; i++) {
        arr[i].pid = i + 1;
        printf("Arrival Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].at);
        printf("Burst Time for Process %d: ", i + 1);
        scanf("%d", &arr[i].bt);
    }
    int* gantt = (int *)malloc((n + 1) * sizeof(int));
    gantt[0] = 0;
    int stat = 0; //sum of turn around times
    int swt = 0; // sum of waiting times
    for (int i = 1; i < n + 1; i++) {
        gantt[i] = gantt[i - 1] + arr[i - 1].bt; //filling gantt chart, withi initial timing as 0
        arr[i - 1].ct = gantt[i];
        arr[i - 1].tat = arr[i - 1].ct - arr[i - 1].at; //TAT = CT - AT
        arr[i - 1].wt = arr[i - 1].tat - arr[i - 1].bt; //WT = TAT-BT
        stat += arr[i - 1].tat;
        swt += arr[i - 1].wt;
    }
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", arr[i].pid, arr[i].at, arr[i].bt, arr[i].ct, arr[i].tat, arr[i].wt);
    }

    printf("Average Turn Around Time: %.2f\n", (float)stat /n);
    printf("Average Waiting Time: %.2f\n", (float)swt /n);
    printf("Execution Order: Start(0) -> ");
    for (int i = 0; i < n; i++) {
        printf("P%d(%d)", arr[i].pid, arr[i].ct);
        if (i < n - 1)
            printf(" -> ");
    }
    printf("\n");
}
