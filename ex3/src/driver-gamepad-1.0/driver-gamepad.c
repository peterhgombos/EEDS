/*
 * This is a demo Linux kernel module.
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#include <asm/siginfo.h>
#include <asm/uaccess.h>

#include "efm32gg.h"

#define DEVICE_NAME "gamepad"

static struct class *cl;
static dev_t devno;
static int use_count;

static void __exit dev_exit(void);
static int __init dev_init(void);
static int dev_fasync (int fd, struct file *filp, int mode);
static int dev_open (struct inode *inode, struct file *filp);
static int dev_probe (struct platform_device *dev);
static ssize_t dev_read (struct file *filp, char *buff, size_t len, loff_t *off);
static int dev_release (struct inode *inode, struct file *filp);
static int dev_remove (struct platform_device *dev);
static inline void interrupt_enable (void);
static inline void interrupt_disable (void);

static const struct of_device_id dev_of_match[] = {
      { .compatible = "tdt4258", },
      { },
};
MODULE_DEVICE_TABLE (of, dev_of_match);

static struct platform_driver dev_driver = {
    .probe = dev_probe,
    .remove = dev_remove,
    .driver = {
        .name = "gamepad",
        .owner = THIS_MODULE,
        .of_match_table = dev_of_match,
    },
};

static struct file_operations f_ops = {
    .owner = THIS_MODULE,
    .read = dev_read,
    .open = dev_open,
    .release = dev_release,
    .fasync = dev_fasync,
};

static struct cdev c_dev;

/* Common GPIO interrupt handle */
irqreturn_t m_irq (int irq, void *dev_id, struct pt_regs *regs)
{
  /* Clear interrupt flags as soon as possible */
  *GPIO_IFC = 0xFF;
  printk("m_irq()\n");

  return IRQ_HANDLED;
}

static void __exit dev_exit(void)
{
  platform_driver_unregister (&dev_driver);
}

static int __init dev_init(void)
{
  return platform_driver_register (&dev_driver);
}

static int dev_fasync (int fd, struct file *filp, int mode)
{
  printk("dev_fasync()\n");
  return 0;
}

static int dev_open (struct inode *inode, struct file *filp)
{
  printk("dev_open()\n");
  if (use_count++) {
    interrupt_enable ();
  }

  return 0;
}

static int dev_probe (struct platform_device *pdev)
{
  int even_irq, odd_irq;
  int size;
  struct resource *res;

  printk("dev_probe()\n");

  /* Disable the interrupt before setup */
  interrupt_disable ();

  /* Set external interrupts on PORT C */
  *GPIO_EXTIPSELL = 0x22222222;
  *GPIO_EXTIFALL = 0xFF;

  /* Configure buttons on PORT C */
  *GPIO_PC_MODEL = 0x33333333;
  *GPIO_PC_DOUT = 0xFF;

  /* Register interrupt handler for odd and even GPIO pins */
  even_irq = platform_get_irq (pdev, 0);
  if (request_irq (even_irq, (irq_handler_t)(m_irq), IRQF_DISABLED, "gamepad", NULL)) {
      dev_err(&pdev->dev, "gamepad: cannot register IRQ %d\n", even_irq);
      goto err_even;
  }

  odd_irq = platform_get_irq (pdev, 1);
  if (request_irq (odd_irq, (irq_handler_t)(m_irq), IRQF_DISABLED, "gamepad", NULL)) {
      dev_err(&pdev->dev, "gamepad: cannot register IRQ %d\n", odd_irq);
      goto err_odd;
  }

  /* Request GPIO memory region */
  res = platform_get_resource (pdev, IORESOURCE_MEM, 0);
  size = resource_size (res);
  if (!request_mem_region (res->start, size, pdev->name)) {
      dev_err(&pdev->dev, "gamepad: cannot request GPIO mem region");
      goto err_res;
  }

  if (alloc_chrdev_region (&devno, 0, 1, "gamepad") != 0) {
      dev_err(&pdev->dev, "gamepad: cannot alloc char device region");
      goto err_chrdev;
  }

  if ((cl = class_create (THIS_MODULE, "gamepad")) == NULL) {
      dev_err(&pdev->dev, "gamepad: cannot create class");
      goto err_class;
  }

  if (device_create (cl, NULL, devno, NULL, "gamepad") == NULL) {
      dev_err(&pdev->dev, "gamepad: cannot create char device");
      goto err_create;
  }

  cdev_init (&c_dev, &f_ops);

  if (cdev_add (&c_dev, devno, 1) == -1) {
      dev_err(&pdev->dev, "gamepad: cannot add device");
      goto err_add;
  }
  
  /* Clear interrupts */
  *GPIO_IFC = 0xFF;

  interrupt_enable ();

  return 0;

err_add:
device_destroy (cl, devno);

err_create:
class_destroy (cl);

err_class:
unregister_chrdev_region (0, 1);

err_chrdev:
res = platform_get_resource (pdev, IORESOURCE_MEM, 0);
size = resource_size (res);
release_mem_region (res->start, size);

err_res:
odd_irq = platform_get_irq (pdev, 1);
free_irq (odd_irq, NULL);

err_odd:
even_irq = platform_get_irq (pdev, 1);
free_irq (even_irq, NULL);

err_even:
  return -1;
}

static ssize_t dev_read (struct file *filp, char *buff, size_t len, loff_t *off)
{
  put_user ('c', buff);

  return (ssize_t) 1;
}

static int dev_release (struct inode *inode, struct file *filp)
{
  printk("dev_release()\n");

  if (--use_count == 0) {
    interrupt_disable ();
  }

  return 0;
}

static int dev_remove (struct platform_device *pdev)
{
  int even_irq, odd_irq;
  int size;
  struct resource *res;

  dev_notice (&pdev->dev, "dev_remove()\n");

  interrupt_disable ();

  cdev_del (&c_dev);
  device_destroy (cl, devno);
  class_destroy (cl);

  /* Unregister char device region */
  unregister_chrdev_region (0, 1);

  /* Free IRQs */
  even_irq = platform_get_irq (pdev, 0);
  free_irq(even_irq, NULL);

  odd_irq = platform_get_irq (pdev, 1);
  free_irq(odd_irq, NULL);

  /* Release GPIO memory */
  res = platform_get_resource (pdev, IORESOURCE_MEM, 0);
  size = resource_size (res);
  release_mem_region (res->start, size);
  
  return 0;
}

static inline void interrupt_enable (void)
{
  *GPIO_IEN = 0xFF; /* Enable external interrupts */
}

static inline void interrupt_disable (void)
{
  *GPIO_IEN = 0x00; /* Disable external interrupts */
}


module_init(dev_init);
module_exit(dev_exit);

MODULE_DESCRIPTION("Driver for the TMA4258 gamepad");
MODULE_AUTHOR("Morten Mjelva");
MODULE_LICENSE("Dual BSD/GPL");
