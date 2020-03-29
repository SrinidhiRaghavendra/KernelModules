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
//file descriptor to file pointer? How does it get converted? - got it, read system library call to vfs_read to this
ssize_t proc_list_read(struct file *fp, char __user* out, size_t size, loff_t* off) {
	if(0 == *off) {
	  struct task_struct *p;
	  int buf_length = 1024;
	  char *buf = kzalloc(buf_length, GFP_KERNEL);
	  for_each_process(p) {
	    char state[24];
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
	    char task_buf[39+strlen(state)];
	    sprintf(task_buf, "PID=%5d, PPID=%5d, CPU=%2d, STATE=", p->pid, (p->parent)->pid, task_cpu(p));
	    strcat(task_buf, state);
	    strcat(task_buf, "\n");
	    if(strlen(task_buf) + strlen(buf) > buf_length) {
		buf_length += buf_length - strlen(buf) + strlen(buf);
		char *intermediate = kzalloc(buf_length, GFP_KERNEL);
		memcpy(intermediate, buf, strlen(buf));
		kfree(buf);
		buf = intermediate;
	    }
            strcat(buf,task_buf);
	}
	fp->private_data = buf;
	}
	
	char output[size];
	int i;
	int int_off = (int)*off;
	if(int_off >= strlen((char*)fp->private_data)) {
		return 0;
	}
	int end = (size > strlen((char*)fp->private_data) - int_off)  ? (strlen((char*)fp->private_data) - int_off) : size;  
	for(i = 0; i < end; i++) {
		output[i] = (char)(((char*)fp->private_data)[int_off + i]);
	}
	*off = *off + end; 
	copy_to_user(out, output, end);
	return end;
}

static struct file_operations proc_list_fops = {
	.read = proc_list_read
};

//struct of type miscdevice to register the device
static struct miscdevice proc_list_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "proc_list",
	.mode = S_IALLUGO,
	.fops = &proc_list_fops
};

// called when module is installed
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
void __exit cleanup_module()
{
	misc_deregister(&proc_list_device);
	printk(KERN_ALERT "mymodule: Goodbye, cruel world!!\n");
}
// For checkign printk message, use command "dmesg" or "sudo tail -n 1000 /var/log/kern.log" 
MODULE_DESCRIPTION("Character Device Driver for a device that tracks process metadata");
MODULE_AUTHOR("Srinidhi Raghavendra <sraghav2@binghamton.edu>");
