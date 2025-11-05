#ifndef KERNEL_CORE_H_
#define KERNEL_CORE_H_

#include "Configs.h"

void kernel_init(void);
void kernel_launch(uint32_t quanta);
void ThreadYield(void);

#endif
