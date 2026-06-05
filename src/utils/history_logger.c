#include "history_logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#define make_dir(path) _mkdir(path)
#else
#include <sys/stat.h>
#define make_dir(path) mkdir(path, 0777)
#endif

#define MAX_HISTORY_RECORDS 500

typedef struct
{
    char algo_name[100];
    int data_size;
    double execution_time;
} BenchmarkRecord;

static BenchmarkRecord history_records[MAX_HISTORY_RECORDS];
static int history_count = 0;

void add_to_history(const char* algo_name, int data_size, double execution_time)
{
    if (history_count < MAX_HISTORY_RECORDS)
    {
        strncpy(history_records[history_count].algo_name, algo_name, sizeof(history_records[history_count].algo_name) - 1);
        history_records[history_count].algo_name[sizeof(history_records[history_count].algo_name) - 1] = '\0';
        history_records[history_count].data_size = data_size;
        history_records[history_count].execution_time = execution_time;
        history_count++;
    }
}

void export_history_to_csv(void)
{
    if (history_count == 0)
    {
        printf("\n⚠️ No performance history records to export. Please profile some sorting or searching algorithms first!\n");
        return;
    }

    make_dir("output");

    const char* filename = "output/benchmark_history.csv";

    FILE* check_file = fopen(filename, "r");
    int is_new = 0;
    if (check_file == NULL)
    {
        is_new = 1;
    }
    else
    {
        fseek(check_file, 0, SEEK_END);
        if (ftell(check_file) == 0)
        {
            is_new = 1;
        }
        fclose(check_file);
    }

    FILE* csv_file = fopen(filename, "a");
    if (csv_file == NULL)
    {
        printf("\n⚠️ Error: Unable to open or initialize local metrics log sheet inside output directory.\n");
        return;
    }

    if (is_new)
    {
        fprintf(csv_file, "Algorithm/Array Type,Input Size,Execution Time (Seconds),Timestamp\n");
    }

    time_t raw_time;
    struct tm* time_info;
    char timestamp[30];
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info);

    for (int i = 0; i < history_count; i++)
    {
        fprintf(csv_file, "%s,%d,%f,%s\n",
                history_records[i].algo_name,
                history_records[i].data_size,
                history_records[i].execution_time,
                timestamp);
    }

    fclose(csv_file);
    printf("\n✅ Successfully exported %d metrics record(s) to %s\n", history_count, filename);

    history_count = 0;
}
