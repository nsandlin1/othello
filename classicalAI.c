// Name: Novi Sandlin
// SSID: 10389876
// Date: 11/1/2023
// Assignment #: 3
// Description: This program allows pvp othello gameplay as well as pvc gameplay
// the algorithm implemented is mini-max, or minimizing the maximum score of you opponent.
// it is easily configurable to maxi-min
// ply is variable

// imports
#include "othello.c"
#include <math.h>
#include <float.h>

// https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

int debug = 1;

typedef struct {
    int i;
    int j;
    double h;
} s_Tile;

// define the heuristic
// AI plays as black
// defined as number of black tokens minus number of white tokens
// https://courses.cs.washington.edu/courses/cse573/04au/Project/mini1/RUSSIA/Final_Paper.pdf
double heuristic(Token board[][8]) {
    BoardState boardState = getBoardState(board);
    int coin_parity = 100 * (boardState.blacks - boardState.whites) / (boardState.blacks + boardState.whites);
    // will eventually implement more
    return coin_parity;
}

Token** deepCopyTokenMatrix(Token board[][8], Token aux_board[][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            aux_board[i][j] = board[i][j];
        }
    }
    return aux_board;
}


s_Tile minimax(Token board[][8], int ply, double alpha, double beta, int maximizing_player, int player_color, int do_prune) {
    s_Tile curr_best_tile;

    printf("%i\n", ply);

    if (ply == 0) {
        curr_best_tile.h = heuristic(board);
        curr_best_tile.i = -1;
        curr_best_tile.j = -1;
        return curr_best_tile;
    }

    if (maximizing_player) {
        curr_best_tile.h = -DBL_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // this could easily be removed by changing place to
                // not check if position is playable but to have separate
                // function
                if (board[i][j] == none) {
                    Token aux_board[8][8];
                    deepCopyTokenMatrix(board, aux_board);
                    // printf("copied board\n");

                    if (player_color == 1) {
                        placeChip(i, j, black, aux_board);
                    } else {
                        placeChip(i, j, white, aux_board);
                    }
                    // printf("placed chip\n");

                    // if it did not place meaning it was unplayable
                    if (aux_board[i][j] == none) {
                        continue;
                    }
                    
                    s_Tile temp_tile = minimax(aux_board, ply - 1, alpha, beta, 0, player_color, do_prune);

                    if (temp_tile.h > curr_best_tile.h) {
                        curr_best_tile.h = temp_tile.h;
                        curr_best_tile.i = i;
                        curr_best_tile.j = j;
                    }
                    if (ply == 2) {
                        printf("%i, %i, %lf\n", i, j, temp_tile.h);
                    }
                    if (do_prune) {
                        if (temp_tile.h > alpha) {
                            alpha = temp_tile.h;
                        }
                    }
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            if (beta <= alpha) {
                break;
            }
        }
        return curr_best_tile;
    } else {
        curr_best_tile.h = DBL_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // this could easily be removed by changing place to
                // not check if position is playable but to have separate
                // function
                if (board[i][j] == none) {
                    Token aux_board[8][8];
                    deepCopyTokenMatrix(board, aux_board);

                    if (player_color == 1) {
                        placeChip(i, j, white, aux_board);
                    } else {
                        placeChip(i, j, black, aux_board);
                    }

                    // if it did not place meaning it was unplayable
                    if (aux_board[i][j] == none) {
                        continue;
                    }

                    
                    s_Tile temp_tile = minimax(aux_board, ply - 1, alpha, beta, 1, player_color, do_prune);
                    
                    if (temp_tile.h < curr_best_tile.h) {
                        curr_best_tile.h = temp_tile.h;
                        curr_best_tile.i = i;
                        curr_best_tile.j = j;
                    }
                    if (do_prune) {
                        if (temp_tile.h < beta) {
                            beta = temp_tile.h;
                        }
                    }
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            if (beta <= alpha) {
                break;
            }
        }
        return curr_best_tile;
    }
}

