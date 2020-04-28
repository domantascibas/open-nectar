#ifndef _NS_THREAD_MANAGER_H
#define _NS_THREAD_MANAGER_H

#include "mbed.h"

#define THREAD_STOP         0
#define THREAD_START        1

typedef enum {
    threadProcTemp,
    threadBoilerTemp,
} threadEnum_t;

typedef struct {
    threadEnum_t id;
    Thread *ptr;
    void (*func)();
} threadInfo_t;

void threadStart(threadEnum_t threadId);
void threadStop(threadEnum_t threadId);
void threadStatus(threadEnum_t threadId);

#endif
