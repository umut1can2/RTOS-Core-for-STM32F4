#ifndef MAILBOX_H_
#define MAILBOX_H_

#include "Configs.h"
#include "signal.h"

uint8_t MailSend(Signal *s, Queue_t *q, uint32_t data);
uint32_t MailReceive(Signal *s, Queue_t *q);

#endif
