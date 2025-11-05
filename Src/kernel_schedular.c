#include "kernel_schedular.h"
#include "thread_manager.h"

#if SCHEDULAR_TYPE == 0
// Round Robin Scheduler
void kernel_schedular(void)
{
    do
    {
        currentThread = currentThread->nextThread;
    } while(currentThread->state != THREAD_READY || currentThread->sleepTime > 0);
}

#else
// Priorty Schedular
void kernel_schedular(void)
{
    uint32_t current_size = get_current_thread_count();
    uint32_t highestPriorty = 255;
    Tcb_t *bestCase = currentThread;

    for(int i = 0; i < current_size; i++)
    {
        if(Threads[i].priorty < highestPriorty)
        {
            if(Threads[i].state != THREAD_BLOCKED && Threads[i].sleepTime == 0)
            {
                highestPriorty = Threads[i].priorty;
                bestCase = &Threads[i];
            }
        }
    }

    currentThread = bestCase;
}
#endif
