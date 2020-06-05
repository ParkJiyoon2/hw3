
# Branch 32160462

- TODO LIST

- input_vm-light

```c
fault!❌ freepfn: 0x0 pgn: 0xc pid: 8
pid: 8, va: 0x0000c4f6 pa: [0x000004f6] = 0x000004F6
pid: 8, va: 0x0000ca81 pa: [0x00000a81] = 0x00000A81
fault!❌ freepfn: 0x1 pgn: 0xb pid: 8
pid: 8, va: 0x0000b81c pa: [0x0000181c] = 0x0000181C
fault!❌ freepfn: 0x2 pgn: 0x2 pid: 8
pid: 8, va: 0x00002d5b pa: [0x00002d5b] = 0x00002D5B
pid: 8, va: 0x00002b94 pa: [0x00002b94] = 0x00002B94
fault!❌ freepfn: 0x3 pgn: 0x4 pid: 8
pid: 8, va: 0x00004072 pa: [0x00003072] = 0x00003072
fault!❌ freepfn: 0x4 pgn: 0x7 pid: 8
pid: 8, va: 0x00007d72 pa: [0x00004d72] = 0x00004D72
fault!❌ freepfn: 0x5 pgn: 0x1 pid: 8
pid: 8, va: 0x000011ad pa: [0x000051ad] = 0x000051AD
fault!❌ freepfn: 0x6 pgn: 0x3 pid: 8
pid: 8, va: 0x000031a8 pa: [0x000061a8] = 0x000061A8
fault!❌ freepfn: 0x7 pgn: 0xf pid: 8
pid: 8, va: 0x0000f3d0 pa: [0x000073d0] = 0x000073D0
-1 5 2 6 3 -1 -1 4 -1 -1 -1 1 0 -1 -1 7
     Pgn | Pgfn
 00000000| ffffffff
 00000001| 00000005
 00000002| 00000002
 00000003| 00000006
 00000004| 00000003
 00000005| ffffffff
 00000006| ffffffff
 00000007| 00000004
 00000008| ffffffff
 00000009| ffffffff
 0000000a| ffffffff
 0000000b| 00000001
 0000000c| 00000000
 0000000d| ffffffff
 0000000e| ffffffff
 0000000f| 00000007
```

- input_vm

```c
pid: 8, va: 0x0000f446 pa: [0x00007446] = 0x00007446
pid: 8, va: 0x00002866 pa: [0x00002866] = 0x00002866
pid: 8, va: 0x000063b8 pa: [0x0000c3b8] = 0x0000C3B8
pid: 8, va: 0x0000ebe1 pa: [0x0000dbe1] = 0x0000DBE1
pid: 8, va: 0x0000099b pa: [0x0000f99b] = 0x0000F99B
pid: 8, va: 0x000004fc pa: [0x0000f4fc] = 0x0000F4FC
pid: 8, va: 0x0000c5f4 pa: [0x000005f4] = 0x000005F4
pid: 8, va: 0x0000521c pa: [0x0000a21c] = 0x0000A21C
pid: 8, va: 0x00009f7a pa: [0x00008f7a] = 0x00008F7A
pid: 8, va: 0x0000122c pa: [0x0000522c] = 0x0000522C
pid: 8, va: 0x0000e28b pa: [0x0000d28b] = 0x0000D28B
pid: 8, va: 0x0000aadd pa: [0x0000eadd] = 0x0000EADD
pid: 8, va: 0x0000fd43 pa: [0x00007d43] = 0x00007D43
pid: 8, va: 0x0000e862 pa: [0x0000d862] = 0x0000D862

...
pid: 8, va: 0x0000ceae pa: [0x00000eae] = 0x00000EAE
pid: 8, va: 0x0000ddc4 pa: [0x0000bdc4] = 0x0000BDC4
15 5 2 6 3 10 12 4 9 8 14 1 0 11 13 7
     Pgn | Pgfn
 00000000| 0000000f
 00000001| 00000005
 00000002| 00000002
 00000003| 00000006
 00000004| 00000003
 00000005| 0000000a
 00000006| 0000000c
 00000007| 00000004
 00000008| 00000009
 00000009| 00000008
 0000000a| 0000000e
 0000000b| 00000001
 0000000c| 00000000
 0000000d| 0000000b
 0000000e| 0000000d
 0000000f| 00000007
```

