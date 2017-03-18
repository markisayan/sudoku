#include <iostream>
#include <vector>
#include <ctime>
#include <exception>


// If this reaches 0 -> the sudoku is solved
int unsolved_cells = 81;
// If this reaches 0 -> the normal algorithm can't solve it
int default_algo_timeout_counter = 81;

void definitize_if_possible(std::vector<std::vector<std::vector<int>>> & new_sudoku, const int i, const int j) {
	// If the number of possible values has reached 1
	// That'll mean that the we have found the value
	if (new_sudoku[i][j][1] == 1) {
		// We just loop through all possible value cells
		for (int l = 2; l < 11; l++) {
			// And if we find the one that's left
			if (new_sudoku[i][j][l] == 1) {
				// We make it definitive answer
				new_sudoku[i][j][0] = l - 1;
				unsolved_cells--;
			}
		}
	}
}

std::vector<std::vector<std::vector<int>>> get_new_sudoku(const std::vector<std::vector<int>> original_sudoku) {
	std::vector<std::vector<std::vector<int>>> new_sudoku (9, std::vector<std::vector<int>>(9, std::vector<int> (11))) ;

	/* 
	Setting up the sudoku the following way
		* We create a 3 dimentional array (new_sudoku)

		* We add one more dimension to track which value
		can potentially be written in the original sudoku's cell (buckets)

		* The first cell (0th) of the 3rd dimension is initially 0
		if we decide that sudoku's cell should contain a particular number we
		save it in there
		Ex. Let's say the cell with coordinates (8,2) should contain 5
		We'll get:

			new_sudoku[8][2][0] = 5

		* The second cell (1st) will contain the number of possible values of the cell

		* If a we still don't know what number should be written in the sudoku's first cell, 
		the cells with potential-value indices (plus one) are going to contain 1
		Ex. Let's say the original sudoku's cell with coordinates (1,4) could contain values 5, 8 or 9. 
		We'll get:
			
			new_sudoku[1][4][0] = 0 // Actual value not chosen yet
			new_sudoku[1][4][1] = 3 // Number of potential values

			new_sudoku[1][4][2] = 0
			new_sudoku[1][4][3] = 0
			new_sudoku[1][4][4] = 0
			new_sudoku[1][4][5] = 0
			new_sudoku[1][4][6] = 1
			new_sudoku[1][4][7] = 0
			new_sudoku[1][4][8] = 0
			new_sudoku[1][4][9] = 1
			new_sudoku[1][4][10] = 1
	*/
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (original_sudoku[i][j] != 0) {
				// If the cell value is predetermined
				// We write the value in the new_sudoku's 0th cell
				new_sudoku[i][j][0] = original_sudoku[i][j];
				unsolved_cells--;
			}
			else
			{
				// Otherwise we fill the first cell with 0 (not defined yet)
				new_sudoku[i][j][0] = 0;
				// And the number of possible values is set to 9
				new_sudoku[i][j][1] = 9;
				// And we fill all the buckets with 1
				for (int k = 2; k < 11; k++)
					new_sudoku[i][j][k] = 1;
			}
		}
	}

	return new_sudoku;
}

void exclude_ver_horiz(std::vector<std::vector<std::vector<int>>> & new_sudoku, int i, int j) {
	// Vertical/horizontal check
	for (int k = 0; k < 9; k++) {
		// We check vertically
		if (new_sudoku[k][j][0] != 0 && k != i) {
			if (new_sudoku[i][j][new_sudoku[k][j][0] + 1] != 0) {
				// We "remove" the value from the potential value list
				new_sudoku[i][j][new_sudoku[k][j][0] + 1] = 0;
				// We reduce the number of possible values by 1
				new_sudoku[i][j][1]--;
				default_algo_timeout_counter = 81;
			}
		}
		// Then we check horizontally
		if (new_sudoku[i][k][0] != 0 && k !=j ) {
			if (new_sudoku[i][j][new_sudoku[i][k][0] + 1] != 0) {
				new_sudoku[i][j][new_sudoku[i][k][0] + 1] = 0;
				new_sudoku[i][j][1]--;
				default_algo_timeout_counter = 81;
			}
		}
	}
}

void exclude_block(std::vector<std::vector<std::vector<int>>> & new_sudoku, int i, int j) {
	// Block check
	for (int k = 0; k <= 2; k++) {
		for (int l = 0; l <= 2; l++) {
			if (i != k && j != l) {
				int x = 3 * (i / 3) + k;
				int y = 3 * (j / 3) + l;

				if (new_sudoku[x][y][0] != 0) {
					if (new_sudoku[i][j][new_sudoku[x][y][0] + 1] != 0) {
						new_sudoku[i][j][new_sudoku[x][y][0] + 1] = 0;
						new_sudoku[i][j][1]--;
						default_algo_timeout_counter = 81;
					}
				}
			}
		}
	}
}

