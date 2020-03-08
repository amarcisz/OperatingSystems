When operating systems were first designed, RAM was managed such that there was a space reserved for the kernel (starting at address 0) and user processes shared all of the other space in RAM.
So, if your kernel was 1GB in size and your parents purchased you 8GB of RAM in your laptop, then addresses 0 -> 1GB was reserved and 1GB -> 8GB was for processes.
When processes were created they requested how much RAM they wanted. When they were terminated, they released the memory back to the OS for new processes to use.
Note: When processes are terminated their memory is joined with any adjacent section(s) to create a merged larger section!
To allocate memory there were 3 different algorithms used:
    • First Fit: Find the first free section large enough to satisfy the request. Allocate the size desired and any remaining memory is a section available for new processes.
    • Best Fit: Find the free section that is large enough to satisfy the request but also create the smallest remaining section available for new processes.
    • Worst Fit: Find the free section that is the largest to satisfy the request and also create the largest remaining section available for new processes.

You are to write a program that will take three command line parameters: the size of the kernel, the total RAM available (in MB), and –f|b|w to indicate first, best or worst fit.
For example:
  ./a.out –k 1024 –t 8192 -w
indicates a kernel that is 1GB in size on a machine with 8GB of RAM using worst fit.

When running your program will read lines from stdin that either create a new process or terminate a process.

To create a process id 5 of 20MB in size:
C5 20

To terminate process 73:
T73

Your program will terminate when either it reaches the end of stdin (Control-D), it is unable to perform the create (not enough memory available), or the terminate (process does not exist).
After your program terminates you are to print the total amount of memory (in MB) that is both available and allocated. For example, the following is the message if I were to provide the two sample input lines above.
  Total available 7148 Total allocated 1044