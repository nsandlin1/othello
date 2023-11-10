#include <stdio.h>

// define token
typedef enum {none, white, black} Token;

// initialize  board w/ starting state
Token board[8][8];

// get token name as string
char* getTokenStatus(Token t) {
	switch(t) {
		case none: return "-";
		case white: return "White";
		case black: return "Black";
	}	
}

// initialize board
void initializeBoard(Token b[][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i == 3 && j == 3) || (i == 4 && j == 4)) {
				b[i][j] = white;
			} else if ((i == 3 && j == 4) || (i == 4 && j == 3)) {
				b[i][j] = black;
			} else {
				b[i][j] = none;
			}
		}
	}
}

// print board state
void printBoard(Token b[][8]) {
	printf("\t\e[0;42m     0   1   2   3   4   5   6   7  j \e[0m\n");
	printf("\t\e[0;42m   ---------------------------------  \e[0m\n");
	for (int i = 0; i < 8; i++) {
		printf("\t\e[0;42m %i ", i);
		for (int j = 0; j < 8; j++) {
			printf("\e[0;42m| \e[1;42m%c ", getTokenStatus(b[i][j])[0]);
		}
		printf("\e[0;42m|  \e[0m\n\t\e[0;42m   ---------------------------------  \e[0m\n");
	}
	printf("\t\e[0;42m i                                    \e[0m\n");
}

void printBoard2(Token b[][8]) {
	printf("\t     0   1   2   3   4   5   6   7  j \n");
	printf("\t   ---------------------------------  \n");
	for (int i = 0; i < 8; i++) {
		printf("\t %i ", i);
		for (int j = 0; j < 8; j++) {
			printf("| %c ", getTokenStatus(b[i][j])[0]);
		}
		printf("|  \n\t   ---------------------------------  \n");
	}
	printf("\t i                                    \n");
}

