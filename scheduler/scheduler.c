#include "scheduler_functions.h"

void add_process(PL *pl_adr, ProcessData pd)
{
    PL ptr;

    ptr = (PL)malloc(sizeof(PLNode));
    ptr->pd = pd;
    ptr->next = *pl_adr;
    *pl_adr = ptr;
}

void print_process_data(ProcessData pd)
{
    printf("Process Data:\n");
    puts(pd.process_name);
    printf("%d", pd.arrival_time);
    printf("\n");
    printf("%d", pd.duration);
    printf("\n");
    printf("%d", pd.priority);
    printf("\n");
}

void print_process_list(PL pl)
{
    PL ptr;

    ptr = pl;
    while (ptr != NULL)
    {
        printf("\n------------------------\n");
        print_process_data(ptr->pd);
        printf("\n------------------------\n");
        ptr = ptr->next;
    }
}

void enqueue(ReadyQueue *rq, PLNode *process_node_adress)
{
    ReadyQueueNode *ptr;

    ptr = (ReadyQueueNode *)malloc(sizeof(ReadyQueueNode));

    ptr->prev = NULL;
    ptr->process_node_adress = process_node_adress;
    ptr->next = rq->tail;

    if (rq->tail != NULL){
        rq->tail->prev = ptr;
    }
    
    rq->tail = ptr;
    if (rq->head == NULL)
    {
        rq->head = ptr;
    }
}

PLNode* dequeue(ReadyQueue *rq)
{
    PLNode *pl_node_adress;
    ReadyQueueNode *ptr;

    pl_node_adress = NULL;
    ptr = NULL;

    ptr = rq->head;
    pl_node_adress = ptr->process_node_adress;
    rq->head = rq->head->prev;
    if (rq->head == NULL){
        rq->tail = NULL;
    } else {
        rq->head->next = NULL;
    }
   
    free(ptr); 
    return pl_node_adress;
}

int isEmptyQueue(ReadyQueue rq){
    if (rq.head == NULL && rq.tail == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void print_rq(ReadyQueue rq){
    ReadyQueueNode *ptr;
    ptr = rq.tail;

    while (ptr != NULL)
    {
        printf("\n-----------Start rq  -----------\n");
        printf("Prev: %p", ptr->prev);
        printf("\n");
        print_process_data((*(*ptr).process_node_adress).pd);
        printf("Next: %p", ptr->next);
        ptr = ptr->next;

        printf("\n-----------End rq  -----------\n");
    }  
    
}