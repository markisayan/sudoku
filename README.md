# Sudoku solver
Not the most effective one out there, but still does the job.



## How it works

This uses 2 methods to solve the puzzle. First it tries to solve it with a simple reasoning algorithm, by excluding all the values that can't be written in a particular cell (that contradict sudoku's rules). For every cell it checks for vertical/horizontal/in-block repetitions. If the number of possible values for a particular cell reaches 1, that'll mean that was the value we were looking for. If that doesn't solve the puzzle, it calls a backtracking algorithm, which inserts recursively every remaining possible value that's left after applying the first method, and checks if the sudoku is valid. It repeats for every cell untill there are not empty cells left.


### Complexity: 
* First algo: O(n^2)
* Second algo: O(n^m), where m is the number of remaining empty cells

## Note
This uses C++11 functionality
