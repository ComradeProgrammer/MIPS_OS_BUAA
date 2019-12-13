/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurred during read the IDE disk, panic. 
// 	
// Hint: use syscalls to access device registers and buffers
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int run=0;
	u_char ret=0;
	int tmp=0;
	//writef("ide-read para:%d,%d,%x,%d",diskno,secno,dst,nsecs);
	while (offset_begin + offset < offset_end) {
            // Your code here
		//set diskno
		syscall_write_dev(&diskno,0x13000010,4);
		//set offset
		tmp=offset_begin+offset;
		syscall_write_dev(&tmp,0x13000000,4);
		//set status
		syscall_write_dev(&run,0x13000020,4);
		//get return value;	
		syscall_read_dev(&ret,0x13000030,1);
		if(ret==0){
		// error occurred, then panic.
			user_panic("read disk error\n");
		}
		syscall_read_dev(dst+offset,0x13004000,0x200);
		offset+=0x200;
        }
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurred during read the IDE disk, panic.
//	
// Hint: use syscalls to access device registers and buffers
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
        // Your code here
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;
	int run=1;
	u_char ret=0;
	int tmp=0;
//	writef("diskno: %d\n", diskno);
	while (offset_begin + offset < offset_end)  {
	    // copy data from source array to disk buffer.
		syscall_write_dev(src+offset,0x13004000,0x200);
		//set disk no
		syscall_write_dev(&diskno,0x13000010,4);
		//set offset
		tmp=offset_begin+offset;
		syscall_write_dev(&tmp,0x13000000,4);
		// set status
		syscall_write_dev(&run,0x13000020,4);
		//get return value;	
		syscall_read_dev(&ret,0x13000030,1);
                // if error occur, then panic.
		if(ret<0){
			user_panic("write disk error");
		}
		offset+=0x200;
	 }
	return 0;
}

