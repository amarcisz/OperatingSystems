Lab2: Pipes

Write a multithreaded program that calculates various statistical values for a list of integers.
This program will read a series of integers from standard input (scanf()) and then will create three separate worker threads.
One thread will determine the average of the numbers, the second will determine the maximum value and the third will determine the minimum number.

For example, suppose your program is passed the integers:
    90 81 78 95 79 72 85

The program will report
   The average value is 82
   The minimum value is 72
   The maximum value is 95

The variables representing the average, minimum and maximum values will be stored globally.
The worker threads will set these values, and the parent thread will output the values once the workers have exited.

Notes:
    • The end of the input will be denoted by reading a 0 (which is NOT to be used as part of the calculation).
    • You are to use pipes to deliver numbers read to the worker threads.