#include "osKernel.h"

#define QUANTA		10

uint32_t count0=0, count1=0, count2=0;

void Task0(void){
	while(1){
		count0++;
	}
}

void Task1(void){
	while(1){
		count1++;
	}
}

void Task2(void){
	while(1){
		count2++;
	}
}


int main(void){
	OSKernelInit();
	ucOSKernelAddThreads(&Task0,&Task1,&Task2);
	OSKernelLaunch(QUANTA);
	return 0;
}
