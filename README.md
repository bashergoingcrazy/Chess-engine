## Chess Engine completely written in C++
A completly terminal based application that plays the marvelous game of chess.
Uses **UCI Protocol** to communicate the best moves in terminal can be connected to a gui like chess_arena gnu 

### Features
- Alpha-Beta prunning
- Quiescence
- Iterative Deepening
- Bitboard representation for (most O(1)) calculations unlike arrays.
- Magic Bitboards for Queen, Bishops, and Rooks

### Additional Info
- can play better then me I am 1500 chess.com and 1800 lichess 
- Estimated right now to be 2400 lichess without any opening book addition
- max Depth right now for good gameplay is 7 (need to improve)

## Things to improve
- improve move search time better (increase search depth in less time)
- handle all draws (3-fold, 50 moves, insufficient material)
- Add null move prunning.
- 
  
