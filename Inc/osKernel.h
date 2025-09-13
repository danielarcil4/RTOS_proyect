#ifndef __OSKERNEL_H__
#define __OSKERNEL_H__

void init_os_kernel(void (*task0)(void),void (*task1)(void),void (*task2)(void));
void osKernelLaunch();
void osSchedulerLaunch(void);
void osKernelAddThreads(void (*task0)(void),void (*task1)(void),void (*task2)(void));

#endif // __OSKERNEL_H__