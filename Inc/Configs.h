
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdint.h>
#include "stm32f411xe.h"

#define MAX_THREAD_SIZE 10
#define NUM_OF_PERIODIC_THREADS 10
#define STACK_SIZE 512
#define FIFO_SIZE 16

#ifndef BUS_FREQ
	#define BUS_FREQ 16000000 // 16MHz Standart dahili osilator
#endif

/* Scheduler Tipi
 *  0=Round Robin
 * 1=Priority
 * */
#ifndef SCHEDULAR_TYPE
	#define SCHEDULAR_TYPE 0
#endif

#define NULL (void*)0

typedef enum
{
    THREAD_EXECUTING,
    THREAD_READY,
    THREAD_BLOCKED,
    THREAD_SLEEP
} ThreadState;

typedef struct tcb
{
    int32_t *stackPointer;
    struct tcb *nextThread;
    ThreadState state;
    uint32_t sleepTime;
    uint8_t priorty;
} Tcb_t;

typedef struct ptcb
{
    void (*task)(void);
    uint32_t period;
    uint32_t TimeLeft;
} PTcb_t;

typedef struct queue
{
    uint32_t arr[FIFO_SIZE];
    uint32_t putI;
    uint32_t getI;
    uint32_t currentSize;
    uint32_t lostData;
} Queue_t;

typedef struct
{
    Tcb_t *owner;
    Tcb_t *waitingList[MAX_THREAD_SIZE];
    uint32_t waitingCount;
} Semaphore;

typedef struct
{
    uint32_t state;
    uint32_t waitingCount;
    Tcb_t* waitingList[MAX_THREAD_SIZE];
} Signal;

extern Tcb_t Threads[MAX_THREAD_SIZE];
extern int32_t TCB_STACK[MAX_THREAD_SIZE][STACK_SIZE];
extern PTcb_t PeriodicTasks[NUM_OF_PERIODIC_THREADS];
extern Tcb_t *currentThread;

#endif
