#include "kernel_core.h"
#include "kernel_schedular.h"
#include "periodic_task.h"
#include "thread_manager.h"


Tcb_t Threads[MAX_THREAD_SIZE];
int32_t TCB_STACK[MAX_THREAD_SIZE][STACK_SIZE];
PTcb_t PeriodicTasks[NUM_OF_PERIODIC_THREADS];
Tcb_t *currentThread;

static uint32_t millis_prescaler;

extern void kernel_schedular_launch(void);
// Idle Thread ---> kullanici hic thread eklemediyse eklenir
static void idle_task(void)
{
    while(1)
    {
    	// Dusuk guc modu fonksiyonu cagirilinca degerler iyi okunmuyor!!!
        __asm("NOP");
    }
}

void kernel_init(void)
{
    millis_prescaler = (BUS_FREQ / 1000);
    periodic_task_setup(1000, 6); // 1MS
}



void kernel_launch(uint32_t quanta)
{
    if(get_current_thread_count() == 0)
        add_thread(idle_task, 255);

    SysTick->CTRL = 0;
    SysTick->VAL = 0;
    SysTick->LOAD = (quanta * millis_prescaler) - 1;

    NVIC_SetPriority(SysTick_IRQn, 7);
    SysTick->CTRL = 0x00000007;

    kernel_schedular_launch();
}
