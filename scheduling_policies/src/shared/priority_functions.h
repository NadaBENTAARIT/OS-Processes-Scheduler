#ifndef PRIORITY_FUNCTIONS_HEADER_FILE
#define PRIORITY_FUNCTIONS_HEADER_FILE

#include "../../../scheduler/types.h"
#include <string.h>

PLNode* get_process_with_priority(ReadyQueue *rq, char *priority){
    ReadyQueueNode *rqn_ptr;
    ReadyQueueNode *process_to_run;
    PLNode *result;
    int n;
    char priority_order[5];
    strcpy(priority_order, "DESC");
    
    process_to_run = rq->tail;
    rqn_ptr = rq->tail->next;
    while (rqn_ptr != NULL)
    {
        if (strcmp(priority, priority_order) == 0){
            if (rqn_ptr->process_node_adress->pd.priority < process_to_run->process_node_adress->pd.priority){
                process_to_run = rqn_ptr;
            }
        } else {
            if (rqn_ptr->process_node_adress->pd.priority > process_to_run->process_node_adress->pd.priority){
                process_to_run = rqn_ptr;
            }
        }
        
        rqn_ptr = rqn_ptr->next;
    }
    
    result = process_to_run->process_node_adress;


    // Check if the process to run is at the begenning or end of the ready queue
    if (process_to_run == rq->head){
        rq->head = rq->head->prev;
        if (rq->head != NULL){
            rq->head->next = NULL;
        } else {
            rq->tail = NULL;
        }
        
    } else if (process_to_run == rq->tail){        
        rq->tail = rq->tail->next;
        if (rq->tail != NULL){
            rq->tail->prev = NULL;
        } else {
            rq->head = NULL;
        }
    } else if (process_to_run != rq->tail && process_to_run != rq->head){        
        process_to_run->prev->next = process_to_run->next;
        process_to_run->next->prev = process_to_run->prev;
    }

    free(process_to_run);

    return result;
} 

#endif