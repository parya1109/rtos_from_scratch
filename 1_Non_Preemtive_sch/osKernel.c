/* \file 			osKernel.c
 * \brief			OS Kernel driver
 * \detail		This file contains function needed to create the RTOS kernel
 * \author		Prajapati,Parthiv
 * \version		1.0.0
 */
/********************************************************************************/
#include "stm32f4xx.h"                  // Device header
#include "osKernel.h"
/********************************************************************************/
#define SYSPRI3					(*((volatile uint32_t *)0xE000ED20))
#define INTCTRL					(*((volatile uint32_t *)0xE000ED04))
	
#define NUM_OF_THREADS	3			//!< Number of Threads
#define STACK_SIZE			100		//!< Stack size
#define SYS_FREQ				8000000	//!< System Freq
/********************************************************************************/

/* \brief Thread Control Block */
typedef struct stTCB{
	int32_t *ipStack;				//!< Stack pointer
	struct stTCB *stpNext;	//!< Next stack pointer
}stTCB_t;

stTCB_t stTCBs[NUM_OF_THREADS];		//!< Array of total TCBs 
stTCB_t *stpCurrent;							//!< Current pointer to TCB

int32_t iTCBStack[NUM_OF_THREADS][STACK_SIZE];	//!< Stack

uint32_t uiMillisPrescaler;
/********************************************************************************/
void OSSchedulerLaunch(void);

void OSKernelStackInit(int iThreadNumber){
	//!- Assign stack pointer of TCB to top of the stack 
	stTCBs[iThreadNumber].ipStack = &iTCBStack[iThreadNumber][STACK_SIZE-16];
	//!- Initialize PC (EPSR) resistor for thumb mode
	iTCBStack[iThreadNumber][STACK_SIZE-1] = 0x01000000;
	
}

uint8_t ucOSKernelAddThreads( void (*vpTask0)(void),void (*vpTask1)(void),void (*vpTask2)(void)){
	//!- Disable all interrupts
	__disable_irq();
	
	//!- Connect task in circuler 
	stTCBs[0].stpNext = &stTCBs[1];
	stTCBs[1].stpNext = &stTCBs[2];
	stTCBs[2].stpNext = &stTCBs[0];
	//!- Initialize OS kernel stack and load task address to PC
	OSKernelStackInit(0);
	iTCBStack[0][STACK_SIZE-2] = (int32_t)(vpTask0);
	OSKernelStackInit(1);
	iTCBStack[1][STACK_SIZE-2] = (int32_t)(vpTask1);
	OSKernelStackInit(2);
	iTCBStack[2][STACK_SIZE-2] = (int32_t)(vpTask2);
	
	stpCurrent = &stTCBs[0];
	//!- Enable all interrupts
	__enable_irq();
	return 1;
}

void OSKernelInit(void){
		//!- Disable all interrupts
	__disable_irq();
	
	//!- Calculate system prescaler for mSec
	uiMillisPrescaler = SYS_FREQ/1000;
	
}

void OSKernelLaunch(uint32_t uiQuanta){
	//!- Reset SysTick control and value registers
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	//!- Set SysTick priority to 7 (low)
	SYSPRI3 = (SYSPRI3&0x00FFFFFF)|0xE0000000;
	//!- Load SysTick with Quanta mSec
	SysTick->LOAD = (uiQuanta * uiMillisPrescaler)-1;
	//!- Start SysTick 
	SysTick->CTRL = 0x00000007;
	
	OSSchedulerLaunch();
	
}
	
void OSThreadYield(void){
	//!- Trigger system tick
	INTCTRL = 0x04000000;	
}


