#include "history_logger.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define access _access
#define F_OK 0
#else
#include <unistd.h>
#endif

void test_history_logger(void)
{
    const char* filename = "output/benchmark_history.csv";
    int found_algo_a = 0;
    int found_algo_b = 0;
    int header_count = 0;

    add_to_history("Test Algo A", 42, 0.001500);
    add_to_history("Test Algo B", 99, 1.234567);
    assert(access(filename, F_OK) == 0);

    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    char line[256];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (strstr(line, "Algorithm/Array Type") != NULL)
        {
            header_count++;
        }
        if (strstr(line, "Test Algo A") != NULL && strstr(line, "42") != NULL &&
            strstr(line, "0.001500") != NULL)
        {
            found_algo_a = 1;
        }
        if (strstr(line, "Test Algo B") != NULL && strstr(line, "99") != NULL &&
            strstr(line, "1.234567") != NULL)
        {
            found_algo_b = 1;
        }
    }

    fclose(file);

    assert(header_count == 1);
    assert(found_algo_a);
    assert(found_algo_b);

    printf("History logger tests passed successfully!\n");
}

int main(void)
{
    test_history_logger();
    return 0;
}
