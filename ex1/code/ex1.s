.syntax unified

.include "efm32gg.s"

/****************************************************************************
 *                                                                          *
 * Exception vector table                                                   *
 * This table contains addresses for all exception handlers                 *
 *                                                                          *
 ***************************************************************************/

.section .vectors

.long   stack_top               /* Top of Stack                 */
.long   _reset                  /* Reset Handler                */
.long   dummy_handler           /* NMI Handler                  */
.long   dummy_handler           /* Hard Fault Handler           */
.long   dummy_handler           /* MPU Fault Handler            */
.long   dummy_handler           /* Bus Fault Handler            */
.long   dummy_handler           /* Usage Fault Handler          */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* SVCall Handler               */
.long   dummy_handler           /* Debug Monitor Handler        */
.long   dummy_handler           /* Reserved                     */
.long   dummy_handler           /* PendSV Handler               */
.long   dummy_handler           /* SysTick Handler              */

/* External Interrupts */
.long   dummy_handler
.long   gpio_handler            /* GPIO even handler */
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   gpio_handler            /* GPIO odd handler */
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler
.long   dummy_handler

.section .text

/****************************************************************************
 *                                                                          *
 * Reset handler                                                            *
 * The CPU will start executing here after a reset                          *
 *                                                                          *
 ***************************************************************************/

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
    /* Enable clock to GPIO module */
    ldr r1, =CMU_BASE                           /* Load the CMU register base address                                   */
    ldr r2, [r1, #CMU_HFPERCLKEN0]              /* Load the value of High Frequency Peripheral Clock Enable Register 0  */
    mov r3, #1
    lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
    orr r2, r2, r3                              /* Enable the GPIO bit in the CMU_HFPERCLKEN0 register                  */
    str r2, [r1, #CMU_HFPERCLKEN0]              /* Write the new value to the register                                  */


    /* Configure LED pins */
    ldr r1, =GPIO_PA_BASE                       /* Load the PORT A register base address                                */

    mov r2, #2
    str r2, [r1, #GPIO_CTRL]                    /* Write 2 to the PORT A Control Register to set high drive strength    */

    mov r2, #0xFFFFFFFF
    str r2, [r1, #GPIO_DOUT]                    /* Output pins are output low, so we set them high to disable the LEDs  */


    /* Configure input button pins */
    ldr r3, =GPIO_PC_BASE                       /* Load the PORT C register base address                                */
    mov r2, #0x33333333                         /* Set pins 0-7 on PORT C as input                                      */
    str r2, [r3, #GPIO_MODEL]

    mov r2, #0xff                               /* Load enable flag                                                     */
    str r2, [r3, #GPIO_DOUT]                    /* Enable internal pull-up on pins 0-7 on PORT C                        */

    
/* Configure pin change interrupts on PORT C */
ldr r1, =GPIO_BASE            /* Load GPIO register base address */
mov r2, #0x22222222           /* Load enable bit for all PORTC pins */
str r2, [r1, #GPIO_EXTIPSELL] /* Write to External Interrupt Port Select Low Register */

mov r2, #0xff                 /* Load enable flag */
str r2, [r1, #GPIO_EXTIFALL]  /* Enable hi->lo pin change interrupt */
str r2, [r1, #GPIO_EXTIRISE]  /* Enable lo->hi pin change interrupt */
str r2, [r1, #GPIO_IEN]       /* Enable external interrupt generation */

ldr r1, =ISER0                /* Load Interrupt Set Enable Register 0 address */
ldr r2, =#0x802               /* Load GPIO_EVEN and GPIO_ODD bits */
str r2, [r1, #0]              /* Write new value to register */

/* Enable energy mode 3 */
ldr r1, =SCR
mov r2, #7
str r2, [r1, #0]

    /* Main program loop */
    LOOP:
        /* Disable PORT A pins output in order to save power. This has to be done every time. */
        mov r2, #0
        ldr r1, =GPIO_PA_BASE
        str r2, [r1, #GPIO_MODEH]

        /* Put controller to sleep */
        wfi                                     /* Wait for instruction */

        b LOOP                                  /* Next iteration       */

/****************************************************************************
 *                                                                          *
 * GPIO handler                                                             *
 * The CPU will jump here when there is a GPIO interrupt                    *
 *                                                                          *
****************************************************************************/

.thumb_func
gpio_handler:  
    /* Clear interrupt flag. This is done to prevent interrupts from being interrupted. */
    ldr r1, =GPIO_BASE          /* Load GPIO register base address      */
    mov r2, #0xff               /* Load clear flag              */
    str r2, [r1, #GPIO_IFC]     /* Set GPIO Interrupt Flag Clear Register */

    /* Set pins 8-15 on PORT A as output */
    ldr r1, =GPIO_PA_BASE
    mov r2, #0x55555555
    str r2, [r1, #GPIO_MODEH]

    /* Read button values and set LEDs accordingly */
    ldr r1, =GPIO_PA_BASE       /* Load PORT A register base address    */
    ldr r2, =GPIO_PC_BASE       /* Load PORT C register base address    */
    ldr r3, [r2, #GPIO_DIN]     /* Load button values                   */
    lsl r3, r3, #8              /* Shift button values values so they can be written directly the the LED register  */
    str r3, [r1, #GPIO_DOUT]    /* Write to LED register                */

    bx lr                       /* Return from interrupt            */
    

/***************************************************************************/

.thumb_func
dummy_handler:  
b .  // do nothing

