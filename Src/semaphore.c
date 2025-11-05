#include "semaphore.h"

extern void ThreadYield(void);

void SemaphoreInit(Semaphore *sem)
{
    sem->owner = NULL;
    sem->waitingCount = 0;
    for (int i = 0; i < MAX_THREAD_SIZE; i++)
        sem->waitingList[i] = NULL;
}

void SemaphoreGet(Semaphore *s)
{
    __disable_irq();

    // Semaphore un bos ise currentThread semaphore u aliyor
    if(s->owner == NULL)
    {
        s->owner = currentThread;
        __enable_irq();
        return;
    }

    // Zaten semaphore kendisindeyse islemine devam ediyor
    if(s->owner == currentThread)
    {
        __enable_irq();
        return;
    }

    s->waitingList[s->waitingCount++] = currentThread;
    currentThread->state = THREAD_BLOCKED;

    __enable_irq();
    ThreadYield();
}

void SemaphoreRelease(Semaphore *s)
{
    __disable_irq();

    // Semaphore u sadece alan thread geri birakabilir!!!
    if (s->owner != NULL && s->owner != currentThread)
    {
        __enable_irq();
        return;
    }

    if(s->waitingCount <= 0)
    {
        s->owner = NULL;
        __enable_irq();
        return;
    }
    s->owner = s->waitingList[0];
    s->waitingList[0]->state = THREAD_READY;

    for(int i = 0; i < s->waitingCount - 1; i++)
        s->waitingList[i] = s->waitingList[i + 1];

    s->waitingList[s->waitingCount - 1] = NULL;
    s->waitingCount--;

    ThreadYield();
    __enable_irq();
}
