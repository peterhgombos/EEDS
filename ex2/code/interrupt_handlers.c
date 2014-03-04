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
  //*GPIO_PA_DOUT = 0x0F00; /* turn on LEDs D4-D8 (LEDs are active low) */
  pins++; /* turn on LEDs D4-D8 (LEDs are active low) */
  *TIMER1_IFC = 1;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  *GPIO_PA_DOUT = pins; /* turn on LEDs D4-D8 (LEDs are active low) */
  *GPIO_IFC = 0xFF; /* Clear interrupt flags */
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  //*GPIO_IFC = 0xFF; /* Clear interrupt flags */
}
