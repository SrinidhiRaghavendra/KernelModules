This assignment solution contains two parts:
1. Kernel module that registers a character device to that outputs process related information.
2. User program that makes use of the kernel module to read data from the character device

1. Kernel module source: dev_ps.c
a. Defines the new misc_device struct called "proc_list_device" with name "/dev/proc_list" and a file operations structure that defines a custom function "proc_list_fops" for read operation(while open and close are assigned the default functionailty).
b. The above mentioned misc_device is then registered using misc_register function.
c. The custom proc_list_read file operation function is defined. On call of this function, the kernel apis are used to find process related metadata such as: pid, parent pid, cpus, state and returned as a string to the caller.

2. User program source: user.c
Calls read on the character device "/dev/proc_list" in an infinte loop.

Plan of Action:
1. The kernel module is first build in the same order as mentioned in the Kernel module source description section.
2. Need to learn about the OS data structures like task_struct to extract the required meta data about a process.

The user program is then implemented to call read on the character device to demonstrate the usage of kernel modules to add a character device driver and its functionality.
