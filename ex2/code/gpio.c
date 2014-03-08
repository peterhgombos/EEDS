#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

/* function to set up GPIO mode and interrupts*/
void gpio_init (void)
{
  *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_GPIO; /* enable GPIO clock*/

  /* TODO set input and output pins for the joystick */
  *GPIO_EXTIPSELL = 0x22222222; /* Set external interrupts on PORT C */
  *GPIO_EXTIFALL = 0xFF;

  /* Configure buttons on PORT C */
  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xFF; /* turn on buttons */
    
  *GPIO_PA_CTRL = 2;  /* set high drive strength */
  *GPIO_PA_MODEH = 0x55555555; /* set pins A8-15 as output */
  *GPIO_PA_DOUT = 0xFFFFF; /* turn on LEDs D4-D8 (LEDs are active low) */

  *GPIO_IFC = 0xFF; /* Clear interrupts before enabling */
  *GPIO_IEN = 0xFF; /* Enable all external interrupts */
}
