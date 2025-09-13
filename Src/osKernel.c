#include "Inc/osKernel.h"
#include "Inc/timeBase.h"
#include "Inc/led.h"

#define QUANTA		            10
#define NUM_OF_THREADS			3
#define STACKSIZE				400

typedef struct tcb{
    int32_t *stackPt;
    struct tcb *nextPt;
} tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelAddThreads(void (*task0)(void),void (*task1)(void),void (*task2)(void));
void osKernelLaunch();

void init_os_kernel(void (*task0)(void),void (*task1)(void),void (*task2)(void)){
    /*Add Threads*/
	osKernelAddThreads(task0,task1,task2);

	/*Set RoundRobin time quanta*/
	osKernelLaunch();
}

void stackInit(uint8_t i){
    TCB_STACK[i][STACKSIZE - 1] = (1U << 24); // PSR with T-bit set for Thumb mode

    TCB_STACK[i][STACKSIZE - 3] = 0xAAAAAAAA; // LR
    TCB_STACK[i][STACKSIZE - 4] = 0xAAAAAAAA; // R12
    
    TCB_STACK[i][STACKSIZE - 5] = 0xAAAAAAAA; // R3
    TCB_STACK[i][STACKSIZE - 6] = 0xAAAAAAAA; // R2
    TCB_STACK[i][STACKSIZE - 7] = 0xAAAAAAAA; // R1
    TCB_STACK[i][STACKSIZE - 8] = 0xAAAAAAAA; // R0

    TCB_STACK[i][STACKSIZE - 9] = 0xAAAAAAAA; // R11
    TCB_STACK[i][STACKSIZE - 10] = 0xAAAAAAAA; // R10
    TCB_STACK[i][STACKSIZE - 11] = 0xAAAAAAAA; // R9
    TCB_STACK[i][STACKSIZE - 12] = 0xAAAAAAAA; // R8
    TCB_STACK[i][STACKSIZE - 13] = 0xAAAAAAAA; // R7
    TCB_STACK[i][STACKSIZE - 14] = 0xAAAAAAAA; // R6
    TCB_STACK[i][STACKSIZE - 15] = 0xAAAAAAAA; // R5
    TCB_STACK[i][STACKSIZE - 16] = 0xAAAAAAAA; // R4
}

void addThread(void (*task)(void), tcbType *nextTcb,uint8_t i){
    stackInit(i);
    TCB_STACK[i][STACKSIZE - 2] = (int32_t)(task);   // Initial PC
    tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE - 16]; // Stack Pointer
    tcbs[i].nextPt = nextTcb;
}

void osKernelAddThreads(void (*task0)(void),void (*task1)(void),void (*task2)(void)){
    __asm("CPSID	I"); // Disable global interrupts
    
    addThread(task0, &tcbs[1], 0);
    addThread(task1, &tcbs[2], 1);
    addThread(task2, &tcbs[0], 2);
    currentPt = &tcbs[0];
    
    __asm("CPSIE	I");  // Enable global interrupts
}

void osKernelLaunch(){
    init_timebase(QUANTA);
    osSchedulerLaunch();
}

