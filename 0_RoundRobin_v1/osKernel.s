				AREA |.text|,CODE,READONLY,ALIGN=2
				THUMB
				EXTERN stpCurrent
				EXPORT SysTick_Handler
				EXPORT OSSchedulerLaunch
				
SysTick_Handler					;Save R0,R1,R2,R3,R12,LR,PC,PSR
	CPSID		I
	PUSH		{R4-R11}		;Save R4,R5,R6,R7,R8,R9,R10,R11
	LDR			R0,=stpCurrent	;R0 points to current stack
	LDR			R1,[R0]			;R1 = stpCurrent
	STR			SP,[R1]			;stpCurrent = SP
	LDR			R1,[R1,#4]		;R1 = stpCurrent->next
	STR			R1,[R0]			;stpCurrent = R1 
	LDR			SP,[R1]			; SP = R1 = stpCurrent
	POP			{R4,R11}
	CPSIE		I
	BX			LR
	
OSSchedulerLaunch
	LDR			R0,=stpCurrent
	LDR			R2,[R0]
	LDR			SP,[R2]
	POP			{R4-R11}
	POP			{R0-R3}
	POP			{R12}
	ADD			SP,SP,#4
	POP			{LR}
	ADD			SP,SP,#4
	CPSIE		I
	BX			LR
	
	ALIGN
	END
	
	
		