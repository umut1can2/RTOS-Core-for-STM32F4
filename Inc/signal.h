#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "Configs.h"

void SignalInit(Signal *s, uint32_t state);
void SignalSet(Signal *s);
void SignalWait(Signal *s);
void SignalWaitAndClear(Signal *s);

#endif
