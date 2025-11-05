#ifndef THREAD_MANAGER_H_
#define THREAD_MANAGER_H_

#include "Configs.h"

uint8_t add_thread(void (*task)(void), uint8_t priorty);
uint32_t get_current_thread_count(void);
void ThreadSleep(uint32_t sleep);
void update_sleeping_threads(void);

#endif
