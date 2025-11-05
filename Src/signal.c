#include "signal.h"

extern void ThreadYield(void);

void SignalInit(Signal *s, uint32_t state)
{
    s->state = state;
    s->waitingCount = 0;
    for(int i = 0; i < MAX_THREAD_SIZE; i++)
        s->waitingList[i] = NULL;
}

void SignalSet(Signal *s)
{
    __disable_irq();
    s->state = 1;

    if(s->waitingCount <= 0)
    {
        __enable_irq();
        return;
    }

    for(int i = 0; i < s->waitingCount; i++)
    {
        s->waitingList[i]->state = THREAD_READY;
        s->waitingList[i] = NULL;
    }

    s->waitingCount = 0;
    ThreadYield();
    __enable_irq();
}

void SignalWait(Signal *s)
{
    __disable_irq();

    if(s->state == 1)
    {
        __enable_irq();
        return;
    }

    s->waitingList[s->waitingCount++] = currentThread;
    currentThread->state = THREAD_BLOCKED;

    ThreadYield();
    __enable_irq();
}

void SignalWaitAndClear(Signal *s)
{
    __disable_irq();

    if(s->state == 1)
    {
        s->state = 0;
        __enable_irq();
        return;
    }

    s->waitingList[s->waitingCount++] = currentThread;
    currentThread->state = THREAD_BLOCKED;

    ThreadYield();
    __enable_irq();
}