# hw3, Virtual memory simulation By June 8

In this programming assignment, we will simulate one of virtual memory system, paging.
This is 3-weeks code work, and 5-freeday rule also applies here.

## Objective of the homework
1. to simulate paging system with your own assumptions.
there are several options to expand your work; so choose your difficulty level

option|choices|addr_space|Nproc|demand_pg|phys_mem|swapping
|------|---|---|---|---|---|---|
a.| simple |16-bit address space| single process| demand paging| - | - 
b.| complicated |32-bit address space| 10 procs| demand paging| inf. physical memory simulation| - 
c.| complex |32-bit addr space| 10 procs| demand paging| limited physical memory| swapping

2. sample input has the following format
pid access_va rw

3. sample output should be the following format
pid access_va rw [accss_pa] mem_value

4. you can make your own assumption, and this code work is open to extend.
Please document if you implement your own things.
(I strongly recommend you to try that.)

## Paging as a means of VA-PA mapping
With the virtual memory, a process can work with its own logical address space, meaning that a user process can independently use memory from the other processes.

For example, assume two different processes, P1 and P2. 
P1 and P2 share physical memory because they are running concurrently.
P1 has its own 4GB address space from 0 to 0xFFFF:FFFF.
So does P2. 
That is, P1's view of memory always begins from the address 0; and P2's view of memory also begins with the address 0.
P1's memory address 0 should be different from P2's memory address 0; How that can be possibly implemented in computer?
Virtual memory system provides per-process address space, which allows a process to use logical memory address, instead of physical memory address.

Virtual memory in modern OS enables this by VA-to-PA mapping. 
With the virtual memory, all user process memory (address) are logical memory (address), instead of physical memory.
Even without knowing where physically located in, a process can work only with logical memory address.

Modern OS and computer architecture cooperates to implement a virtual memory system.
To use with logical address, instead of physical address, computer architecture has MMU (memory management unit). 
Once MMU is turned on, all memory access is interpreted by MMU, and logical-to-physical address translation is conducted on the CPU core.

OS sets up the logical-to-physical address translation table, and sets the translation table's base register in the CPU core as the beginning address of the translation table.
The table has the mapping between logical address (virtual address) and physical address, with some more information.

Paging is the fundamental way to mapping the entire virtual address space onto physical address space.
When we use paging, the virtual address space is divided into small same-sized pieces, called page.
Usually, page size is 4KB; so 4GB address space (for 32 bit computers) is divided into 1M pages. 

Then, the entire physical memory is also divided into page-sized fragments, called page frame (or simply frame). 
One page is mapped onto a page frame, so that the 4KB page can have physical memory region.
Page and page frame can be numbered with some index number. 
For example, virtual address of 0\~0xFFF is the page number 0, 0x1000\~0x1FFF is the page number 1, and so one.
Similarly, physical address of 0\~0xFFF correspond to the page frame number (pfn) 0, 0x1000\~0x1FFF correspond to the page frame number (pfn) 1.

The translation table has million of entries that maps page number with page frame number.
For example, table[0] contains page 0's mapping pfn.
If table[0].pfn = 3, then page 0 is mapped with pfn 3. 


## Two-level paging
The page table size is quite large.
When we consider 32 bit address space, 4KB paging, a process's page table consumes 1M entries; 
further if we assume 4 bytes of page table entry, then the size of a page table is 4MB. 
If we have 100 processes, 400MB should be used for translation table, which has no user data; but only translation-related metadata.

