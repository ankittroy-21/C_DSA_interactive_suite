#ifndef HISTORY_LOGGER_H
#define HISTORY_LOGGER_H

void add_to_history(const char* algo_name, int data_size, double execution_time);
void export_history_to_csv(void);

#endif