void osSchedulerLaunch(void){
    //disable global interrupts
    __asm("CPSID	I");
    
    //Load address of currentPt into R0
	__asm("LDR R0,=currentPt");
    
	//Load r2 from address equals r0,i.e r2 =currentPt
	__asm("LDR R1,[R0]");

    __asm("LDR R2,[R1]");
    
	//Load Cortex-M SP from address equals R2,i.e. SP = currentPt->stackPt
	__asm("MOV  SP, R2");

	//Restore r4,r5,r6,r7,r8,r9,r10,r11
	__asm("LDR R0, [SP,#28]\n"
          "MOV R11,R0     \n"
          "LDR R0,[SP,#24]\n"
          "MOV R10,R0     \n"
          "LDR R0, [SP,#20]\n"
          "MOV R9,R0     \n"
          "LDR R0, [SP,#16]\n"
          "MOV R8,R0     \n"
          "LDR R0, [SP,#12]\n"
          "MOV R7,R0     \n"
          "LDR R0, [SP,#8]\n"
          "MOV R6,R0     \n"
          "LDR R0, [SP,#4]\n"
          "MOV R5,R0     \n"
          "LDR R0, [SP,#0]\n"
          "MOV R4,R0     "); // Save remaining callee-saved registers

    __asm("ADD SP,SP,#32"); // free space

    __asm("ADD SP,SP,#16"); // skip R0-R3 temporally

	//Restore r12
	__asm("LDR R0,[SP,#0]\n"
          "MOV R12,R0");

    __asm("ADD SP,SP,#8"); // free space and skip LR

    __asm("LDR R0,[SP,#0]\n"
          "MOV LR,R0"); // PUT PC in LR

    __asm("SUB SP,SP,#24"); // back to R0-R3 position
    
    __asm("POP {R0-R3}"); // Restore r0,r1,r2,r3

    //Free space and Skip PSR
	__asm("ADD  SP,SP,#32");

	//Enable global interrupts
	__asm("CPSIE	I");
    
	//Return from exception
	__asm("BX	LR");

}

__attribute__((naked)) extern void isr_systick(void)
{
    //led_toggle();
    __asm("CPSID I"); // Disable interrupts globally
    
    __asm("SUB SP,SP,#32");// reserved space

    __asm("MOV R0,R11     \n"
          "STR R0, [SP,#28]\n"
          "MOV R0,R10     \n"
          "STR R0,[SP,#24]\n"
          "MOV R0,R9     \n"
          "STR R0, [SP,#20]\n"
          "MOV R0,R8     \n"
          "STR R0, [SP,#16]\n"
          "MOV R0,R7     \n"
          "STR R0, [SP,#12]\n"
          "MOV R0,R6     \n"
          "STR R0, [SP,#8]\n"
          "MOV R0,R5     \n"
          "STR R0, [SP,#4]\n"
          "MOV R0,R4     \n"
          "STR R0, [SP,#0]\n"); // Save remaining callee-saved registers
    
    __asm("LDR R0, =currentPt"); // Load address of currentPt into R0

    __asm("LDR R1,[R0]"); // Load r1 from address equals r0, i.e. r1 =currentPt

    __asm("MOV R2, SP"); 

    __asm("STR R2, [R1]"); // Store Cortex-M SP at address equals r1, i.e Save SP into tcb

    __asm("LDR R1,[R1,#4]"); // Load r1 from a location 4bytes above address r1, i.e r1 = currentPt->next

    __asm("STR R1,[R0]"); // Store r1 at address equals r0, i.e currentPt =  r1

    __asm("LDR R0,[R1]");

    __asm("MOV SP,R0"); // Load Cortex-M SP from address equals r1, i.e SP =  currentPt->stackPt

    __asm("LDR R0, [SP,#28]\n"
          "MOV R0,R11     \n"
          "LDR R0,[SP,#24]\n"
          "MOV R0,R10     \n"
          "LDR R0, [SP,#20]\n"
          "MOV R0,R9     \n"
          "LDR R0, [SP,#16]\n"
          "MOV R0,R8     \n"
          "LDR R0, [SP,#12]\n"
          "MOV R0,R7     \n"
          "LDR R0, [SP,#8]\n"
          "MOV R0,R6     \n"
          "LDR R0, [SP,#4]\n"
          "MOV R0,R5     \n"
          "LDR R0, [SP,#0]\n"
          "MOV R0,R4     "); // Save remaining callee-saved registers

    __asm("ADD SP,SP,#32"); // free space

    __asm("CPSIE I"); // Enable interrupts globally

    __asm("BX LR"); // Return from exception and restore r0,r1,r2,r3,r12,lr,pc,psr
} 
    