#ifndef SCHEDULER_FUNCTIONS_HEADER_FILE
#define SCHEDULER_FUNCTIONS_HEADER_FILE

#include "types.h"

void add_process(PL *pl_adr, ProcessData pd);
void print_process_data(ProcessData pd);
void print_process_list(PL pl);
void enqueue(ReadyQueue *rq, PLNode *process_node_adress);
PLNode* dequeue(ReadyQueue *rq);
int isEmptyQueue(ReadyQueue rq);
void print_rq(ReadyQueue rq);

#endif