#include "../../scheduler/scheduler_functions.h"
#include "../../file_manager/file_manager_functions.h"

#include "../../analysis/analysis_functions.h"
// This header file contains all the shared functions between the static and dynamic priority algorithms
#include "shared/priority_functions.h"

void static_priority(PL pl, char *priority, TDL *tdl)
{
    ReadyQueue rq;
    PLNode *ptr;
    PLNode *current_running_process;
    int process_arrival_time;
    int n;

    int timer;

    // Init the ready queue
    rq.head = NULL;
    rq.tail = NULL;

    // Init the pointer used to loop through the linked list
    ptr = pl;

    // Init the ready queue by the elemnts wich will be used later
    enqueue(&rq, ptr);
    timer = ptr->pd.arrival_time;

    ptr = ptr->next;
    while (ptr != NULL)
    {
        if (ptr->pd.arrival_time == timer)
        {
            enqueue(&rq, ptr);
            ptr = ptr->next;
        }
        else
        {
            break;
        }
    }

    n = 0;
    while (!(isEmptyQueue(rq) == 1))
    {
        current_running_process = get_process_with_priority(&rq, priority);

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
        printf("The process: %s ran from %d with a priority equal to %d and left the cpu at %d\n\n", current_running_process->pd.process_name, timer, current_running_process->pd.priority, timer + current_running_process->pd.duration);
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
    char priority[5];

    int res;

    res = 0;
    printf("Type the order in wich you want the processes to be scheduled (allowed values are ASC / DESC): ");
    while (res == 0)
    {
        scanf("%s", priority);
        if (strcmp(priority, "ASC") == 0 || strcmp(priority, "DESC") == 0)
        {
            res = 1;
        }    
        else
        {
            printf("Order selected does not match the allowed values !, Pleas try again: ");
        }
    }
    

    strcpy(algorithm, "Static_priority");
    

    pl = parse_file(argv[1]);
    pl_sort(pl);
    // print_process_list(pl);
    
    static_priority(pl, priority, &tdl);

    // Generate the analysis data
    load_data(pl, tdl, algorithm, 0, "./analysis/data/scheduler_dataset.csv");
    return 0;
}