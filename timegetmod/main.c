#include <linux/init.h>
#include <linux/module.h>
#include <linux/timex.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>

ssize_t mytimewrite(struct file *file, const char __user *buf,
					size_t count, loff_t *offset){
	unsigned long long cmd;
	cycles_t cycle_begin;
	cycles_t cycle_end;

	if(copy_from_user((unsigned long long *)&cmd, (unsigned long long *)buf, sizeof(unsigned long long)))
		return -EFAULT;

	if(cmd == 0){
		cycle_begin = get_cycles();
		msleep(1000);
		cycle_end = get_cycles();

		cmd = cycle_end - cycle_begin;
		if(copy_to_user((unsigned long long *)buf, (unsigned long long *)&cmd, sizeof(unsigned long long)))
			return -EFAULT;
	}

	return 0;
}

static struct proc_dir_entry *timeget;
static const struct file_operations timeget_fops = {
	.write	= mytimewrite,
};

int __init my_proc_init(void)
{
	struct proc_dir_entry *p;

	timeget = proc_mkdir("timeget", NULL);
	if(!timeget)
		goto out;

	p = proc_create("timeget/my_cycles", 0666, NULL, &timeget_fops);
	if(!p)
		goto out1;

	return 0;

out1:
	remove_proc_entry("timeget", NULL);
out:
	return -ENOMEM;
}

static void __init main_init(void)
{
	int err;
	printk("enter module main\n");
	err = my_proc_init();
	if(err)
		printk("create proc errorr!\n");
}


static int __exit main_exit(void)
{
	remove_proc_entry("timeget/my_cycles", NULL);
	remove_proc_entry("timeget", NULL);
	
	printk("leave module main\n");

	return 0;
}

module_init(main_init);
module_exit(main_exit);
MODULE_LICENSE("PGL");