int main() {

    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("\nWelcome to Othello!!!\n");
    printf("Would you like to play 1 player or 2 player? ");

    char buffer[100];
    int num_players;
    int num_items;
    while (1) {
        fgets(buffer, 99, stdin);
        num_items = sscanf(buffer, "%i", &num_players);
        if (num_items != 1 || (num_players != 1 && num_players != 2)) {
            printf("\nInvalid input.\n");
            continue;
        } else {
            break;
        }
    }

    // player color equals 1 means active player is white
    printf("Would you like to play white (1) or black (2)? ");
    int player_color;
    while (1) {
        fgets(buffer, 99, stdin);
        num_items = sscanf(buffer, "%i", &player_color);
        if (num_items != 1 || (player_color != 1 && player_color != 2)) {
            printf("\nInvalid input.\n");
            continue;
        } else {
            break;
        }
    }

    // to prune or not to prune
    printf("Would you like to use pruning? Yes (1) or No (2). ");
    int do_prune;
    while (1) {
        fgets(buffer, 99, stdin);
        num_items = sscanf(buffer, "%i", &do_prune);
        if (num_items != 1 || (do_prune != 1 && do_prune != 2)) {
            printf("\nInvalid input.\n");
            continue;
        } else {
            break;
        }
    }

    if (num_players == 1) {
        if (player_color == 1) {
            printf("You are white. Computer is Black.\n");
        } else {
            printf("You are black. Computer is white.\n");
        }
    } else {
        if (player_color == 1) {
            printf("Player 1 is white. Player Two is Black.\n");
        } else {
            printf("Player 1 is black. Player Two is White.\n");
        }
    }

    // initialize  board w/ starting state
    // initializeBoard(board);
    Token board[8][8];
    initializeBoard(board);

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    int row, col;
    int curr_player_turn = 1;
    while (1) {
        Token chip_color;
        if (curr_player_turn == 1) {
            if (player_color == 1) {
                chip_color = white;
            } else {
                chip_color = black;
            }
        } else {
            if (player_color == 1) {
                chip_color = black;
            } else {
                chip_color = white;
            }
        }
        if (!isPossiblePlay(board, chip_color)) {
            BoardState boardState = getBoardState(board);
            if (boardState.nones != 0) {
                curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                continue;
            } else {
                if (boardState.whites > boardState.blacks) {
                    printf("White wins!\n\n");
                    break;
                } else if (boardState.whites < boardState.blacks) {
                    printf("Black wins!\n\n");
                    break;
                } else {
                    printf("There has been a tie.\n\n");
                    break;
                }
            }
        }
        printBoard(board);
        if (num_players == 1) {
            if (curr_player_turn == 1) {
                printf("\nEnter where you want to play your chip in the form <i, j>: ");
                fgets(buffer, 99, stdin);
                num_items = sscanf(buffer, "%i, %i", &row, &col);
                if (num_items != 2) {
                    printf("Incorrect format.\n");
                    continue;
                }
                if (debug == 1) {
                    printf("i: %i | j; %i, num_itmes: %i\n", row, col, num_items);
                }

                if (board[row][col] != none) {
                    printf("Invalid Play.\n");
                    continue;
                }
            } else {
                // computer play
                s_Tile bestPlay;

                bestPlay = minimax(board, 3, -DBL_MAX, DBL_MAX, 1, player_color, do_prune);

                printf("best play: %i, %i, %lf\n", bestPlay.i, bestPlay.j, bestPlay.h);
                if (player_color == 1) {
                    placeChip(bestPlay.i, bestPlay.j, black, board);
                } else {
                    placeChip(bestPlay.i, bestPlay.j, white, board);
                }
                curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
                continue;
            }
        } else {
            printf("Player %i, enter where you would like to play in the form <i, j>: ", curr_player_turn);
        }
        if (curr_player_turn == 1) {
            if (player_color == 1) {
                printf("placing chip white\n");
                placeChip(row, col, white, board);
            } else {
                printf("placing chip black\n");
                placeChip(row, col, black, board);
            }
        } else {
            if (player_color == 1) {
                printf("placing chip black\n");
                placeChip(row, col, black, board);
            } else {
                printf("placing chip white\n");
                placeChip(row, col, white, board);
            }
        }

        // if chip was not placed
        if (board[row][col] == none) {
            printf("Invalid move.\n");
            continue;
        }

        BoardState boardState = getBoardState(board);
        if (boardState.nones != 0) {
            curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
            // printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            continue;
        } else {
            if (boardState.whites > boardState.blacks) {
                printf("White wins!\n\n");
                break;
            } else if (boardState.whites < boardState.blacks) {
                printf("Black wins!\n\n");
                break;
            } else {
                printf("There has been a tie.\n\n");
                break;
            }
        }
        
    }

    printBoard(board);
    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

    return 0;
}