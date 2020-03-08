The simplest form of storing a file to a disk is to use linked allocation. To read a file with linked allocation you start by knowing the first block assigned to a file.
When you read the first block then it gives you data belonging to the file, but also gives the next block to read.

For example consider the example linked allocation below:
0 3
1 2
2 -1
3 1
4 -1
. . .

Each line represents a cylinder number (sequential from 0 to 9999) followed by a number indicating the next block in the file sequence.
A -1 indicates the end of the file sequence. In this case, if we are told a file starts on block 0 then we can follow the linked allocation to see the order of blocks that contain the file are:

0 -> 3 -> 1 -> 2

You are to modify your program from Assignment #7 such that you read a third parameter – a file containing the disk layout as specified above. The file will contain blocks ordered from 0 to 9999.
    ./a.out -f –p 47 –b 10 disk_layout.txt

The program will read a series of file requests (no maximum) and service them according to each of the algorithms specified on the command line.
Note: You will read ALL of the file requests from stdin before you start to service the requests.
For each file requested, you will print the blocks traversed in order to read the file (as shown above).
After execution, the program will report the total amount of head movement required to read all of the files requested.

Note: A sample disk_layout.txt file and a file of starting blocks for files is provided for the assignment on D2L.