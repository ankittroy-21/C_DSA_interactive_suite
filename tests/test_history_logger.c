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

    // 1. Ensure any existing file is deleted first so we test empty/creation logic
    remove(filename);

    // 2. Export with empty history should do nothing and file should not exist
    export_history_to_csv();
    assert(access(filename, F_OK) != 0);

    // 3. Add a history record
    add_to_history("Test Algo A", 42, 0.001500);
    
    // 4. Export it
    export_history_to_csv();

    // 5. Verify file exists now
    assert(access(filename, F_OK) == 0);

    // 6. Verify file contents (Header + 1 Record)
    FILE* file = fopen(filename, "r");
    assert(file != NULL);

    char line[256];
    // Check Header
    assert(fgets(line, sizeof(line), file) != NULL);
    assert(strstr(line, "Algorithm/Array Type") != NULL);
    assert(strstr(line, "Input Size") != NULL);
    assert(strstr(line, "Execution Time (Seconds)") != NULL);
    assert(strstr(line, "Timestamp") != NULL);

    // Check Record 1
    assert(fgets(line, sizeof(line), file) != NULL);
    assert(strstr(line, "Test Algo A") != NULL);
    assert(strstr(line, "42") != NULL);
    assert(strstr(line, "0.001500") != NULL);

    // Ensure EOF
    assert(fgets(line, sizeof(line), file) == NULL);
    fclose(file);

    // 7. Add another history record and export to check append (without writing header again)
    add_to_history("Test Algo B", 99, 1.234567);
    export_history_to_csv();

    file = fopen(filename, "r");
    assert(file != NULL);

    // Read Header
    assert(fgets(line, sizeof(line), file) != NULL);
    // Read Record 1
    assert(fgets(line, sizeof(line), file) != NULL);
    // Read Record 2
    assert(fgets(line, sizeof(line), file) != NULL);
    assert(strstr(line, "Test Algo B") != NULL);
    assert(strstr(line, "99") != NULL);
    assert(strstr(line, "1.234567") != NULL);

    // Ensure EOF
    assert(fgets(line, sizeof(line), file) == NULL);
    fclose(file);

    // Clean up test file
    remove(filename);

    printf("History logger tests passed successfully!\n");
}

int main(void)
{
    test_history_logger();
    return 0;
}
