#include "periodic_task.h"
#include "thread_manager.h"

static uint32_t NumOfPeriodicThreads = 0;

uint8_t add_periodic_thread(void(*task)(void), uint32_t period)
{
    if(NumOfPeriodicThreads == NUM_OF_PERIODIC_THREADS || period == 0)
        return 0;

    PeriodicTasks[NumOfPeriodicThreads].task = task;
    PeriodicTasks[NumOfPeriodicThreads].period = period;
    PeriodicTasks[NumOfPeriodicThreads].TimeLeft = period - 1;
    NumOfPeriodicThreads++;
    return 1;
}

void periodic_task_setup(uint32_t freq, uint8_t priorty)
{
    __disable_irq();

    RCC->APB1ENR |= 1;
    TIM2->PSC = 16 - 1;
    TIM2->ARR = (1000000 / freq) - 1;
    TIM2->CR1 = 1;
    TIM2->DIER |= 1;

    NVIC_SetPriority(TIM2_IRQn, priorty);
    NVIC_EnableIRQ(TIM2_IRQn);

    __enable_irq();
}

void periodic_thread_execute(void)
{
    for(int i = 0; i < NumOfPeriodicThreads; i++)
    {
        if(PeriodicTasks[i].TimeLeft == 0)
        {
            PeriodicTasks[i].task();
            PeriodicTasks[i].TimeLeft = PeriodicTasks[i].period - 1;
        }
        else
        {
            PeriodicTasks[i].TimeLeft--;
        }
    }

    update_sleeping_threads();
}

void TIM2_IRQHandler(void)
{
    TIM2->SR = 0;
    periodic_thread_execute();
}
