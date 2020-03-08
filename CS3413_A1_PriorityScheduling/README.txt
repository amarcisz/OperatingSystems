A default way in which operating systems schedule jobs to be completed is via Priority. Priority is a number provided by the user that indicates the importance of a job and how quickly it should be executed. The number is often specified in the range of 1 to 10, where 1 is the highest priority. Hint: This is typically implemented using a sorted linked-list data structure.
For example:
User  Process  Arrival  Duration  Priority
Jim  A  2  5  1
Sue  A  3  5  5
Jim  B  5  3   2
Mary  C  6  4  10

This would result in:
Time    Job
 2       A
 7       B
 10      A
 15      C
 19      IDLE

Summary
Jim     10
Sue     15
Mary    19

C program will read in job requests and print out the corresponding job schedule according to a Priority algorithm as above.
The input format is each line entry contains a job separated by a tab. The first line of input is ignored as the header.
The output format is two tables.
First table is the running time and the job currently executing (tab separated).
The second table is a summary with the user name (in the order in which jobs arrive) and the time when their last job is completed.