#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>

#include "analysis_functions.h"

void get_execution_id(char *fileName, char *buffer)
{
    FILE *file = fopen(fileName, "r"); /* should check the result */

    if (!file)
    {
        printf("\n Unable to open : %s ", fileName);
    }

    fgets(buffer, sizeof(buffer), file);
    fclose(file);
}

void set_execution_id(char *fileName, char *buffer)
{
    FILE *file = fopen(fileName, "w"); /* should check the result */

    if (!file)
    {
        printf("\n Unable to open : %s ", fileName);
    }

    fputs(buffer, file);
    fclose(file);
}

TDL get_process_data(TDL tdl, char *process_name)
{
    TDL ptr;
    TDL res;

    res = NULL;
    ptr = tdl;

    while (ptr != NULL)
    {
        if (strcmp(ptr->data.process_name, process_name) == 0)
        {
            res = ptr;
            break;
        }
        ptr = ptr->next;
    }

    return res;
}

void set_cpu_time(TDL node_adress, char *target, int value)
{
    CPU_Time_List ptr;
    CPU_Time_List new_cpu_time_ptr;
    if (strcmp(target, "IN") == 0)
    {
        ptr = node_adress->data.entry_cpu_time;
        if (ptr == NULL)
        {
            node_adress->data.entry_cpu_time = (CPU_Time_Node *)malloc(sizeof(CPU_Time_Node));
            node_adress->data.entry_cpu_time->time = value;
            node_adress->data.entry_cpu_time->next = NULL;
        }
        else
        {
            while (ptr->next != NULL)
            {
                ptr = ptr->next;
            }

            new_cpu_time_ptr = (CPU_Time_Node *)malloc(sizeof(CPU_Time_Node));
            new_cpu_time_ptr->time = value;
            new_cpu_time_ptr->next = NULL;

            ptr->next = new_cpu_time_ptr;
        }
    }
    else
    {
        ptr = node_adress->data.exit_cpu_time;
        if (ptr == NULL)
        {
            node_adress->data.exit_cpu_time = (CPU_Time_Node *)malloc(sizeof(CPU_Time_Node));
            node_adress->data.exit_cpu_time->time = value;
            node_adress->data.exit_cpu_time->next = NULL;
        }
        else
        {
            while (ptr->next != NULL)
            {
                ptr = ptr->next;
            }

            new_cpu_time_ptr = (CPU_Time_Node *)malloc(sizeof(CPU_Time_Node));
            new_cpu_time_ptr->time = value;
            new_cpu_time_ptr->next = NULL;

            ptr->next = new_cpu_time_ptr;
        }
    }
}

void init_tracking_data_node(TDL *tdl, char *process_name)
{
    TDL new_tracking_data_node_ptr;

    new_tracking_data_node_ptr = (TrackingDataNode *)malloc(sizeof(TrackingDataNode));
    // Init the node data
    strcpy(new_tracking_data_node_ptr->data.process_name, process_name);
    new_tracking_data_node_ptr->data.entry_cpu_time = NULL;
    new_tracking_data_node_ptr->data.exit_cpu_time = NULL;

    // Setting the next to the head of the linked list
    new_tracking_data_node_ptr->next = *tdl;
    *tdl = new_tracking_data_node_ptr;
}

void display_track_list(TDL tdl)
{
    TDL ptr;
    CPU_Time_List cpu_time_list_ptr;

    ptr = tdl;

    printf("\nDisplaying the track list\n");
    while (ptr != NULL)
    {
        printf("------------------------------------\n");
        printf("The process name: %s\n", ptr->data.process_name);

        printf("The process CPU entry time\n");
        cpu_time_list_ptr = ptr->data.entry_cpu_time;
        printf("[");
        while (cpu_time_list_ptr != NULL)
        {
            printf("%d ", cpu_time_list_ptr->time);
            cpu_time_list_ptr = cpu_time_list_ptr->next;
        }
        printf("]\n");

        printf("The process CPU exit time\n");
        cpu_time_list_ptr = ptr->data.exit_cpu_time;
        printf("[");
        while (cpu_time_list_ptr != NULL)
        {
            printf("%d ", cpu_time_list_ptr->time);
            cpu_time_list_ptr = cpu_time_list_ptr->next;
        }
        printf("]\n");

        printf("------------------------------------\n");

        ptr = ptr->next;
    }
}

