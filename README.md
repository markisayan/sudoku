# Sudoku solver

## How it works

It uses 2 methods to solve the puzzle. First, a simple reasoning/elimination algorithm is used to solve the puzzle, by elmininating all the values that can't be written in a particular cell. Every cell is checked for vertical/horizontal/in-block repetitions. The process continues until the number of possible values for every cell reaches 1. If the puzzle isn't solved, a backtracking algorithm is called, which recursively inserts every remaining possible value that's left after applying the first algo. Then it checks if sudoku is still valid. Algorithm keeps going until there are no empty cells left.

## Complexity 
* Elimination algorithm: O(n^2)
* Backgracking algorithm: O(n^m), where m is the number of the remaining empty cells

## In practice
  I ran the code on the [cpp.sh](http://cpp.sh) and input some of the most difficult sudoku puzzles out there. Here are the results
  
  * AI Escargot ~0.002 seconds
  * Golden Nugget (SE 11.9) ~0.066 seconds
  * Arto Inkala's puzzle ~0.017 seconds
  * Impossible #49 ~0.247 seconds

## Note
This uses C++11 functionality.
