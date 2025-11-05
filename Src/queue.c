#include "queue.h"

void QueueInit(Queue_t *q)
{
    q->getI = 0;
    q->putI = 0;
    q->lostData = 0;
    q->currentSize = 0;
}
