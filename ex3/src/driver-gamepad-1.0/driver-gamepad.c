/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/device.h>

#include "efm32gg.h"

static int dev_open (struct inode *inode, struct file *filp);
static int dev_release (struct inode *inode, struct file *filp);
static ssize_t dev_read (struct file *filp, char *buff, size_t len, loff_t *off);


static struct file_operations my_fops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .open = dev_open,
    .release = dev_release
};

static struct cdev my_cdev;

/* Common GPIO interrupt handle */
irqreturn_t m_irq (int irq, void *dev_id, struct pt_regs *regs)
{
  *GPIO_IFC = 0xFF;
  printk("Interrupt!\n");

  return IRQ_HANDLED;
}

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

static int __init template_init(void)
{
  printk("Hello World!\n");

  /* Initialize char device */
  cdev_init (&my_cdev, &my_fops);
  cdev_add (&my_cdev, 0, 1);

  /* Configure GPIO interrupts */
  request_mem_region (GPIO_PA_BASE + 0x100, 28, "gamepad");
  *GPIO_EXTIPSELL = 0x222; /* Set external interrupts on PORT C */
  *GPIO_EXTIFALL = 0x7;   /* Enable interrupt on falling edge */
  release_mem_region (GPIO_PA_BASE + 0x100, 28);

  /* Release GPIO memory */
  request_mem_region (GPIO_PC_BASE, 32, "gamepad");

  /* Free IRQs, just in case */
  free_irq(17, NULL);
  free_irq(18, NULL);

  /* Register interrupt handler for odd and even GPIO pins */
  request_irq (17, (irq_handler_t)(m_irq), IRQF_DISABLED, "gamepad", (void *)(m_irq));
  request_irq (18, (irq_handler_t)(m_irq), IRQF_DISABLED, "gamepad", (void *)(m_irq));

  return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
  printk("Short life for a small module...\n");
  
  free_irq(1, NULL);
  release_mem_region (GPIO_PC_BASE, 32);
  cdev_del (&my_cdev);
}


module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");
