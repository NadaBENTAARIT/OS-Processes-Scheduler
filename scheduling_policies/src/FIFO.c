#include "../../scheduler/scheduler_functions.h"
#include "../../file_manager/file_manager_functions.h"
#include<string.h>
#include "../../analysis/analysis_functions.h"

void fifo(PL pl, TDL *tdl)
{
    ReadyQueue rq;
    PLNode *ptr;
    PLNode *current_running_process;
    int timer;

    // Init the ready queue
    rq.head = NULL;
    rq.tail = NULL;

    // Init the pointer used to loop through the linked list
    ptr = pl;

    // Init the current running process
    enqueue(&rq, ptr);
    timer = ptr->pd.arrival_time;
    ptr = ptr->next;

    int n;
    n = 0;

    while (!(ptr == NULL && isEmptyQueue(rq) == 1))
    {
        current_running_process = dequeue(&rq);

        // Calculating when the process will finish its execution
        // timer represents when the currnet running process will finis its execution

        if (current_running_process->pd.arrival_time > timer)
        {
            printf("The CPU was empty from %d to %d\n", timer, current_running_process->pd.arrival_time);
            timer = current_running_process->pd.arrival_time;
        }
        else if (timer > current_running_process->pd.arrival_time && n == 1)
        {
            printf("The process: %s was in the ready queue from the moment he wanted the CPU at %d to %d the moment when the CPU is empty\n", current_running_process->pd.process_name, current_running_process->pd.arrival_time, timer);
        }

        // Track the processes entry and exit cpu time
        track_process(tdl, current_running_process, timer, timer + current_running_process->pd.duration);
        // End track
        printf("The process: %s ran from %d with a duration of %d and left the cpu at %d\n\n", current_running_process->pd.process_name, timer, current_running_process->pd.duration, timer + current_running_process->pd.duration);
        timer += current_running_process->pd.duration;

        while (ptr != NULL)
        {
            if (ptr->pd.arrival_time <= timer)
            {
                enqueue(&rq, ptr);
                ptr = ptr->next;
            }
            else
            {
                // Test if the ready queue is empty
                if (isEmptyQueue(rq) == 1)
                {
                    enqueue(&rq, ptr);
                    ptr = ptr->next;
                }
                break;
            }
        }

        (n == 0) && (n++);
        // Display the state of the ready queue
        // ... to do
    }
}

int main(int argc, char **argv)
{
    PL pl;
    TDL tdl;

    char algorithm[100];

    strcpy(algorithm, "FIFO");

    pl = parse_file(argv[1]);
    pl_sort(pl);
    // print_process_list(pl);

    // Init the tracking data list
    tdl = NULL;
    fifo(pl, &tdl);

    // Generate the analysis data
    load_data(pl, tdl, algorithm, 0, "./analysis/data/scheduler_dataset.csv");
    return 0;
}