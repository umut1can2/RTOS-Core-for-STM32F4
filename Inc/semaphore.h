#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include "Configs.h"

void SemaphoreInit(Semaphore *sem);
void SemaphoreGet(Semaphore *s);
void SemaphoreRelease(Semaphore *s);

#endif
