This question consists of writing a program that translates logical to physical addresses for a virtual address space of size 216 = 65,535 bytes.
Your program will read from stdin a list of logical addresses, translate each logical address to a physical address and then print the value at that address.
The goal is to simulate the steps involved in translating logical to physical addresses.

Design your simulation with the following parameters:
    • A page is 28 = 256 bytes. (Note, this means a frame is also 28 in size).
    • Therefore, the page table has 28 entries. You can use a simple array to implement this table.
    • Your process has been allocated n frames. n is a parameter to your program on the command line.

For the address translation you are to use the simple single level of page table mapping (page table is 8 bits, page offset is 8 bits)
        [ Page Table | Page Offset ]

I have provided a sample file memory.txt that contains the contents of the processes memory. This memory file is the second parameter to your program.
When you page “in” you must load the frame with the contents of the page from this file. You can use fopen(), fread(), fseek() and fclose() to manage the file to get the data.

Your program should print for each logical address read:
    Logical address -> physical address -> char
(where char is the character read at that logical address). It should also keep track of the number of page faults that occur and print the statistic at the end.

Notes:
    • Running your program with the sample file can be done as: ./a.out 10 memory.txt < sample_logical_addr
    • For this question you can use a simple First-in First-out (FIFO) page replacement policy. Since you do NOT have to worry about writing to the page you DO NOT need to save the page when you replace it!
    • Be aware of data type sizes. If you read the logical address as an int then this will be read as a 32 or 64 bit value. You want it as a 16 bit value. I choose 8 bits for each so as to make it easier dealing with bit manipulation.
    • A coding suggestion is to keep a large character array to represent the contents of your physical memory. This array would be n * 28 in size. Once you translate logical to physical then you can find the value by just accessing that index of the array.