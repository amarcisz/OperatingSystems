The Smoker’s Problem
There are four processes in this problem: three smoker processes and an agent process. Each of the smoker processes will make a cigarette and smoke it.
To make a cigarette requires tobacco, paper, and matches. Each smoker process has one of the three items. i.e., one process has tobacco, another has paper, and a third has matches.
The agent has an infinite supply of all three. The agent randomly places two of the three items on the table, and the smoker that has the third item makes the cigarette.
After having a smoke a process sleeps for a random amount of time. Write a pthread program using semaphores to synchronize the processes.