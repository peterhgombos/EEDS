#include <stdint.h>
#include <stdbool.h>
#include "wish.h"

#include "efm32gg.h"

/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{

  /* Clear timer 1 interrupt */
  *TIMER1_IFC = 1;

  static int count = 0;

  /* Pass data to the DAC */
  *DAC0_CH0DATA = wish[count++];

  /* Update LEDs */
  *GPIO_PA_DOUT = count;
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