void track_process(TDL *tdl, PLNode *current_running_process, int entry_cpu_time, int exit_cpu_time)
{
    TDL tdli;
    tdli = get_process_data(*tdl, current_running_process->pd.process_name);
    if (tdli == NULL)
    {
        init_tracking_data_node(tdl, current_running_process->pd.process_name);
        set_cpu_time(*tdl, "IN", entry_cpu_time);
        set_cpu_time(*tdl, "OUT", exit_cpu_time);
    }
    else
    {
        set_cpu_time(tdli, "IN", entry_cpu_time);
        set_cpu_time(tdli, "OUT", exit_cpu_time);
    }
}

void load_data(PL pl, TDL tdl, char *algorithm, int quantum, char *destination_file)
{
    PL ptr;
    TDL tracked_process_data_ptr;
    CPU_Time_List cpu_time_ptr;

    char buffer[256];
    int execution_id;
    // Loading the data to the csv file

    // Opening the destination file
    FILE *file = fopen(destination_file, "w"); /* should check the result */
    if (!file)
    {
        printf("\n Unable to open the destination file : %s ", destination_file);
    }
    else
    {
        // Wrting the file header 
        fprintf(file, "Execution_ID,Process_Name,Arrival_Time,Duration,Priority,Algorithm,Entry_CPU_Time,Exit_CPU_Time,Quantum\n");

        // 1- Extracting the execution id
        get_execution_id("./analysis/config", buffer);
        execution_id = atoi(buffer);
        execution_id++;
        sprintf(buffer, "%d", execution_id);
        set_execution_id("./analysis/config", buffer);

        // 2- Looping through the process list and loading the data to the csv file
        ptr = pl;
        while (ptr != NULL)
        {
            // 3- Find the current process entry and exit cpu time
            tracked_process_data_ptr = get_process_data(tdl, ptr->pd.process_name);

            // 4- Loading data to the destination file
            fprintf(file, "%d", execution_id);
            fprintf(file, ",");

            fprintf(file, "%s", ptr->pd.process_name);
            fprintf(file, ",");

            fprintf(file, "%d", ptr->pd.backup.arrival_time);
            fprintf(file, ",");

            fprintf(file, "%d", ptr->pd.backup.duration);
            fprintf(file, ",");

            fprintf(file, "%d", ptr->pd.priority);
            fprintf(file, ",");

            fprintf(file, "%s", algorithm);
            fprintf(file, ",");

            // Loading the Entry_CPU_Time
            fprintf(file, "[");
            cpu_time_ptr = tracked_process_data_ptr->data.entry_cpu_time;
            while (cpu_time_ptr != NULL)
            {
                fprintf(file, "%d", cpu_time_ptr->time);
                if (cpu_time_ptr->next != NULL)
                {
                    fprintf(file, ",");
                }

                cpu_time_ptr = cpu_time_ptr->next;
            }

            fprintf(file, "],[");

            cpu_time_ptr = tracked_process_data_ptr->data.exit_cpu_time;
            while (cpu_time_ptr != NULL)
            {
                fprintf(file, "%d", cpu_time_ptr->time);
                if (cpu_time_ptr->next != NULL)
                {
                    fprintf(file, ",");
                }

                cpu_time_ptr = cpu_time_ptr->next;
            }

            fprintf(file, "],");

            fprintf(file, "%d", quantum);
            fprintf(file, ",\n");

            ptr = ptr->next;
        }

        /*
        fputs(buffer, file);
        fputs(",", file);
        fprintf(file, "%d", 4444);

        */

        // 2-
    }

    fclose(file);
}

/*
int main()
{

    char line[256];
    char test[26];

    strcpy(test, "4444");
    get_execution_id("analysis/config", line);
    printf("\n1- %s\n", line);
    set_execution_id("analysis/config", test);
    get_execution_id("analysis/config", line);
    printf("\n2- %s\n", line);
}
*/