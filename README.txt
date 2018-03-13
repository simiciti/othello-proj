CONTRIBUTIONS:
	Week 1:
	Bose:		Implemented basic functions of player class and heuristics
	Nathaniel:	Implemented minimax algorithm. 

	Week 2:
	Bose: 		Implemented more heuristics and experimented with combinations
				of them and depth. 
	Nathaniel:	Implemented alpha-beta pruning and time awareness. 


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