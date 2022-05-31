///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2019-2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
//
///////////////////////////////////////////////////////////////////////////////
 
////////////////////////////////////////////////////////////////////////////////
// Main File:        heapAlloc.c
// This File:        heapAlloc.c
// Other Files:      -
// Semester:         CS 354 Spring 2020
//
// Author:           Jiwon Song
// Email:            jsong99@wisc.edu
// CS Login:         jis
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "heapAlloc.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           
    int size_status;
    /*
    * Size of the block is always a multiple of 8.
    * Size is stored in all block headers and free block footers.
    *
    * Status is stored only in headers using the two least significant bits.
    *   Bit0 => least significant bit, last bit
    *   Bit0 == 0 => free block
    *   Bit0 == 1 => allocated block
    *
    *   Bit1 => second last bit 
    *   Bit1 == 0 => previous block is free
    *   Bit1 == 1 => previous block is allocated
    * 
    * End Mark: 
    *  The end of the available memory is indicated using a size_status of 1.
    * 
    * Examples:
    * 
    * 1. Allocated block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 27
    *      If the previous block is free, size_status should be 25
    * 
    * 2. Free block of size 24 bytes:
    *    Header:
    *      If the previous block is allocated, size_status should be 26
    *      If the previous block is free, size_status should be 24
    *    Footer:
    *      size_status should be 24
    */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int allocsize;

/*
 * Additional global variables may be added as needed below
 */
blockHeader *prev = NULL;

