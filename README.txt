Group Member Contributions
Arjun

Nathaniel 

Tournament Competition Improvements 
The evaluation algorithm was improved in both ability to win and time
usage. 
We modified the minimax algorithm to implement alpha-beta pruning. 
This should increase the efficiency of the search and decrease the 
effective branching factor by eliminating paths known from prior 
information to be inferior to an already found solution.

We implemented iterative deepening to more effectively manage the remaining
time while preserving a reasonable search depth.
We implemented transposition tables to speed up the iterative deepening 
by storing the results of prior searches, eliminating much of the 
redundancy from retreading those prior searches. 
