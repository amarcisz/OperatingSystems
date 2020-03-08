Write a pthreads solution to the following problem.
A barber has three barber chairs and a waiting room with a number of chairs in it.
When a barber finishes cutting a customer's hair, they dismiss the customer and then goes to the waiting room to see if there are other customers waiting.
If there are, they bring one of them back to the chair and cut their hair. It takes between 5 and 7 seconds to cut a customer’s hair.
If there are no other customers waiting, the barber returns to their chair and sleeps in it.

Each customer, when they arrive, looks to see what the barbers are doing. If a barber is sleeping, then the customer wakes a barber up and sits in their chair for a cut.
If all the barbers are cutting hair, then the customer goes to the waiting room. If there is a free chair in the waiting room, the customer sits in it and waits their turn.
Customers that are waiting will be serviced in the order that they arrived. If there is no free chair, then the customer leaves with a long shaggy mop of hair ☺

Note: For your solution you should use a pthread for each of the barbers.

There are always 3 barbers and 3 barber chairs! How frequent a customer arrives for a haircut is chosen at random between 0 and n seconds. See the rand() system call.
Your program will also take as a command line parameter the number of chairs in the waiting room.
So  ./a.out –c 5 –n 12 Will specify a waiting room with 5 chairs with customers randomly arriving for a haircut between 0 – 12 seconds.

Output of the program will consist of the following:
    • When a customer is turned away from a full waiting room: o Customer x leaves with a long shaggy mop of hair!
    • When a customer takes a seat in the waiting room: o Customer x is waiting for a haircut.
    • When a customer is finished getting a haircut: o Customer x is styling a fresh haircut :)

x in each of these output lines is the number of the customer. Numbers are assigned sequentially starting at 0 to customers as they arrive to check the waiting room.