#ifndef OS_H
#define OS_H

typedef int os_intstatus_t;

/* Mock Windows service macros */
static inline os_intstatus_t OS_WINDOWSGoingToUseWindowsService(void) { return 0; }
static inline void OS_WINDOWSFinishedUsingWindowsService(os_intstatus_t val) { (void)val; }

#endif /* OS_H */
