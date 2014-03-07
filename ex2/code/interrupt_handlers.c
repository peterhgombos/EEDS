#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#include "buttons.h"
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

  fsm_update();
}

/* TIMER2 interrupt handler */
void __attribute__ ((interrupt)) TIMER2_IRQHandler()
{
  /* Clear timer 1 interrupt */
  *TIMER2_IFC = 1;
  buttons_timer_irq();
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
  /* Clear GPIO interrupts */
  *GPIO_IFC = 0xFF;
  buttons_gpio_irq();
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
  /* Clear GPIO interrupts */
  *GPIO_IFC = 0xFF;
  buttons_gpio_irq();
}
