#include "../../scheduler/scheduler_functions.h"
#include "../../file_manager/file_manager_functions.h"
#include "shared/priority_functions.h"

#include "../../analysis/analysis_functions.h"

void dynamic_priority(PL pl, char *priority, TDL *tdl)
{
    ReadyQueue rq;
    PLNode *ptr;
    PLNode *ptrxp;
    PLNode *process_to_be_selected;
    PLNode *current_running_process;
    int process_arrival_time;
    int n, search;

    int timer;
    int expected_end_execution_time;

    search = 1;

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
        if (search == 1)
        {
            current_running_process = get_process_with_priority(&rq, priority);
        }
        else
        {
            current_running_process = process_to_be_selected;
        }

        // The current running process is going to run for a certain amount of time
        // until a process with more priority come and take his palace
        expected_end_execution_time = current_running_process->pd.duration + timer;

        //
        search = 1;
        process_to_be_selected = NULL;
        while (ptr != NULL)
        {
            if (ptr->pd.arrival_time < expected_end_execution_time)
            {
                if (ptr->pd.priority <= current_running_process->pd.priority)
                {
                    enqueue(&rq, ptr);
                }
                else
                {
                    // Find the process with the highest priority:
                    // @time < expected end execution time
                    // priority > priority of the current running process
                    search = 0;
                    process_to_be_selected = ptr;
                    ptrxp = ptr->next;
                    while (ptrxp != NULL)
                    {
                        if (ptrxp->pd.priority > process_to_be_selected->pd.priority && ptrxp->pd.arrival_time == process_to_be_selected->pd.arrival_time)
                        {
                            enqueue(&rq, process_to_be_selected);
                            process_to_be_selected = ptrxp;
                            ptrxp = ptrxp->next;
                        }
                        else
                        {
                            ptr = ptrxp;
                            break;
                        }
                    }
                    break;
                }
            }
            else
            {
                if (ptr->pd.arrival_time == expected_end_execution_time)
                {
                    enqueue(&rq, ptr);
                }
                else
                {
                    if (isEmptyQueue(rq) == 1)
                    {
                        enqueue(&rq, ptr);
                        ptr = ptr->next;
                    }
                    break;
                }
            }

            ptr = ptr->next;
        }

        //---------------------

        if (current_running_process->pd.arrival_time > timer)
        {
            printf("The CPU was empty from %d to %d\n", timer, current_running_process->pd.arrival_time);
            timer = current_running_process->pd.arrival_time;
        }
        else if (timer > current_running_process->pd.arrival_time && n == 1)
        {
            printf("The process: %s was in the ready queue from the moment he wanted the CPU at %d to %d the moment when the CPU is empty\n", current_running_process->pd.process_name, current_running_process->pd.arrival_time, timer);
        }

        if (process_to_be_selected == NULL)
        {
            // Track the processes entry and exit cpu time
            track_process(tdl, current_running_process, timer, expected_end_execution_time);
            // End track
            printf("The process: %s ran from %d with a priority equal to %d and left the cpu at %d (process is not interrupted)\n\n", current_running_process->pd.process_name, timer, current_running_process->pd.priority, expected_end_execution_time);
            timer += current_running_process->pd.duration;
        }
        else
        {
            // Track the processes entry and exit cpu time
            track_process(tdl, current_running_process, timer, process_to_be_selected->pd.arrival_time);
            // End track
            printf("The process: %s ran from %d with a priority equal to %d and left the cpu at %d (process is interrupted by %s)\n\n", current_running_process->pd.process_name, timer, current_running_process->pd.priority, process_to_be_selected->pd.arrival_time, process_to_be_selected->pd.process_name);

            // Here we are sure that the current running process didn't finish its duration
            // Upgrading the current running process data

            // calculating the remaining duration of the current running process
            current_running_process->pd.duration = expected_end_execution_time - process_to_be_selected->pd.arrival_time;

            // Upgrading the value of the timer
            timer = process_to_be_selected->pd.arrival_time;

            // Upgrading the new arrival time of the current running process
            current_running_process->pd.arrival_time = timer;

            // the current running process is going to leave the cpu
            enqueue(&rq, current_running_process);
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

    char priority[5];
    char algorithm[100];

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
    
    strcpy(algorithm, "Dynamic_priority");
    

    pl = parse_file(argv[1]);
    pl_sort(pl);
    // print_process_list(pl);

    // Init the tracking data list
    tdl = NULL;

    // Performing the scheduling algorithm
    dynamic_priority(pl, priority, &tdl);

    // Generate the analysis data
    load_data(pl, tdl, algorithm, 0, "./analysis/data/scheduler_dataset.csv");
    
    /*
    printf("\nDEBUGGING \n");
    display_track_list(tdl);
    */
    return 0;
}