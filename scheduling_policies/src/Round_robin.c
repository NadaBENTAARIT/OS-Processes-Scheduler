#include "../../scheduler/scheduler_functions.h"
#include "../../file_manager/file_manager_functions.h"
#include <string.h>
#include "../../analysis/analysis_functions.h"

#include "../../utils/types_validation.h"

void round_robin(PL pl, int quantum, TDL *tdl)
{
    ReadyQueue rq;
    PLNode *ptr;
    PLNode *current_running_process;
    int timer, old_timer, flag;

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

        // Here I consider that the process will finish its execution
        flag = 1;

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

        if (quantum < current_running_process->pd.duration)
        {
            // Saving the old of value timer in a tmp variable
            old_timer = timer;

            // Calculating the new duration of the process
            current_running_process->pd.duration = current_running_process->pd.duration - quantum;

            // Updating the value of timer
            timer += quantum;

            // Calculating the new arrival time of a process
            current_running_process->pd.arrival_time = timer;

            // Mean that the current running process wants more processor time
            flag = 0;

            // Track the processes entry and exit cpu time
            track_process(tdl, current_running_process, old_timer, timer);
            // End track
            printf("The process %s ran during 1 quantum, from %d to %d, the new duration is equal to = %d and the new arrival time is = %d\n\n", current_running_process->pd.process_name, old_timer, timer, current_running_process->pd.duration, current_running_process->pd.arrival_time);
        }
        else
        {
            old_timer = timer;
            timer += current_running_process->pd.duration;
            // Track the processes entry and exit cpu time
            track_process(tdl, current_running_process, old_timer, timer);
            // End track
            printf("The process %s ran during %d CPU unit, from %d to %d. The process finish its execution.\n\n", current_running_process->pd.process_name, current_running_process->pd.duration, old_timer, timer);
        }

        while (ptr != NULL)
        {
            if (ptr->pd.arrival_time <= timer)
            {
                enqueue(&rq, ptr);
                ptr = ptr->next;

                if (flag == 0)
                {
                    // Enqueue the current running process after the new coming pocesses
                    enqueue(&rq, current_running_process);
                }
            }
            else
            {
                if (flag == 0)
                {
                    enqueue(&rq, current_running_process);
                }

                // Test if the ready queue is empty
                if (isEmptyQueue(rq) == 1)
                {
                    enqueue(&rq, ptr);
                    ptr = ptr->next;
                }
                break;
            }
        }

        // Check if the last process didn't finish its execution
        if (ptr == NULL)
        {
            if (flag == 0)
            {
                enqueue(&rq, current_running_process);
            }
        }

        // Increment he value of n to test if the process if the 1st one or not
        (n == 0) && (n++);

        // Display the state of the ready queue
        // ... to do
    }
}

int main(int argc, char **argv)
{
    PL pl;
    TDL tdl;

    int quantum;
    char algorithm[100];

    char input[256];
    int res;

    strcpy(algorithm, "Round_robin");

    pl = parse_file(argv[1]);
    pl_sort(pl);

    // Getting the value of the quantum from the user
    res = 0;
    printf("\nEnter the value of the quantum: ");
    while (res == 0)
    {
        scanf("%s", input);
        res = digit_check(input);
        if (res == 0)
        {
            printf("The quantum must be an integer !, Pleas try again: ");
        }
    }

    quantum = atoi(input);
    
    // Init the tracking list
    tdl = NULL;
    
    // print_process_list(pl);
    round_robin(pl, quantum, &tdl);

    // Generate the analysis data
    load_data(pl, tdl, algorithm, quantum, "./analysis/data/scheduler_dataset.csv");
    return 0;
}