int isPossiblePlay(Token b[][8], Token t) {
	int has_seen_opp = 0;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			has_seen_opp = 0;
			if (b[row][col] != none) {
				continue;
			}
			// check north
			for (int i = row; i >= 0; i--) {
				if (i == row) {
					continue;
				}
				if (t == white) {
					if (b[i][col] == white) {
						if (i == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[i][col] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[i][col] == black) {
						if (i == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[i][col] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check south
			has_seen_opp = 0;
			for (int i = row; i < 8; i++) {
				if (i == row) {
					continue;
				}
				if (t == white) {
					if (b[i][col] == white) {
						if (i == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[i][col] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[i][col] == black) {
						if (i == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[i][col] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check east
			has_seen_opp = 0;
			for (int i = col; i < 8; i++) {
				if (i == col) {
					continue;
				}
				if (t == white) {
					if (b[row][i] == white) {
						if (i == col + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[row][i] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[row][i] == black) {
						if (i == col + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[row][i] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check west
			has_seen_opp = 0;
			for (int i = col; i >= 0; i--) {
				if (i == col) {
					continue;
				}
				if (t == white) {
					if (b[row][i] == white) {
						if (i == col - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[row][i] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[row][i] == black) {
						if (i == col - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[row][i] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check northeast
			has_seen_opp = 0;
			for (int r = row, c = col; r >= 0 && c < 8; c++, r--) {
				if (c == col) {
					continue;
				}
				if (t == white) {
					if (b[r][c] == white) {
						if (r == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[r][c] == black) {
						if (r == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check northwest
			has_seen_opp = 0;
			for (int r = row, c = col; r >= 0 && c >= 0; c--, r--) {
				if (c == col) {
					continue;
				}
				if (t == white) {
					if (b[r][c] == white) {
						if (r == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[r][c] == black) {
						if (r == row - 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check southeast
			has_seen_opp = 0;
			for (int r = row, c = col; r < 8 && c < 8; c++, r++) {
				if (c == col) {
					continue;
				}
				if (t == white) {
					if (b[r][c] == white) {
						if (r == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[r][c] == black) {
						if (r == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
			// check southwest
			has_seen_opp = 0;
			for (int r = row, c = col; r < 8 && c >= 0; c--, r++) {
				if (c == col) {
					continue;
				}
				if (t == white) {
					if (b[r][c] == white) {
						if (r == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				} else {
					if (b[r][c] == black) {
						if (r == row + 1) {
							break;
						} else if (has_seen_opp == 1) {
							return 1;
						}
					} else if (b[r][c] == none) {
						break;
					} else {
						has_seen_opp = 1;
					}
				}
			}
		}
	}
	return 0;
}

// place chip
// assumes given board location is none
// does nothing if there is no valid play (i.e.: board value should remain none)
// O(8n) >:•∫-<
// this code is reducable via while loop methinks... but I twas in a copyin' mood
void placeChip(int row, int col, Token t, Token b[][8]) {
	// if has been played, do nothing
	if (b[row][col] != none) {
		return;
	}
	// check north
	int is_play = 0;
	int has_seen_opp = 0;
	for (int i = row; i >= 0; i--) {
		if (i == row) {
			continue;
		}
		if (t == white) {
			if (b[i][col] == white) {
				if (i == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[i][col] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[i][col] == black) {
				if (i == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[i][col] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int i = row - 1; i >= 0; i--) {
			// printf("i: %i\n", i);
			if (b[i][col] == t) {
				break;
			} else {
				b[i][col] = t;
			}
		}
		b[row][col] = t;
	}

	// check south
	is_play = 0;
	has_seen_opp = 0;
	for (int i = row; i < 8; i++) {
		if (i == row) {
			continue;
		}
		if (t == white) {
			if (b[i][col] == white) {
				if (i == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[i][col] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[i][col] == black) {
				if (i == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[i][col] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int i = row + 1; i < 8; i++) {
			// printf("i: %i\n", i);
			if (b[i][col] == t) {
				break;
			} else {
				b[i][col] = t;
			}
		}
		b[row][col] = t;
	}

	// check east
	is_play = 0;
	has_seen_opp = 0;
	for (int i = col; i < 8; i++) {
		if (i == col) {
			continue;
		}
		if (t == white) {
			if (b[row][i] == white) {
				if (i == col + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[row][i] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[row][i] == black) {
				if (i == col + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[row][i] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int i = col + 1; i < 8; i++) {
			// printf("i: %i\n", i);
			if (b[row][i] == t) {
				break;
			} else {
				b[row][i] = t;
			}
		}
		b[row][col] = t;
	}

	// check west
	is_play = 0;
	has_seen_opp = 0;
	for (int i = col; i >= 0; i--) {
		if (i == col) {
			continue;
		}
		if (t == white) {
			if (b[row][i] == white) {
				if (i == col - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[row][i] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[row][i] == black) {
				if (i == col - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[row][i] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int i = col - 1; i >= 0; i--) {
			if (b[row][i] == t) {
				break;
			} else {
				b[row][i] = t;
			}
		}
		b[row][col] = t;
	}

	// check northeast
	is_play = 0;
	has_seen_opp = 0;
	for (int r = row, c = col; r >= 0 && c < 8; c++, r--) {
		if (c == col) {
			continue;
		}
		if (t == white) {
			if (b[r][c] == white) {
				if (r == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[r][c] == black) {
				if (r == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int r = row - 1, c = col + 1; r >= 0 && c < 8; c++, r--) {
			if (b[r][c] == t) {
				break;
			} else {
				b[r][c] = t;
			}
		}
		b[row][col] = t;
	}

	// check northwest
	is_play = 0;
	has_seen_opp = 0;
	for (int r = row, c = col; r >= 0 && c >= 0; c--, r--) {
		if (c == col) {
			continue;
		}
		if (t == white) {
			if (b[r][c] == white) {
				if (r == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[r][c] == black) {
				if (r == row - 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; c--, r--) {
			if (b[r][c] == t) {
				break;
			} else {
				b[r][c] = t;
			}
		}
		b[row][col] = t;
	}

	// check southeast
	is_play = 0;
	has_seen_opp = 0;
	for (int r = row, c = col; r < 8 && c < 8; c++, r++) {
		if (c == col) {
			continue;
		}
		if (t == white) {
			if (b[r][c] == white) {
				if (r == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[r][c] == black) {
				if (r == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int r = row + 1, c = col + 1; r < 8 && c < 8; c++, r++) {
			if (b[r][c] == t) {
				break;
			} else {
				b[r][c] = t;
			}
		}
		b[row][col] = t;
	}

	// check southwest
	is_play = 0;
	has_seen_opp = 0;
	for (int r = row, c = col; r < 8 && c >= 0; c--, r++) {
		if (c == col) {
			continue;
		}
		if (t == white) {
			if (b[r][c] == white) {
				if (r == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		} else {
			if (b[r][c] == black) {
				if (r == row + 1) {
					break;
				} else if (has_seen_opp == 1) {
					is_play = 1;
					break;
				}
			} else if (b[r][c] == none) {
				break;
			} else {
				has_seen_opp = 1;
			}
		}
	}
	if (is_play) {
		for (int r = row + 1, c = col - 1; r < 8 && c >= 0; c--, r++) {
			if (b[r][c] == t) {
				break;
			} else {
				b[r][c] = t;
			}
		}
		b[row][col] = t;
	}
}

// return if game over (all spots played)
// 1 if over, 0 otherwise
// int isGameOver(Token b[][8]) {
// 	int w_count = 0;
// 	int b_count = 0;
// 	for (int i = 0; i < 8; i++) {
// 		for (int j = 0; j < 8; j++) {
// 			if (b[i][j] == white) {
// 				w_count++;
// 			} else if (b[i][j] == black) {
// 				b_count++;
// 			}
// 		}
// 	}
// 	// if board full or one player has no tokens
// 	if (w_count + b_count == 64 || w_count == 0 || b_count == 0) {
// 		return 1;
// 	} else {
// 		return 0;
// 	}
// }

// return the winner
// white if white wins, black if black wins, none if tie
// Token getWinner(Token b[][8]) {
// 	int w_count = 0;
// 	int b_count = 0;
// 	for (int i = 0; i < 8; i++) {
// 		for (int j = 0; j < 8; j++) {
// 			if (b[i][j] == white) {
// 				w_count++;
// 			} else if (b[i][j] == black) {
// 				b_count++;
// 			}
// 		}
// 	}
// 	if (w_count > b_count) {
// 		return white;
// 	} else if (w_count < b_count) {
// 		return black;
// 	} else {
// 		return none;
// 	}
// }

typedef struct {
	int whites;
	int blacks;
	int nones;
} BoardState;

// return current board state (num of each token/blank spaces)
// must pass num_rows due to array decay
BoardState getBoardState(Token board[][8]) {
	BoardState boardState = {0, 0, 0};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == white) {
				boardState.whites++;
			} else if (board[i][j] == black) {
				boardState.blacks++;
			} else {
				boardState.nones++;
			}
		}
	}
	return boardState;
}


