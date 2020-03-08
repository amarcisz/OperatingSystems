Problem 1:
CPUs today have multiple CPUs. Dual-core has 2 processors and Quad-core has 4 processors.
Modify your program so that the output from Assignment #1 now reflects n processors operating at once.
To make your program flexible, provide n as a command line argument to the program.
The input file is the same, but the output is different.

For the example (for n = 2):
User  Process  Arrival  Duration  Priority
Jim  A  2  5  1
Sue  A  3  5  5
Jim  B  5  3   2
Mary  C  6  4  10

This would result in:
Time  Job
2   A
3   A
7   B
8   C
10  CPU 2 IDLE
12  CPU 1 IDLE

Summary
Sue  8
Jim  10
Mary  12

Note: Be careful! You cannot start a job until it has arrived.

Problem 2:
The downside of Priority is that you are at the mercy of the order in which users specify importance of jobs and their arrival times.
A large job specified to be important at the beginning has everyone waiting and becoming irritated.
A solution to this is to try and help people with small jobs get in and out quickly.
To do this, we need pre-emption – the concept of periodically stopping a job to take away the CPU and decide who is the next eligible process to run.
This is needed to prevent large jobs that acquire the CPU and start computing to only hold the CPU while new jobs arrive and have to wait.
To implement pre-emption, you only execute a job for 1 period and then you add it back to the run queue.

For example if we have 1 CPU:

User  Process  Arrival  Duration  Priority
Jim  A  2  5  1
Mary  B  2  3   1
Sue  D  5  5  5
Mary  C  6  2  5

This would result in:
Time  Job
2  A
3  B
4  A
5  B
6  A
7  B
8  A
9  A
10  D
11  C
12  D
13  C
14  D
15  D
16  D
17  IDLE

Summary
Jim  10
Mary  14
Sue  17

Modify your C program from question #1 that will read in job requests and print out the corresponding job schedule according to a Priority algorithm with pre-emption as above.
The input format is each line entry contains a job separated by a tab. The first line of input is ignored as the header. The output format is two tables.
First table is the running time and the job currently executing (tab separated).
The second table is a summary with the user name (in the order in which jobs arrive) and the time when their last job is completed.

Hints: While your run queue removes jobs like a queue, jobs are not added like a queue. Instead they are added like a sorted list addition. It is very important to see that you CANNOT read all the jobs and create the queue in advance as you may have done in earlier questions. Instead, you should add jobs to the queue as they arrive in time.