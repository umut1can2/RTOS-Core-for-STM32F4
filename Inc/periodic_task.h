#ifndef PERIODIC_TASK_H_
#define PERIODIC_TASK_H_

#include "Configs.h"

uint8_t add_periodic_thread(void(*task)(void), uint32_t period);
void periodic_task_setup(uint32_t freq, uint8_t priorty);
void periodic_thread_execute(void);

#endif
