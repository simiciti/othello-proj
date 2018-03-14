
CONTRIBUTIONS:
	Week 1:
	Bose:		Implemented basic functions of player class and heuristics
	Nathaniel:	Implemented minimax algorithm. 

	Week 2:
	Bose: 		Implemented more heuristics and experimented with combinations
				of them and depth. 
	Nathaniel:	Implemented alpha-beta pruning and time awareness. Created (currently non-functional)
	transposition table impelmentation.

Tournament Competition Improvements 
The evaluation algorithm was improved in both ability to win and time
usage. 
We modified the minimax algorithm to implement alpha-beta pruning. 
This should increase the efficiency of the search and decrease the 
effective branching factor by eliminating paths known from prior 
information to be inferior to an already found solution.

We implemented iterative deepening to more effectively manage the remaining
time while preserving a reasonable search depth.
We attempted to implement transposition tables to speed up the iterative deepening 
by storing the results of prior searches, eliminating much of the 
redundancy from retreading those prior searches, but debugging the unfamiliar and unexpected
behaviors and interactions of the tables with our code took far longer than expected, 
and we had to remove transposition tables from the final version, even though
they did offer significant time improvements. Had we been aware of the difficulties,
we would've devoted far more time to the "final stretch" than the 6.5 hours we 
put in today.


IMPROVEMENTS TO AI:
	Heuristics experimented with:
		CornerCloseness:    highly values occupying corners and lowly values 
		                    playing beside corners
		Coins:              Number of pieces on board.
		WeightedCoins:      Spaces occupied with static weights dependent on
		                    location of piece.
		Mobility:           Number of moves available.

	Final Version:
	    The final version of the AI looks down 3 moves and uses evaluates
	    positions with a combination of CornerCloseness, Coins, and 
	    Mobility.
