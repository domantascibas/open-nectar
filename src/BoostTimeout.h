#ifndef _BOOST_TIMEOUT_H
#define _BOOST_TIMEOUT_H

// Boost timeout. Turns off boost mode in case it can't reach 70C in certain amount of time
static const uint8_t BOOST_TIMEOUT = 180;

void boostTimeoutIncreaseCounter(void);
bool boostTimeoutReached(void);
void boostTimeoutReset(void);
void boostTimeoutStartCounter(void);

#endif