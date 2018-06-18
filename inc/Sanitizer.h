#ifndef _SANITIZER_H
#define _SANITIZER_H

static const uint16_t LEGIONELLA_PREVENTION_PERIOD = 5;

void sanitizerIncreaseCounter(void);
void sanitizerTurnOn(bool turn_on);

#endif