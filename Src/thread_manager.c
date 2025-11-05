#include "thread_manager.h"
static uint32_t current_thread_size = 0;
extern void ThreadYield(void);


void kernel_init_stack(int i)
{
    Threads[i].stackPointer = &TCB_STACK[i][STACK_SIZE - 16];

    /* Interrupt geldiginde cpu;nun pushladigi registerlar  */
    TCB_STACK[i][STACK_SIZE - 1] = (0x1 << 24);  // xPSR - Thumb mode
    // -2 is PC
    TCB_STACK[i][STACK_SIZE - 3] = 0x14141414;   // R14 (LR)
    TCB_STACK[i][STACK_SIZE - 4] = 0x12121212;   // R12
    TCB_STACK[i][STACK_SIZE - 5] = 0x03030303;   // R3
    TCB_STACK[i][STACK_SIZE - 6] = 0x02020202;   // R2
    TCB_STACK[i][STACK_SIZE - 7] = 0x01010101;   // R1
    TCB_STACK[i][STACK_SIZE - 8] = 0x00000000;   // R0

    /* Geri kalanlar RTOS un denetiminde */
    TCB_STACK[i][STACK_SIZE - 9]  = 0x11111111;  // R11
    TCB_STACK[i][STACK_SIZE - 10] = 0x10101010;  // R10
    TCB_STACK[i][STACK_SIZE - 11] = 0x09090909;  // R9
    TCB_STACK[i][STACK_SIZE - 12] = 0x08080808;  // R8
    TCB_STACK[i][STACK_SIZE - 13] = 0x07070707;  // R7
    TCB_STACK[i][STACK_SIZE - 14] = 0x06060606;  // R6
    TCB_STACK[i][STACK_SIZE - 15] = 0x05050505;  // R5
    TCB_STACK[i][STACK_SIZE - 16] = 0x04040404;  // R4
}


uint32_t get_current_thread_count(void)
{
    return current_thread_size;
}

uint8_t add_thread(void (*task)(void), uint8_t priorty)
{
    if(current_thread_size >= MAX_THREAD_SIZE)
    {
        return 0;
    }

    __disable_irq();

    kernel_init_stack(current_thread_size);
    // PC degerini burada guncelliyoruz
    // Normalde or (0x1) olayi donuyor burada kontrol et
    TCB_STACK[current_thread_size][STACK_SIZE - 2] = (uint32_t)task;
    Threads[current_thread_size].sleepTime = 0;
    Threads[current_thread_size].priorty = priorty;

    if(current_thread_size == 0)
    {
        currentThread = &Threads[0];
        Threads[0].nextThread = &Threads[0];
    }
    else
    {
        int i = 0;
        for(; i < current_thread_size; i++)
        {
            Threads[i].nextThread = &Threads[i + 1];
        }
        Threads[i].nextThread = &Threads[0];
    }

    Threads[current_thread_size].state = THREAD_READY;
    current_thread_size++;

    __enable_irq();
    return 1;
}

void ThreadSleep(uint32_t sleep)
{
    __disable_irq();
    currentThread->sleepTime = sleep;
    currentThread->state = THREAD_SLEEP;
    __enable_irq();

    ThreadYield();
}

/*
 * Periodic thread iicn kullaniliyor
*/
void update_sleeping_threads(void)
{
    for(int i = 0; i < current_thread_size; i++)
    {
        if(Threads[i].sleepTime > 0)
        {
            Threads[i].sleepTime--;
        }
        else
        {
            if(Threads[i].state == THREAD_SLEEP)
                Threads[i].state = THREAD_READY;
        }
    }
}
