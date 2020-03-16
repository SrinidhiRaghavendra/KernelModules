This assignment solution contains two parts:
1. Kernel module that registers a character device to that outputs process related information.
2. User program that makes use of the kernel module to read data from the character device

1. Kernel module source: dev_ps.c
a. Defines the new miscdevice(included from linux/miscdevice.h) struct called "proc_list_device" with name "proc_list"(will be in the location /dev) and a file_operations structure that points to a custom function "proc_list_fops" for read operation(while open and close are assigned the default functionailty).
b. The above mentioned miscdevice is then registered using misc_register(from linux/miscdevice.h) function.
c. The custom proc_list_read file operation function is defined. On call of this function, the kernel apis are used to find process related metadata such as: pid, parent pid, cpus, state and returned as a string to the caller.

2. User program source: user.c
Calls read on the character device "/dev/proc_list" in an infinte loop.
Can have multiple processes of user.c reding the same device.

Plan of Action:
1. The kernel module is first build in the same order as mentioned in the Kernel module source description section.
   Progress: A compilable kernel module with miscdevice definition and registering and deregistering is done.
   TODO: Update the custom read method (proc_list_read) to extract the required metadata. (Have to learn about the kernel provided structs suxh as task_struct, task_cpus, etc)
   TODO: Handle error conditions in read function with access_ok.

2. The user program calls read on the character device to demonstrate the usage of kernel modules to add a character device driver and its functionality.
   TODO: Implement the above mentioned user program.

Timeline: 
March 21 - Kernel module will be completed.
March 22 - User program will be completed.
March 27 - Tested clean code submission.

Github link: https://github.com/SrinidhiRaghavendra/KernelModules
