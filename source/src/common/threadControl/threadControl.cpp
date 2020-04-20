#include "threadControl.h"
#include "processor_temperature.h"

Thread _threadProcTemp;

threadInfo_t threadInfo[] = {
    // must be same sequence as the threadEnum_t, or implement a search func
    {threadProcTemp,            &_threadProcTemp,           procTempFunc},
};

void threadStart(threadEnum_t threadId) {
    printf("starting %d\r\n", threadId);
    threadInfo_t currThread = threadInfo[(uint8_t)threadId];
    currThread.ptr->start(currThread.func);
}

void threadStop(threadEnum_t threadId) {
    // threadStatusToggle(threadId, THREAD_STOP);
}

void threadStatus(threadEnum_t threadId) {
    threadInfo_t currThread = threadInfo[(uint8_t)threadId];
    printf("[thread] %d status:%d, currently running: %d\r\n", ThisThread::get_id(), (uint8_t)currThread.ptr->get_state(), currThread.ptr->gettid());
}
