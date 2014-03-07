#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "fsm.h"
#include "wish.h"

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{
  /* Clear timer 1 interrupt */
  *TIMER1_IFC = 1;

  //static int count = 0;

  /* Pass data to the DAC */
  //*DAC0_CH0DATA = wish[count++];

  /* Update LEDs */
  //*GPIO_PA_DOUT = count++;
  //*GPIO_PA_DOUT = 0x00FF; /* turn on LEDs D4-D8 (LEDs are active low) */
  //*GPIO_PA_DOUT = 0xFF00; /* turn on LEDs D4-D8 (LEDs are active low) */

  fsm_update();
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  /* Clear GPIO interrutps */
  *GPIO_IFC = 0xFF;
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  /* Clear GPIO interrutps */
  *GPIO_IFC = 0xFF;
}
