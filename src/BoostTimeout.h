#ifndef _NS_BOOST_TIMEOUT_H
#define _NS_BOOST_TIMEOUT_H

void boostTimeoutIncreaseCounter(void);
bool boostTimeoutReached(void);
void boostTimeoutReset(void);
void boostTimeoutStartCounter(void);

#endif