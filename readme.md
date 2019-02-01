CMPUT 379 Assignment 1 README
Team members: Zhihao Zhang (zhihao9), Siyu Mi (smi2)

Design Questions:

1. For the one signal option, we used SIGUSR1 to represent both bits 0 and 1. We used SIGALRM to send a signal to itself every 0.5 seconds. The messages are converted into ASCII code, then into binary code. If we wanted to send bit 0, then we sent 1 SIGUSR1 in 0.5 seconds. If we wanted to send bit 1, then we sent 5 SIGUSR1 in 0.5 seconds. After the alarm's time is up, depending on the number of signals we have received, it is interpreted as either a 0 (if 1 signal was received) or a 1 (if 5 signals was received).

2. For the two signal option, we used SIGUSR1 to represent bit 0 and SIGUSR2 to represent bit 1. The messages are converted into ASCII code, then into binary code. On the receiver process' side, if a SIGUSR1 is received, then it will be interpreted as a 0, and if a SIGUSR2 is received then it will be interpreted as a 1.

3. We used the line feed that comes after every message as the boundary.

4. We checked for errors by checking that the received signals when converted back to ASCII are valid ASCII codes. 

5. The errors that we are unable to check for are if a valid but unintended ASCII character was received or if the number of characters are shorter or longer than the original message.

6. When a message is received, it enters the signal handler. While it is in there, signals SIGUSR1 and SIGUSR2 are blocked. Once the signal has been handled, the program returns to its place in the main function, and continues where it left off.

7. The only signal we used other than SIGUSR1 and SIGUSR2 was SIGALRM. We used SIGALRM to time the intervals within which SIGUSR1's are received.

8. We saved CPU by leaving the program promptly after the user quits by input ".".


Balance of Work:
Converting from message to signals was implemented by Siyu, converting from signals back to message was implemented by Zhihao. The rest of the implementation, including the single and double options were all programmed together. 


*Our code was tested on the official VM.