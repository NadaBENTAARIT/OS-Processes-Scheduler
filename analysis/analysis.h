#ifndef ANALYSIS_HEADER_FILE
#define ANALYSIS_HEADER_FILE

// Types definition

typedef struct CPU_Time_Node
{
    int time;
    struct CPU_Time_Node *next;

} CPU_Time_Node;

typedef CPU_Time_Node *CPU_Time_List;


typedef struct TrackingData
{
    char process_name[50];
    CPU_Time_List entry_cpu_time;
    CPU_Time_List exit_cpu_time;

} TrackingData;


typedef struct TrackingDataNode
{
    TrackingData data;
    struct TrackingDataNode *next;

} TrackingDataNode;

typedef TrackingDataNode *TDL; //TrackingDataList

/*
typedef struct AnalysisData
{
  
    int execution_id;
    char process_name[50];
    int arrival_time;
    int duration;
    int priority;
    char algorithm[100];
    CPU_Time_List entry_cpu_time;
    CPU_Time_List exit_cpu_time;
    int quantum;

} AnalysisData;
*/


#endif