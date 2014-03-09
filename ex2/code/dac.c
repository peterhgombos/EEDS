#include <stdint.h>
#include <stdbool.h>

#include "dac.h"
#include "efm32gg.h"

void dac_init (void)
{
  *DAC0_CTRL |= (1 << 18) || (1 << 17) || (1 << 16); /* Set prescaler to 2^7 */
  *DAC0_CTRL |= (1 << 3);               /* Set convmode to sample-hold */
  *DAC0_CTRL |= (1 << 4);               /* Set outmode to PIN */
}

void dac_enable (void)
{
  *CMU_HFPERCLKEN0 |= (1 << 17);        /* Enable clock to DAC */
  *DAC0_CH0CTRL |= (1 << 0);            /* Enable channel 0 */
  *DAC0_CH1CTRL |= (1 << 0);            /* Enable channel 1 */
}

void dac_disable (void)
{
  *DAC0_CH0DATA = 0;
  *DAC0_CH1DATA = 0;
  *DAC0_CH0CTRL &= ~(1 << 0);            /* Disable channel 0 */
  *DAC0_CH1CTRL &= ~(1 << 0);            /* Disable channel 1 */
  *CMU_HFPERCLKEN0 &= ~(1 << 17);        /* Enable clock to DAC */
}
