#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

volatile uint16_t pins = 0;

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
  *GPIO_PA_DOUT = ~pins++;
  *TIMER1_IFC = 1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  *GPIO_IFC = 0xFF; /* Clear interrupt flags */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  *GPIO_IFC = 0xFF; /* Clear interrupt flags */
}
