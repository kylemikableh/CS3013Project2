#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <asm/current.h>
#include <asm-generic/uaccess.h>

unsigned long **sys_call_table;

asmlinkage long (*ref_sys_cs3013_syscall2)(void);

typedef struct task_struct task_struct;


struct ancestry {
pid_t ancestors[10];
pid_t siblings[100];
pid_t children[100];
};


void ancestor_search(task_struct* parent_ptr, pid_t* ancestor_ptr) {
  pid_t tmp_pid = parent->pid;
  *ances_ptr++ = parent_ptr->pid;
  printk(KERN_INFO "Parent: %d\n", parent->pid);
  find_ancestors(parent->p_opptr, ances_ptr);

}

asmlinkage long new_sys_cs3013_syscall2(unsigned short *target_pid, struct ancestry *response) {

	//set pointers
	unsigned short target_pid_copy;
	ancestry response_copy;
	ancestry* response_copy_ptr = &response_copy;


	//check validity
	if (copy_from_user(&target_pid_copy, target_pid, sizeof(unsigned short))) {
		return EFAULT;
	}

	if(copy_from_user(response_copy_ptr, response, sizeof(ancestry))){
		return EFAULT;
	}

	//declare ancestry struct pointers
	pid_t* ancestor_ptr = response_copy_ptr->ancestors;
	pid_t* siblings_ptr = response_copy_ptr->siblings;
	pid_t* children_ptr = response_copy_ptr->children;


	//get siblings and children
	//switch to correct process here!!! but how???? cuz idk
	task_struct* current = get_current();
	task_struct* young_sibling = current->p_cptr;
	task_struct* old_sibling = current->p_osptr;
	task_struct* child = current->p_pptr;

	task_struct* pos;


	//list children
	list_for_each_entry (pos, &(child), child) {
		*children_ptr++ = pos->pid;
		printk(KERN_INFO "Child %d found.\n", pos->pid);
	}

	//list siblings

	//needs check to see if "silbling" is itself(the original child one)

	list_for_each_entry(pos, &(young_sibling), young_sibling) {
		siblings_ptr++ = pos->pid;
		printk(KERN_INFO "Sibling %d found.\n", pos->pid);
	}

	list_for_each_entry(pos, &(old_sibling), old_sibling) {
		siblings_ptr++ = pos->pid;
		printk(KERN_INFO "Sibling %d found.\n", pos->pid);
	}

	if (current->pid != 1) {
		ancestor_search(current->p_opptr, ancestor_ptr);
	}


	if(copy_to_user(response, respone_copy_ptr, sizeof(ancestry))){
		return EFAULT;
	}


	return 0;

}

static unsigned long **find_sys_call_table(void) {
  unsigned long int offset = PAGE_OFFSET;
  unsigned long **sct;

  while (offset < ULLONG_MAX) {
    sct = (unsigned long **)offset;

    if (sct[__NR_close] == (unsigned long *) sys_close) {
      printk(KERN_INFO "Virus Scanner: Found syscall table at address: 0x%02lX",
	     (unsigned long) sct);
      return sct;
    }

    offset += sizeof(void *);
  }

  return NULL;
}

static void disable_page_protection(void) {
  /*
    Control Register 0 (cr0) governs how the CPU operates.

    Bit #16, if set, prevents the CPU from writing to memory marked as
    read only. Well, our system call table meets that description.
    But, we can simply turn off this bit in cr0 to allow us to make
    changes. We read in the current value of the register (32 or 64
    bits wide), and AND that with a value where all bits are 0 except
    the 16th bit (using a negation operation), causing the write_cr0
    value to have the 16th bit cleared (with all other bits staying
    the same. We will thus be able to write to the protected memory.

    It's good to be the kernel!
  */
  write_cr0 (read_cr0 () & (~ 0x10000));
}

static void enable_page_protection(void) {
  /*
   See the above description for cr0. Here, we use an OR to set the
   16th bit to re-enable write protection on the CPU.
  */
  write_cr0 (read_cr0 () | 0x10000);
}

static int __init interceptor_start(void) {
  /* Find the system call table */
  if(!(sys_call_table = find_sys_call_table())) {
    /* Well, that didn't work.
       Cancel the module loading step. */
    return -1;
  }




  /* Store a copy of all the existing functions */
  ref_sys_cs3013_syscall2 = (void *)sys_call_table[__NR_cs3013_syscall2];




  /* Replace the existing system calls */
  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)new_sys_cs3013_syscall2;
  enable_page_protection();


  /* And indicate the load was successful */
  printk(KERN_INFO "Loaded Virus Scanner!");

  return 0;
}

static void __exit interceptor_end(void) {
  /* If we don't know what the syscall table is, don't bother. */
  if(!sys_call_table)
    return;

  /* Revert all system calls to what they were before we began. */
  disable_page_protection();
  sys_call_table[__NR_cs3013_syscall2] = (unsigned long *)ref_sys_cs3013_syscall2;
  enable_page_protection();

  printk(KERN_INFO "Unloaded Virus Scanner!!");
}

MODULE_LICENSE("GPL");
module_init(interceptor_start);
module_exit(interceptor_end);
