#ifndef ANALYSIS_FUNCTIONS_HEADER_FILE
#define ANALYSIS_FUNCTIONS_HEADER_FILE

#include "analysis.h"
#include "../scheduler/types.h"

void get_execution_id(char *fileName, char *buffer);
void set_execution_id(char *fileName, char *buffer);
TDL get_process_data(TDL tdl, char *process_name);
void init_tracking_data_node(TDL *tdl, char *process_name);
void set_cpu_time(TDL node_adress, char *target, int value);
void display_track_list(TDL tdl);
void track_process(TDL *tdl, PLNode *current_running_process, int entry_cpu_time, int exit_cpu_time);
void load_data(PL pl, TDL tdl, char *algorithm, int quantum, char *destination_file);

#endif