#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at
	
    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
	lw v0,TF_EPC(sp)
	addiu v0,v0,4
	sw v0,TF_EPC(sp)	
    // TODO: Copy the syscall number into $a0.
    	lw a0,TF_REG4(sp)

    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer
    lw      t3, 16(t0)                  // t3 <- the 5th argument of msyscall
    lw      t4, 20(t0)                  // t4 <- the 6th argument of msyscall
	lw t5,12(t0)//t5<- the 4th argument
	lw t6 ,8(t0)// t6<-the 3rd argument
	lw t7 ,4(t0)//t7<-the 2nd argunent
	lw t1,0(t0)// t1<-the 1st argument
    // TODO: Allocate a space of six arguments on current kernel stack and copy the six arguments to proper location
    	addiu sp,sp,-24
	sw t1,0(sp)
	sw t7,4(sp)
	sw t6,8(sp)
	sw t5,12(sp)
	sw t3,16(sp)
	sw t4,20(sp)
    
    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack
   	addi sp,sp,24 
    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
     .word sys_write_dev
     .word sys_read_dev
