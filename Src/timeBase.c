#include "Inc/timebase.h"
#include "Inc/led.h"

#define MAX_DELAY         0x00FFFFFF
#define COUNT_1MS         125000

#define PPB_BASE_OFFSET       0xe0000000
#define SYSTICK_CSR_OFFSET    0x0000e010
#define SYSTICK_RVR_OFFSET    0x0000e014
#define SYSTICK_CVR_OFFSET    0x0000e018

#define SYSTICK_CSR      ( *(volatile uint32_t *) (PPB_BASE_OFFSET + SYSTICK_CSR_OFFSET) )
#define SYSTICK_RVR      ( *(volatile uint32_t *) (PPB_BASE_OFFSET + SYSTICK_RVR_OFFSET) )
#define SYSTICK_CVR      ( *(volatile uint32_t *) (PPB_BASE_OFFSET + SYSTICK_CVR_OFFSET) )

#define mCTRL_ENABLE     (1U <<  0)
#define mCTRL_TICKINT    (1U <<  1)
#define mCTRL_CLCKSRC    (1U <<  2)

volatile uint16_t counter = 0;

void init_timebase(uint32_t quanta) {
    // Disable SysTick timer
    SYSTICK_CSR = 0;
    // Set the reload value for 1 second
    SYSTICK_RVR = (quanta*COUNT_1MS) - 1;   
    // Clear the current value register
    SYSTICK_CVR = 0;
    //  Enable interrupt, and use processor clock
    SYSTICK_CSR = mCTRL_CLCKSRC | mCTRL_TICKINT | mCTRL_ENABLE; 
}

/*
// Handler of the Tick interrupt 
extern void isr_systick(void)
{
    __asm("CPSID I"); // Disable interrupts globally
    counter++; // Increment the counter
    __asm("CPSIE I"); // Enable interrupts globally
}
*/