/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block on success.
 * Returns NULL on failure.
 * This function should:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 and possibly adding padding as a result.
 * - Use NEXT-FIT PLACEMENT POLICY to chose a free block
 * - Use SPLITTING to divide the chosen free block into two if it is too large.
 * - Update header(s) and footer as needed.
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* allocHeap(int size) {     
    
    // Check whether the size is valid
    if (size <= 0 || size > allocsize) {
        return NULL;
    }
    // variables declaration
    int header = 4;
    int block_size = header + size;
    int padding = 1;
    blockHeader * curr;
    
    // Padding to match heap block alignment
    if (block_size % 8 != 0){

        while((block_size + padding) % 8 != 0) {
            padding ++;
        }

        block_size = block_size + padding;
    }
   
   // if the block size is bigger than the whole heap block, return NULL
    if (block_size > allocsize) {
        return NULL;
    }
    // check if anything is already inserted 
    if (prev == NULL) {
        // if nothing is added, start from the beginning
        curr = heapStart;
    }
    else {
        // else start from the previous allocation point
        curr = prev;
    } 
    // loop over the memory block to find free space
    int found = 0;

    // (blockHeader*) -> casting to assign to a blockHeader ptr
    // (void*)curr + header_size -> move to the next header block
    // iterate to find suitable free block
    while(!found) {
        // get the size of each block from its header block
        int header_size = curr -> size_status >> 3 << 3;
        if ((curr -> size_status & 1) == 0) {
            // if free block is found check whether the block is big enough
            if (header_size < block_size) {
                curr = (blockHeader*)((void*)curr + header_size);
            }
            else {
            // if the block is big enough, break 
                break;
            }
        }

        else {
            // next header block
            curr = (blockHeader*)((void*)curr + header_size);
            // if the ptr reaches the end of the block, start from the beginning
            if (curr -> size_status == 1) {
                curr = heapStart;
            }
        }
        // if current ptr reaches the starting point, return NULL (no free blocks available)
        if (curr == prev) {
            return NULL;
        }
    }
    // get the header size by bit shifting, it will erase last 3 bits
    // 25 -> 24, 27 -> 24
    int header_size = curr -> size_status >> 3 << 3;
    // calculate remaining free size
    int freesize = header_size - block_size;   

    // check whether there is enough free size
    if (freesize > 8) {

       blockHeader * next_block = (blockHeader*)((void*)curr + block_size);
       // update new free block
       next_block -> size_status = freesize;
       next_block -> size_status += 2;
       // update the current block to allocated block
       curr -> size_status = (curr->size_status & 2) + block_size;
       curr -> size_status += 1;
       // update new footer for next free block
       blockHeader * footer = (blockHeader*)((void*)next_block + freesize - 4);
       footer -> size_status = freesize;
    }
    else {
       // if it finds the exact block size
       curr -> size_status += 1;
    }
    // update prev pointer
    prev = curr;
    // return the heap memory location where payload is located
    return (void*)curr + header;
    
   
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - USE IMMEDIATE COALESCING if one or both of the adjacent neighbors are free.
 * - Update header(s) and footer as needed.
 */                    
int freeHeap(void *ptr) {    
    
    // check if the ptr is valid
    if (ptr == NULL) {
        return -1;
    }
    // check if the ptr is outside of the heap space
    blockHeader * end = heapStart;
    while(end -> size_status != 1) {
        end = (blockHeader*)((void*)end + sizeof(blockHeader));
    }
    if (ptr > (void*)end) {
        return -1;
    }

    // get the current header ptr
    blockHeader* curr = (blockHeader*)(ptr - sizeof(blockHeader));
    
    // if ptr block is already freed, return NULL
    if ((curr -> size_status & 1) == 0) {
        return -1;
    }
    // payload size of the working block 
    int block_size = curr -> size_status >> 3 << 3;

    // variables to distinguish whether prev and next blocks are free or not
    int pf = 0;
    int nf = 0;
    int prev_block_size = 0;
    int next_block_size = 0;
    int new_size = 0;

    
    // check if the prev block is free or not; size /10||11 == not free else free
    if ((curr -> size_status & 2) != 2) {
        // prev block is free 
        pf = 1;
    }
    
    // check adj next block
    blockHeader * next_header = (blockHeader*)((void*)curr + block_size);
    // get the next block's block size 
    next_block_size = next_header->size_status >> 3 << 3;
   

    // check if the next block is free or not
    if ((next_header -> size_status & 3) == 2) {
        // next block is free
        nf = 1;
    }

    // if both prev and next blocks are free
    if (pf && nf) {
        
        // check adj prev block
        blockHeader * prev_footer = (blockHeader*)((void*)curr - 4); 
        // get prev block's header
        blockHeader * prev_header = (blockHeader*)((void*)curr - prev_footer->size_status);
        // get the prev block's block size
        prev_block_size = prev_header -> size_status >> 3 << 3;

        // calculate the new block size for new free block
        new_size = prev_block_size + block_size + next_block_size;
        
        // set new header for new free block
        blockHeader * new_header = prev_header;
        // update the size status of the new header
        new_header -> size_status = (prev_header -> size_status & 2) + new_size;
        // set new footer for the new free block
        blockHeader * new_footer = (blockHeader*)((void*)new_header + new_size - sizeof(blockHeader));
        // update the new footer's size_status
        new_footer -> size_status = new_size;
        // do not need to change next block header's size_status because it is always size/01 in this case
        prev = new_header; 

        return 0;
    }
    // if only the prev block is free
    else if (pf && !nf) {
        
        // get prev block header and footer only if prev block is guaranteed to be free
        // check adj prev block
        blockHeader * prev_footer = (blockHeader*)((void*)curr - 4); 
        // get prev block's header
        blockHeader * prev_header = (blockHeader*)((void*)curr - prev_footer->size_status);
        // get the prev block's block size

        prev_block_size = prev_header->size_status >> 3 << 3;
        
        // calculate the new block size for new free block
        new_size = prev_block_size + block_size;
        
        // set new header for new free block
        blockHeader * new_header = prev_header;
        // update the size_status of the new free block's header
        new_header -> size_status = (prev_header -> size_status & 2) + new_size;
        // set new footer for the new free block
        blockHeader * new_footer = (blockHeader*)((void*)new_header + new_size - sizeof(blockHeader));
        // update the new footer's size_status
        new_footer -> size_status = new_size;
        // check whether the next block is endmark and update the next block's header's size_status
        if ((void*)next_header != (void*)end) {
            next_header -> size_status = next_block_size + 1;
        }
        prev = new_header;
        return 0;
    }
    // if only the next block is free
    else if (!pf && nf) {
        // calculate the new block size for new free block
        new_size = block_size + next_block_size;

        // set new header for the new free block
        blockHeader * new_header = curr;
        // update the new header's size_status
        new_header -> size_status = (curr -> size_status & 2) + new_size;
        // sets new footer for the new free block
        blockHeader * new_footer = (blockHeader*)((void*)new_header + new_size - sizeof(blockHeader));
        // update the new footer's size_status
        new_footer -> size_status = new_size;
        prev = new_header;
        return 0;
    }
    // if all adjacent blocks are allocated
    else {
        // update the current block's size_status
        curr -> size_status = (curr -> size_status & 2) + block_size;
        // set new footer for current block
        blockHeader * curr_footer = (blockHeader*)((void*)curr + block_size - 4);
        // set the footer's size_status
        curr_footer -> size_status = block_size;
        // update the next block's header size_status
        next_header -> size_status = (next_header -> size_status & 1) + next_block_size;
        // update the previous block
        prev = curr;
        return 0;
    }
     
} 
 
/*
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int initHeap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple initHeap calls
 
    int pagesize;  // page size
    int padsize;   // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int fd;

    blockHeader* endMark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    allocsize = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    allocsize -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heapStart = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    endMark = (blockHeader*)((void*)heapStart + allocsize);
    endMark->size_status = 1;

    // Set size in header
    heapStart->size_status = allocsize;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heapStart->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
    footer->size_status = allocsize;
  
    return 0;
} 
                  
/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dumpMem() {     
 
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end   = NULL;
    int t_size;

    blockHeader *current = heapStart;
    counter = 1;

    int used_size = 0;
    int free_size = 0;
    int is_used   = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "used");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "used");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total used size = %d\n", used_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", used_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;  
} 

