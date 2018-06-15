#include <linux/kernel.h>
#include <linux/module.h>

extern int knlk_init(void);
extern void knlk_exit(void);

static __init int knetlink_module_init(void)
{
        printk("==== init knetlink module Finish![jfs:%ld]\n",jiffies);
        knlk_init();
        return 0;
}
module_init(knetlink_module_init);

static __exit void knetlink_module_exit(void)
{
        knlk_exit();
        printk("---- exit knetlink module Finish!\n");
        return ;
}
module_exit(knetlink_module_exit);

MODULE_LICENSE("GPL V2.0");
MODULE_DESCRIPTION("Linux Kernel netlink demo");
MODULE_AUTHOR("knetlink@build");
