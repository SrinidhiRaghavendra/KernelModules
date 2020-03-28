#include <linux/init.h> //for __init and __exit macros
#include <linux/module.h> // for init_module and cleanup_module prototypes 
#include <linux/fs.h>// for file_operations struct
#include <linux/kernel.h>
#include <linux/miscdevice.h>// for miscdevice struct
#include <linux/uaccess.h>// for copy_to_user
#include <linux/sched/signal.h> // for for_each_process
#include <linux/sched.h> // for task_cpu
#include <linux/slab.h> // for realloc of memory to the buffer
MODULE_LICENSE("Dual BSD/GPL");

//has to match the required signature
//file descriptor to file pointer? How does it get converted?
ssize_t proc_list_read(struct file *fp, char __user* out, size_t size, loff_t* off) {
	//printk(KERN_ALERT "Why %lld", fp->f_pos);
	if(fp->f_pos == 0) {
	struct task_struct *p;
	char *buf = (char*)kmalloc(8196, GFP_KERNEL);
	for_each_process(p) {
		//printk(KERN_ALERT "%ld %lln\n", size, off);
		
		//char *state = (char*)kmalloc(24, GFP_KERNEL);
		char *task_buf = (char*)kmalloc(64, GFP_KERNEL);
		/*
		switch(p->state) {
			case 0x0000: strcpy(state,"TASK_RUNNING"); break;
			case 0x0001: strcpy(state,"TASK_INTERRUPTIBLE"); break;
			case 0x0002: strcpy(state,"TASK_UNINTERRUPTIBLE"); break;
			case 0x0004: strcpy(state,"TASK_STOPPED"); break;
			case 0x0008: strcpy(state,"TASK_TRACED"); break;
			case 0x0040: strcpy(state,"TASK_PARKED"); break;
			case 0x0080: strcpy(state,"TASK_DEAD"); break;
			case 0x0100: strcpy(state,"TASK_WAKEKILL"); break;
			case 0x0200: strcpy(state,"TASK_WAKING"); break;
			case 0x0400: strcpy(state,"TASK_NOLOAD"); break;
			case 0x0800: strcpy(state,"TASK_NEW"); break;
			case 0x1000: strcpy(state,"TASK_STATE_MAX"); break;

		}
		*/
		//krealloc(state, strlen(state), GFP_KERNEL);
		sprintf(task_buf, "%d,%d,%d,%ld\n", p->pid, (p->parent)->pid, task_cpu(p), p->state);
		krealloc(task_buf, strlen(task_buf), GFP_KERNEL);
		if(sizeof(task_buf) + strlen(buf) > sizeof(buf)) {
			krealloc(buf, sizeof(buf)+1024, GFP_KERNEL);
		}
		strcat(buf,task_buf);
		//kfree(state);
		kfree(task_buf);
		//copy_to_user(out, buf, strlen(buf)+1);
		//strcat(buf,in_buf);
	}
	fp->private_data = buf;
	printk(KERN_ALERT "%s\n %ld\n", (char*)fp->private_data, strlen((char*)fp->private_data));
	//printk(KERN_ALERT "%ld\n", strlen((char*)fp->private_data));
	}
	//printk(KERN_ALERT "%ld\n", strlen((char*)fp->private_data));

	char output[size];
	int i;
	int end = (size > strlen((char*)fp->private_data) - fp->f_pos) ? strlen((char*)fp->private_data) - fp->f_pos : size;  
	for(i = 0; i < end; i++) {
		output[i] = (char)(((char*)fp->private_data)[fp->f_pos + i]);
		//printk(KERN_ALERT "%c\n", output[i]);
	}
	fp->f_pos += end;
	//printk(KERN_ALERT "%s\n", output);
	copy_to_user(out, output, strlen(output));
	//printk(KERN_ALERT "%s\n", out);
	return strlen(output);
}

static struct file_operations proc_list_fops = {
	.read = proc_list_read
};

// What ias the effect of making it static or non-static?
static struct miscdevice proc_list_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "proc_list",
	.mode = S_IALLUGO,
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

MODULE_DESCRIPTION("Character Device Driver for a device that tracks process metadata");
MODULE_AUTHOR("Srinidhi Raghavendra <sraghav2@binghamton.edu>");
