#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sysctl.h>
#include "sysctl-exam.h"

static char mystring[256];
static int myint;

static struct ctl_table my_sysctl_exam[] = {
    {
        .procname    = "myint",
        .data       = &myint,
        .maxlen     = sizeof(int),
        .mode       = 0666,
        .proc_handler   = &proc_dointvec,
    },
    {
        .procname    = "mystring",
        .data       = mystring,
        .maxlen     = MY_MAX_SIZE,
        .mode       = 0666,
        .proc_handler   = &proc_dostring,
    }
};

static struct ctl_table my_root = {
    .procname        = "mysysctl",
    .mode           = 0555,
    .child          = my_sysctl_exam,
};

static struct ctl_table_header * my_ctl_header;

static int __init sysctl_exam_init(void)
{
    my_ctl_header = register_sysctl_table(&my_root);

    return 0;
}

static void __exit sysctl_exam_exit(void)
{
    unregister_sysctl_table(my_ctl_header);
}

module_init(sysctl_exam_init);
module_exit(sysctl_exam_exit);
MODULE_LICENSE("GPL");
