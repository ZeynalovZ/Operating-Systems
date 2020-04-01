#include <linux/init.h>
#include <linux/module.h>
#include <linux/init_task.h>
#include <linux/sched.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zeynal Zeynalov");

static int __init md_init(void)
{
    struct task_struct *task = &init_task;

    do
    {
        printk(KERN_INFO "___%s - %d, parent = %s - %d\n", task->comm, task->pid, task->parent->comm, task->parent->pid);
    } while ((task = next_task(task)) != &init_task);
    
    printk(KERN_INFO "___ Current %s = %d, parent = %s = %d", current->comm, current->pid, current->parent->comm, current->parent->pid);

}

static void __exit md_exit(void)
{
    printk("+ module md2 unloaded\n");
}



module_init(md_init);
module_exit(md_exit);
