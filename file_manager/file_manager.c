#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "file_manager_functions.h"

int verify_line(char *line)
{
    // A line is going to be skipped in the floowing cases
    // - Line begins with #
    // - Line is not well formatted

    int res;
    int ascii;

    res = 1;
    // Check if the line is a comment or an empty line
    if (line[0] == '#' || line[0] == '\n')
    {
        res = 0;
    }
    else
    {
        // Check if the last character is a ","or not
        ascii = *(line + (strlen(line) - 1));
        if (ascii == 10)
        {
            ascii = *(line + (strlen(line) - 2));
            if (ascii != 44)
            {
                *(line + (strlen(line) - 1)) = ',';
            }
        }
        else
        {
            if (ascii != 44)
            {
                strcat(line, ",");
            }
        }
    }

    return res;
}

ProcessData extract_process_data(char *line)
{
    char field[50];
    int n = 0;
    ProcessData pd;

    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] != ',' && line[i] != '\n' && line[i] != '\0')
        {
            strncat(field, &line[i], 1);
        }
        else
        {
            switch (n)
            {
            case 0:
                // printf("process name\n");
                strcpy(pd.process_name, field);
                break;

            case 1:
                // printf("Arrival time\n");
                pd.arrival_time = atoi(field);
                pd.backup.arrival_time = atoi(field);
                break;

            case 2:
                // printf("Duration\n");
                pd.duration = atoi(field);
                pd.backup.duration = atoi(field);
                break;

            case 3:
                // printf("Priority\n");
                pd.priority = atoi(field);
                break;

            default:
                break;
            }

            n++;
            field[0] = '\0';
        }
    }

    // print_process_data(pd);
    return pd;
}

PL parse_file(char *fileName)
{
    int line_verification_status;

    FILE *file = fopen(fileName, "r"); /* should check the result */
    char line[256];

    ProcessData pd;
    PL pl;

    if (!file)
    {
        printf("\n Unable to open : %s ", fileName);
        return NULL;
    }

    // Init the process linked list
    pl = NULL;

    while (fgets(line, sizeof(line), file))
    {
        // Skip comments and empty lines
        line_verification_status = verify_line(line);
        if (line_verification_status == 0)
        {
            continue;
        }

        // Extract the process data
        pd = extract_process_data(line);
        add_process(&pl, pd);
    }

    fclose(file);

    return pl;
}

void pl_sort(PL pl)
{
    PL ptri, ptrj, minptr;
    ProcessData *inter;

    inter = (ProcessData *)malloc(sizeof(ProcessData));
    ptri = pl;

    while (ptri->next != NULL)
    {
        minptr = ptri;
        ptrj = ptri->next;
        while (ptrj != NULL)
        {
            if (minptr->pd.arrival_time > ptrj->pd.arrival_time)
            {
                minptr = ptrj;
            }

            ptrj = ptrj->next;
        }

        if (ptri != minptr)
        {
            *inter = ptri->pd;
            ptri->pd = minptr->pd;
            minptr->pd = *inter;
        }

        ptri = ptri->next;
    }

    free(inter);
}

/*
int main(int argc, char* argv[])
{
    // char const* const fileName = argv[1];

    char* fileName = "tests/config/config2";
    PL pl;

    pl = parse_file(fileName);
    printf("befor sort: \n");
    print_process_list(pl);

    pl_sort(pl);
    printf("after sort: \n");
    print_process_list(pl);
    

    return 0;
}
*/