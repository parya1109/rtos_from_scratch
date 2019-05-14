

/********************************************************************************/
#include "osKernel.h"
/********************************************************************************/
#define NUM_OF_THREADS	3			//!< Number of Threads
#define STACK_SIZE			100		//!< Stack size

/********************************************************************************/
/* \brief Thread Control Block */
typedef struct stTCB{
	int32_t *ipStack;				//!< Stack pointer
	struct stTCB *stpNext;	//!< Next stack pointer
}stTCB_t;

stTCB_t stTCBs[NUM_OF_THREADS];		//!< Array of total TCBs 
stTCB_t *stpCurrent;							//!< Current pointer to TCB

int32_t iTCBStack[NUM_OF_THREADS][STACK_SIZE];	//!< Stack

/********************************************************************************/