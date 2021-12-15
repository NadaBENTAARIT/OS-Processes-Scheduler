#include "../../scheduler/scheduler_functions.h"
#include "../../file_manager/file_manager_functions.h"
#include "../../analysis/analysis_functions.h"
#include <string.h>

PLNode *sjp(ReadyQueue *rq)
{
    ReadyQueueNode *rqn_ptr;
    ReadyQueueNode *process_to_run;
    PLNode *result;
    int n;

    process_to_run = rq->tail;
    rqn_ptr = rq->tail->next;
    while (rqn_ptr != NULL)
    {
        if (rqn_ptr->process_node_adress->pd.duration < process_to_run->process_node_adress->pd.duration)
        {
            process_to_run = rqn_ptr;
        }
        rqn_ptr = rqn_ptr->next;
    }

    result = process_to_run->process_node_adress;

    // Check if the process to run is at the begenning or end of the ready queue
    if (process_to_run == rq->head)
    {
        rq->head = rq->head->prev;
        if (rq->head != NULL)
        {
            rq->head->next = NULL;
        }
        else
        {
            rq->tail = NULL;
        }
    }
    else if (process_to_run == rq->tail)
    {
        rq->tail = rq->tail->next;
        if (rq->tail != NULL)
        {
            rq->tail->prev = NULL;
        }
        else
        {
            rq->head = NULL;
        }
    }
    else if (process_to_run != rq->tail && process_to_run != rq->head)
    {
        process_to_run->prev->next = process_to_run->next;
        process_to_run->next->prev = process_to_run->prev;
    }

    free(process_to_run);

    return result;
}

void sjf(PL pl, TDL *tdl)
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
        current_running_process = sjp(&rq);

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
    strcpy(algorithm, "SJF");

    pl = parse_file(argv[1]);
    pl_sort(pl);
    // print_process_list(pl);

    tdl = NULL;
    sjf(pl, &tdl);

    // Generate the analysis data
    load_data(pl, tdl, algorithm, 0, "./analysis/data/scheduler_dataset.csv");
    return 0;
}