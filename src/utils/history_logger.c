#include "history_logger.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <direct.h>
#include <limits.h>
#include <windows.h>
#define PATH_SEP '\\'
#define make_dir(path) _mkdir(path)
#else
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEP '/'
#define make_dir(path) mkdir(path, 0777)
#endif

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static int get_executable_directory(char* buffer, size_t buffer_size)
{
#ifdef _WIN32
    DWORD length = GetModuleFileNameA(NULL, buffer, (DWORD)buffer_size);
    char* last_sep = NULL;

    if (length == 0 || (size_t)length >= buffer_size)
    {
        return 0;
    }

    last_sep = strrchr(buffer, PATH_SEP);
    if (last_sep == NULL)
    {
        return 0;
    }

    *last_sep = '\0';
    return 1;
#else
    ssize_t length = readlink("/proc/self/exe", buffer, buffer_size - 1);
    char* last_sep = NULL;

    if (length <= 0 || (size_t)length >= buffer_size)
    {
        return 0;
    }

    buffer[length] = '\0';
    last_sep = strrchr(buffer, PATH_SEP);
    if (last_sep == NULL)
    {
        return 0;
    }

    *last_sep = '\0';
    return 1;
#endif
}

static int directory_exists_or_created(const char* directory)
{
    if (make_dir(directory) == 0 || errno == EEXIST)
    {
        return 1;
    }

    return 0;
}

static FILE* open_csv_with_header(const char* filename)
{
    FILE* check_file = fopen(filename, "r");
    FILE* csv_file = NULL;
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

    csv_file = fopen(filename, "a");
    if (csv_file == NULL)
    {
        return NULL;
    }

    if (is_new)
    {
        fprintf(csv_file, "Algorithm/Array Type,Input Size,Execution Time (Seconds),Timestamp\n");
    }

    return csv_file;
}

static FILE* try_open_cwd_history_file(void)
{
    const char* filename = "output/benchmark_history.csv";

    if (!directory_exists_or_created("output"))
    {
        return NULL;
    }

    return open_csv_with_header(filename);
}

static FILE* try_open_exe_history_file(void)
{
    char exe_dir[PATH_MAX];
    char output_dir[PATH_MAX];
    char filename[PATH_MAX];
    int chars_written;

    if (!get_executable_directory(exe_dir, sizeof(exe_dir)))
    {
        return NULL;
    }

    chars_written = snprintf(output_dir, sizeof(output_dir), "%s%coutput", exe_dir, PATH_SEP);
    if (chars_written < 0 || (size_t)chars_written >= sizeof(output_dir))
    {
        return NULL;
    }

    if (!directory_exists_or_created(output_dir))
    {
        return NULL;
    }

    chars_written =
        snprintf(filename, sizeof(filename), "%s%cbenchmark_history.csv", output_dir, PATH_SEP);
    if (chars_written < 0 || (size_t)chars_written >= sizeof(filename))
    {
        return NULL;
    }

    return open_csv_with_header(filename);
}

static FILE* open_history_file(void)
{
    FILE* csv_file = try_open_cwd_history_file();
    if (csv_file != NULL)
    {
        return csv_file;
    }

    csv_file = try_open_exe_history_file();
    if (csv_file != NULL)
    {
        return csv_file;
    }

    printf("\nError: Unable to open or initialize local metrics log sheet inside output directory.\n");
    return NULL;
}

void add_to_history(const char* algo_name, int data_size, double execution_time)
{
    FILE* csv_file = open_history_file();
    time_t raw_time;
    struct tm* time_info;
    char timestamp[30];

    if (csv_file == NULL)
    {
        return;
    }

    time(&raw_time);
    time_info = localtime(&raw_time);
    if (time_info == NULL)
    {
        fclose(csv_file);
        return;
    }

    if (strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", time_info) == 0)
    {
        fclose(csv_file);
        return;
    }

    fprintf(csv_file, "%s,%d,%f,%s\n", algo_name, data_size, execution_time, timestamp);
    fclose(csv_file);
}