std::vector<std::vector<int>> new_sudoku_to_normal(const std::vector<std::vector<std::vector<int>>> & new_sudoku) {
	std::vector<std::vector<int>> solved_sudoku(9, std::vector<int>(9));

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			solved_sudoku[i][j] = new_sudoku[i][j][0];
		}
	}

	return solved_sudoku;
}

bool is_valid(const std::vector<std::vector<std::vector<int>>> & new_sudoku, int i, int j) {
	// Vertical/horizontal check for validity
	for (int k = 0; k < 9; k++) {
		if (new_sudoku[k][j][0] == new_sudoku[i][j][0] && k != i)
			return false;

		if (new_sudoku[i][k][0] == new_sudoku[i][j][0] && k != j)
			return false;

	}
	// Block check for validity
	for (int k = 0; k <= 2; k++) {
		for (int l = 0; l <= 2; l++) {
			int x = 3 * (i / 3) + k;
			int y = 3 * (j / 3) + l;

			if (new_sudoku[x][y][0] == new_sudoku[i][j][0] && x != i && y != j)
				return false;
		}
	}

	return true;
}


// This is a basic recursive backtracking / brute force algorithm
void solve_recursively(std::vector<std::vector<std::vector<int>>> & new_sudoku) {
	bool stop = false;

	// Let's define a base case
	if (unsolved_cells == 0)
		return;

	// We go through all cells
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			// Check if it's empty
			if (new_sudoku[i][j][0] == 0) {
				// Let's assume we successfully solved the problem for this cell
				unsolved_cells--;
				// We go over every possible value of the current cell
				for (int k = 2; k < 11; k++) {
					// We check if it's not 0
					if (new_sudoku[i][j][k] == 1) {
						// We assume it's correct
						new_sudoku[i][j][0] = k - 1;
						// We check if it doesn't violate sudoku's rules
						if (is_valid(new_sudoku, i, j)) {
							// Then we call the same function recursively
							solve_recursively(new_sudoku);

							if (unsolved_cells == 0)
								return;
						}
						new_sudoku[i][j][0] = 0;
					}
				}
				unsolved_cells++;
				return;
			}
		}
	}
}

bool init_validate(const std::vector<std::vector<std::vector<int>>> & new_sudoku) {
	for (int i = 0; i < 9; i++) 
		for (int j = 0; j < 9; j++) 
			if (new_sudoku[i][j][0] != 0)
				if (!is_valid(new_sudoku, i, j))
					return false;
	
	return true;
}

std::vector<std::vector<int>> solve(const std::vector<std::vector<int>> & original_sudoku ) {
	auto new_sudoku = get_new_sudoku(original_sudoku);
	if (!init_validate(new_sudoku))
		return original_sudoku;

	time_t timer = clock();

	while (unsolved_cells > 0) {
		// After the initialization is done we start solving the thing
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				// If the cell is not defined yet
				if (new_sudoku[i][j][0] == 0) {
					exclude_ver_horiz(new_sudoku, i, j);
					exclude_block(new_sudoku, i, j);
					definitize_if_possible(new_sudoku, i, j);
					default_algo_timeout_counter--;
				}
			}
		}

		// If we can't solve it rightaway we pass the half solved sudoku to "bruteforce" function
		if (default_algo_timeout_counter <= 0) {
			solve_recursively(new_sudoku);
			break;
			//throw exception();
		}
	}


	return new_sudoku_to_normal(new_sudoku);
}


int main() {
	std::cout << "Input sudoku cell values (as empty cell = 0)" << std::endl << std::endl;
	std::vector<std::vector<int>> sudoku (9, std::vector<int>(9));
	
	int n;
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cin >> n;
			sudoku[i][j] = n;
		}
	}
	

	time_t timer = clock();

	sudoku = solve(sudoku);

	if (unsolved_cells != 0) {
		std::cout << "Can't be solved!" << std::endl;
		return 0;
	}

	std::cout << std::endl << std::endl << "Sudoku solved in " << (clock() - timer) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;
	std::cout << "Result" << std::endl;
	std::cout << "==================================================" << std::endl << std::endl;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			std::cout << sudoku[i][j] << " ";
			if ((j+1) % 3 == 0)
				std::cout << "    ";
		}
		std::cout << std::endl;
		if ((i+1) % 3 == 0)
			std::cout << std::endl;
	}

}