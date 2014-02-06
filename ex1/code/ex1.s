.syntax unified

.include "efm32gg.s"

/////////////////////////////////////////////////////////////////////////////
//
// Exception vector table
// This table contains addresses for all exception handlers
//
/////////////////////////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////////////////////////
//
// Reset handler
// The CPU will start executing here after a reset
//
/////////////////////////////////////////////////////////////////////////////

.globl  _reset
.type   _reset, %function
.thumb_func
_reset:
    // Load  CMU clock
    // Load CMU base adress
    ldr r1, =CMU_BASE

    // Load current value of HFPERCLK ENABLE
    ldr r2, [r1, #CMU_HFPERCLKEN0]

    // Set bit for GPIO clk
    mov r3, #1
    lsl r3, r3, #CMU_HFPERCLKEN0_GPIO

    orr r2, r2, r3
    
    // Store new value
    str r2, [r1, #CMU_HFPERCLKEN0]


    // Configure LED pins
        // Get PORT A base address
        ldr r1, =GPIO_PA_BASE

        // Set high drive strength on PORT A
        mov r2, #2
        str r2, [r1, #GPIO_CTRL]


        // Turn LEDs off
        mov r2, #0xffffffff
        str r2, [r1, #GPIO_DOUT]

    // Configure input button pins
        // Get PORT C base address
        ldr r3, =GPIO_PC_BASE

        // Set PORT C pins as input
        mov r2, #0x33333333
        str r2, [r3, #GPIO_MODEL]

        // Enable internal pull up
        mov r2, #0xff
        str r2, [r3, #GPIO_DOUT]
    
    // Configure pin interrupts
        // Configure interrupt source pins
        ldr r1, =GPIO_BASE
        mov r2, #0x22222222
        str r2, [r1, #GPIO_EXTIPSELL]

        mov r2, #0xff

        // Enable high to low level interrupt
        str r2, [r1, #GPIO_EXTIFALL]

        // Enable low to high level interrupt
        str r2, [r1, #GPIO_EXTIRISE]

        // Enable interrupt generation
        str r2, [r1, #GPIO_IEN]

        // Enable interrupt handling
        ldr r1, =ISER0
        ldr r2, =#0x802
        str r2, [r1, #0]

    ldr r1, =SCR
    mov r2, #6
    str r2, [r1, #0]

    LOOP:
        // Disable PORT A pins output
        ldr r1, =GPIO_PA_BASE
        mov r2, #0
        str r2, [r1, #GPIO_MODEH]
        // Sleep
        ldr r1, =CMU_BASE
        str r2, [r1, #CMU_LFACLKEN0]
        str r2, [r1, #CMU_LFBCLKEN0]
        wfi
        b LOOP

/////////////////////////////////////////////////////////////////////////////
//
// GPIO handler
// The CPU will jump here when there is a GPIO interrupt
// ///////////////////////////////////////////////////////////////////////////// 
.thumb_func
gpio_handler:  
    // Clear interrupt flag
    ldr r1, =GPIO_BASE
    ldr r2, [r1, #GPIO_IF]
    str r2, [r1, #GPIO_IFC]

    // Enable PORT A pins output
    ldr r1, =GPIO_PA_BASE
    mov r2, #0x55555555
    str r2, [r1, #GPIO_MODEH]

    // Write button values to LEDs
        // Load input and output registers
        ldr r1, =GPIO_PA_BASE
        ldr r2, =GPIO_PC_BASE
        
        // Copy input values to output
        ldr r3, [r2, #GPIO_DIN]
        lsl r3, r3, #8
        str r3, [r1, #GPIO_DOUT]

    // Return from interrupt
    bx lr
    

/////////////////////////////////////////////////////////////////////////////

.thumb_func
dummy_handler:  
b .  // do nothing

