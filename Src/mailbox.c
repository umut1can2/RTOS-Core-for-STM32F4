#include "mailbox.h"

uint8_t MailSend(Signal *s, Queue_t *q, uint32_t data)
{
    uint8_t res = 0;

    __disable_irq();

    if(q->currentSize == FIFO_SIZE)
    {
        q->lostData++;
        res = 0;
    }
    else
    {
        q->arr[q->putI] = data;
        q->putI = (q->putI + 1) % FIFO_SIZE;
        q->currentSize++;
        res = 1;
    }

    __enable_irq();

    if(res)
        SignalSet(s);

    return res;
}

uint32_t MailReceive(Signal *s, Queue_t *q)
{
    uint32_t data;

    SignalWait(s);
    __disable_irq();

    if(q->currentSize == 0)
    {
        s->state = 0;
        __enable_irq();
        return 0;
    }

    data = q->arr[q->getI];
    q->getI = (q->getI + 1) % FIFO_SIZE;
    q->currentSize--;

    if(q->currentSize == 0)
    {
        s->state = 0;
    }

    __enable_irq();

    return data;
}