OS researchers investigated the page table usages, and many parts are commonly used and many parts are empty.
That is, the application's effective memory region is not so large in many cases.
2-level and multi-level paging has introduced with the help of computer architecture.
The address space is further divided into page directory, more than page frame.
A page directory covers 4MB address space. (That is, single entry of page directory maps 4MB memory region.)
In the case, 4GB memory region is divided into 1K (1024) page directories, each of which covers 4MB region.
4MB page directory is further divided into 4KB pages, so that a page directory has 1024 pages.

A good thing of multi-level page table is that we can omit the unnecessary page table entries.
For example, in normal flat (1-level) page table structure, 4MB emptry region takes 1024 entries, and 4KB memory region in the page table.
However, in the case of two-level paging, single entry at page directory, 4bytes, are enough to represent a 4MB empty mapping.
The case is more obvious when we think of 100MB empty region.

When we use 2-level paging, CPU conducts some more jobs in VA-to-PA translation.
Now, CPU's translation table base register points to the page directory, instead of page table.
The VA's high 10 bits are used as index into the page directory.
That is, we identify the page directory entry, with the translation table base and index.
The entry has a pointer to the page table, (beginning address of the page table) which has 1024 entries. 

Then, VA's mid 10 bits are used as index into the page table.
From the derived page table's base address in the previous step, and the mid index, we can identify the page table entry.
The entry has a pfn (page frame number), which covers 4KB memory region.

Inside the page frame, the lower 12 bits are used as page offset, and finally calculates the target address to PA.

## Demand paging
Initially page table is empty when OS creates a process. 
That is, all page table entries are invalid.
Note that every memory access goes through MMU-based VA-PA translation, if you enabled MMU.
At the time, OS triggers 'page fault' when the page table entry is empty or invalid. 

In the case, OS grabs a new free page frame, and fills in the page table, according to the free page frame number. 
To enable this, OS needs to manage the free page frames list. 
At the boot time, OS initializes the free pages frames list, and puts all page frames in the list.

## Swapping
Besides the mapping between VA-to-PA, paging supports swapping.
Virtual memory supports larger address space, and leads to larger application memory usage.
The physical memory for 100 processes would need 400GB physical memory; which is rarely possible for 32bit computers.

Paging allows logical address space, and deals with the case when the logical memory is larger than the physical memory.
That is, some logical address is mapped on physical address; but some address is not mapped on physical address.

In many cases, the permanent storage (lower hierarchy memory, SSD, HDD, network file systems) is slower, but more scalable, and cost-efficient.
Thus, we can save our data on the permanent storage, when it is not immediately needed.

When OS lacks of free page frames, then it finds one of page frames, and evict it to the external storage.
The OS paging system can designate some physical page frames to be out of physical memory, so that we can obtain more free page frames.
Because the page frame is not in the memory, the corresponding page table entry for the evicted page should be invalidated.
It is called as swap out operation, that exchanges physical page frame with permanent storage.
One small notice is that swap out is needed only when you modify data by writing something in the memory.
Usually, page table has modified bit, and CPU core checks the bit when the CPU writes the physical page frame.

To designate an eviction page frame, OS uses several strategies and runs page replacement algorithm.
A common idea to designate an eviction frame is called LRU (least recently used). 
LRU evicts the least recently used page frame, which is thought to be not used in some near future.
There are several simplifying algorithms, such as clock, second-chance, NRU, etc., 
but all of them tries to keep future referencing pages in physical memory, and put out page frames that are not going to be used.

One more thing to note is swap-in operation. 
When we access a page frame that has evicted by OS, then CPU needs to bring back the data from HDD/SSD/network.
To bring your data back to a free frame, 1) you need a free frame to get back.
2) you need to remember where you saved your data in the permanent storage. 
3) copy your data from the storage to the new page frame, and
4) fills in the page table with the new pfn.

Recall that MMU's translation table has all information, and page fault traps the access to the invalid page access, and triggers swap-in operation.

Happy hacking!

Seehwan
