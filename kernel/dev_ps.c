#include <linux/init.h> //for __init and __exit macros
#include <linux/module.h> // for init_module and cleanup_module prototypes 
#include <linux/fs.h>// for file_operations struct
//#include <linux/kernel.h>
#include <linux/miscdevice.h>// for miscdevice struct
#include <linux/uaccess.h>// for copy_to_user
MODULE_LICENSE("Dual BSD/GPL");

//has to match the required signature
//file descriptor to file pointer? How does it get converted?
ssize_t proc_list_read(struct file *fp, char __user* out, size_t size, loff_t* off) {
	char buf[13];
	sprintf(buf, "Hello World\n");
	copy_to_user(out, buf, strlen(buf)+1);
	return 0;
}

static struct file_operations proc_list_fops = {
	.read = proc_list_read
};

// What ias the effect of making it static or non-static?
static struct miscdevice proc_list_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "proc_list",
	.fops = &proc_list_fops
};

// called when module is installed
// What ias the effect of making it static or non-static?
int __init init_module()
{
	int error = 0;
	printk(KERN_ALERT "mymodule: Hello World!\n");
	error = misc_register(&proc_list_device);
	if(error) {
		printk(KERN_ALERT "Unable to register misc device\n");
	}
	return error;
}

// called when module is removed
// What ias the effect of making it static or non-static?
void __exit cleanup_module()
{
	misc_deregister(&proc_list_device);
	printk(KERN_ALERT "mymodule: Goodbye, cruel world!!\n");
}

MODULE_DESCRIPTION("CHaracter Device Driver for a device that tracks process metadata");
MODULE_AUTHOR("Srinidhi Raghavendra <sraghav2@binghamton.edu>");
