#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H

#include <stdint.h>

void OSKernelLaunch(uint32_t uiQuanta);
void OSKernelInit(void);
uint8_t ucOSKernelAddThreads( void (*vpTask0)(void),
															void (*vpTask1)(void),
															void (*vpTask2)(void));

void OSThreadYield(void);
	
#endif
