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
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// https://stackoverflow.com/questions/101439/the-most-efficient-way-to-implement-an-integer-based-power-function-powint-int
// function to return power of integer to another
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

// struct for defing a tile and its heuristic
typedef struct {
    int i;
    int j;
    double h;
} s_Tile;

// heuristic tile value map
int tileValues[8][8] = {
    {50, -20, 5, 5, -20, 50},
    {-20, -50, -2, -2, -50, -20},
    {5, -2, -1, -1, -2, 5},
    {5, -2, -1, -1, -2, 5},
    {-20, -50, -2, -2, -50, -20},
    {50, -20, 5, 5, -20, 50}
};
// define the heuristic
// AI plays as black
// defined as number of black tokens minus number of white tokens
// https://courses.cs.washington.edu/courses/cse573/04au/Project/mini1/RUSSIA/Final_Paper.pdf
double heuristic(Token board[][8]) {
    BoardState boardState = getBoardState(board);
    // printf("board: %i, %i", boardState.whites, boardState.blacks);
    int coin_parity = 100 * (boardState.blacks - boardState.whites) / (boardState.blacks + boardState.whites);
    int tile_bias = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] == black) {
                tile_bias += tileValues[i][j];
            }
        }
    }
    // will eventually implement more
    return coin_parity + tile_bias;
}

// function to deep copy token matrix
Token** deepCopyTokenMatrix(Token board[][8], Token aux_board[][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            aux_board[i][j] = board[i][j];
        }
    }
    return aux_board;
}

// function to deep copy a tile
s_Tile deepCopyTile(s_Tile orig) {
    s_Tile new;
    new.i = orig.i;
    new.j = orig.j;
    new.h = orig.h;
    return new;
}

// function to deep copy a tile array
void deepCopyTileArr(s_Tile *origArr, s_Tile *newArr, int origArrDataLen) {
    for (int i = 0; i < origArrDataLen; i++) {
        newArr[i] = deepCopyTile(origArr[i]);
    }
}

// function to print a tile array
void printTileArr(s_Tile *arr, int len) {
    printf("[ ");
    for (int i = 0; i < len; i++) {
        printf("(%i, %i) ", arr[i].i, arr[i].j);
    }
    printf("]");
}

// function to perform minimax algorithm
s_Tile minimax(Token board[][8], int ply, double alpha, double beta, int maximizing_player, int do_prune, int *state_count, s_Tile curr_path[ply], int curr_path_len, int orig_ply, int debug) {
    // define current bets tile
    s_Tile curr_best_tile;

    // get board state
    BoardState state = getBoardState(board);

    // if there are less openings then ply, set ply to number of openings to avoid infinite loop
    if (state.nones < ply) {
        ply = state.nones;
    }

    // base case
    if (ply == 0) {
        if (debug) {
            printTileArr(curr_path, curr_path_len);
            printf(" %lf\n", curr_best_tile.h);
        }
        curr_best_tile.h = heuristic(board);
        curr_best_tile.i = -1;
        curr_best_tile.j = -1;
        // printf("%i, %i, %lf\n", curr_best_tile.i,  curr_best_tile.j, curr_best_tile.h);
        return curr_best_tile;
    }

    // if ai
    if (maximizing_player) {
        curr_best_tile.h = -DBL_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // printf("max: %i, %i\n", i, j);

                // this could easily be removed by changing place to
                // not check if position is playable but to have separate
                // function
                if (board[i][j] == none) {
                    Token aux_board[8][8];
                    deepCopyTokenMatrix(board, aux_board);
                    // printf("copied board\n");

                    placeChip(i, j, black, aux_board);
                    // printf("placed chip\n");

                    // if it did not place meaning it was unplayable
                    if (aux_board[i][j] == none) {
                        continue;
                    }

                    (*state_count)++;

                    s_Tile new_path[orig_ply];
                    deepCopyTileArr(curr_path, new_path, curr_path_len);
                    new_path[curr_path_len] = (s_Tile) {.i = i, .j = j};
                    
                    s_Tile temp_tile = minimax(aux_board, ply - 1, alpha, beta, 0, do_prune, state_count, new_path, curr_path_len+1, orig_ply, debug);

                    if (temp_tile.h > curr_best_tile.h) {
                        curr_best_tile.h = temp_tile.h;
                        curr_best_tile.i = i;
                        curr_best_tile.j = j;
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
        // printf("%i, %i, %lf, %i\n", curr_best_tile.i,  curr_best_tile.j, curr_best_tile.h, ply);
        return curr_best_tile;
    } 
    // if not ai
    else {
        curr_best_tile.h = DBL_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // printf("min: %i, %i\n", i, j);
                // this could easily be removed by changing place to
                // not check if position is playable but to have separate
                // function
                if (board[i][j] == none) {
                    Token aux_board[8][8];
                    deepCopyTokenMatrix(board, aux_board);

                    placeChip(i, j, white, aux_board);

                    // if it did not place meaning it was unplayable
                    if (aux_board[i][j] == none) {
                        continue;
                    }

                    (*state_count)++;

                    s_Tile new_path[orig_ply];
                    deepCopyTileArr(curr_path, new_path, curr_path_len);
                    new_path[curr_path_len] = (s_Tile) {.i = i, .j = j};
                    
                    s_Tile temp_tile = minimax(aux_board, ply - 1, alpha, beta, 1, do_prune, state_count, new_path, curr_path_len+1, orig_ply, debug);

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

// test
Token temmm[8][8] = {
    {black, black, black, black, black, black, black, black},
    {black, black, black, black, black, black, black, black},
    {black, black, black, white, black, black, white, black},
    {black, white, black, black, black, black, black, black},
    {black, white, black, black, black, black, black, black},
    {black, white, black, black, black, white, black, black},
    {black, black, black, black, white, black, white, black},
    {black, black, black, black, black, black, none, none},
};

// main part of program
int main() {

    // instantiating control variables
    int trace = 0;
    int f;
    if (trace) {
        f = open("gametrace.txt", O_RDWR|O_CREAT|O_APPEND, 0600);
    }

    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    printf("\nWelcome to Othello!!!\n");
    printf("Would you like to play 1 player or 2 player? ");

    // get number of players
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

    // get player color preference
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

    // if ai is playing
    int do_prune;
    int ply;
    int debug;
    if (num_players == 1) {
        // to prune or not to prune
        printf("Would you like to use pruning? Yes (1) or No (0): ");
        while (1) {
            fgets(buffer, 99, stdin);
            num_items = sscanf(buffer, "%i", &do_prune);
            if (num_items != 1 || (do_prune != 1 && do_prune != 0)) {
                printf("\nInvalid input.\n");
                continue;
            } else {
                break;
            }
        }

        // get ai ply number
        printf("Enter your AI ply number: ");
        while (1) {
            fgets(buffer, 99, stdin);
            num_items = sscanf(buffer, "%i", &ply);
            if (num_items != 1 || (ply < 1)) {
                printf("\nInvalid input.\n");
                continue;
            } else {
                break;
            }
        }

        // debug or not
        printf("Would you like to play in debug mode? Yes (1) or No (0): ");
        while (1) {
            fgets(buffer, 99, stdin);
            num_items = sscanf(buffer, "%i", &debug);
            if (num_items != 1 || (debug != 1 && debug != 0)) {
                printf("\nInvalid input.\n");
                continue;
            } else {
                break;
            }
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
    if (player_color == 1) {
        initializeBoard(board, 1);
    } else {
        initializeBoard(board, 0);
    }
    // for (int i = 0; i < 8; i++) {
    //     for (int j = 0; j < 8; j++) {
    //         board[i][j] = temmm[i][j];
    //     }
    // }

    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    int row, col;
    int curr_player_turn = 1; 
    // int curr_player_turn = 2;

    // game loop
    while (1) {
        Token chip_color;
        if (curr_player_turn == 1) {
            chip_color = white;
        } else {
            chip_color = black;
        }
        // if player can play
        if (!isPossiblePlay(board, chip_color)) {
            // get board state
            BoardState boardState = getBoardState(board);
            if (boardState.nones != 0) {
                // if not, next player's turn
                curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
                printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
                continue;
            } else {
                // board full, get winner
                if (boardState.whites > boardState.blacks) {
                    if (player_color == 1) {
                        printf("White wins!\n\n");
                    } else {
                        printf("Black wins!\n\n");
                    }
                    break;
                } else if (boardState.whites < boardState.blacks) {
                    if (player_color == 1) {
                        printf("Black wins!\n\n");
                    } else {
                        printf("White wins!\n\n");
                    }
                    break;
                } else {
                    printf("There has been a tie.\n\n");
                    break;
                }
            }
        }

        // print board
        if (player_color == 1) {
            printBoard(board, 1);
        } else {
            printBoard(board, 0);
        }

        // for file recording
        if (trace) {
            int save_out = dup(fileno(stdout));
            dup2(f, fileno(stdout));
            if (player_color == 1) {
                printBoard2(board, 1);
            } else {
                printBoard2(board, 0);
            }
            fflush(stdout);
            dup2(save_out, fileno(stdout));
            close(save_out);
        }

        // show scores
        BoardState state = getBoardState(board);
        if (player_color == 1) {
            printf("Score:\n\twhite: %i\n\tblack: %i\n", state.whites, state.blacks);
        } else {
            printf("Score:\n\twhite: %i\n\tblack: %i\n", state.blacks, state.whites);
        }

        // get user input
        if (num_players == 1) {
            if (curr_player_turn == 1) {
                while(1) {
                    printf("\nEnter where you want to play your chip in the form <i, j>: ");
                    fgets(buffer, 99, stdin);
                    num_items = sscanf(buffer, "%i, %i", &row, &col);
                    if (num_items != 2) {
                        printf("Incorrect format.\n");
                        continue;
                    }
                    if (row == -1 && col == -1) {
                        if (do_prune == 0) {
                            do_prune = 1;
                            printf("pruning enabled\n");
                        } else {
                            do_prune = 0;
                            printf("pruning disabled\n");
                        }
                        continue;
                    }
                    if (row == -2 && col == -2) {
                        if (debug == 0) {
                            debug = 1;
                            printf("debug enabled\n");
                        } else {
                            debug = 0;
                            printf("debug disabled\n");
                        }
                        continue;
                    }
                    if (debug == 1) {
                        printf("i: %i | j; %i, num_itmes: %i\n", row, col, num_items);
                    }

                    if (board[row][col] != none) {
                        printf("Invalid Play.\n");
                        continue;
                    }
                    break;
                }
            } else {
                // computer play
                s_Tile bestPlay;

                int *state_count;
                int the_count = 0;
                state_count = &the_count;

                s_Tile curr_path[ply];
                int curr_path_len = 0;
                bestPlay = minimax(board, ply, -DBL_MAX, DBL_MAX, 1, do_prune, state_count, curr_path, curr_path_len, ply, debug);

                if (debug) {
                    printf("best play: (%i, %i), (%lf)\n", bestPlay.i, bestPlay.j, bestPlay.h);
                    printf("states visited: %i\n", *state_count);
                }

                placeChip(bestPlay.i, bestPlay.j, black, board);
                curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
                continue;
            }
        } else {
            printf("Player %i, enter where you would like to play in the form <i, j>: ", curr_player_turn);
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
        }
        // place the chip
        if (curr_player_turn == 1) {
            if (player_color == 1) {
                printf("placing chip white\n");
            } else {
                printf("placing chip black\n");
            }
            placeChip(row, col, white, board);
        } else {
            if (player_color == 1) {
                printf("placing chip black\n");
            } else {
                printf("placing chip white\n");
            }
            placeChip(row, col, black, board);
        }

        // if chip was not placed
        if (board[row][col] == none) {
            printf("Invalid move.\n");
            continue;
        }

        // show winner if applicable
        BoardState boardState = getBoardState(board);
        if (boardState.nones != 0) {
            curr_player_turn = curr_player_turn - ipow(-1, curr_player_turn);
            // printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
            continue;
        } else {
            if (boardState.whites > boardState.blacks) {
                if (player_color == 1) {
                    printf("White wins!\n\n");
                } else {
                    printf("Black wins!\n\n");
                }
                if (player_color == 1) {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.whites, state.blacks);
                } else {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.blacks, state.whites);
                }
                break;
            } else if (boardState.whites < boardState.blacks) {
                if (player_color == 1) {
                    printf("Black wins!\n\n");
                } else {
                    printf("White wins!\n\n");
                }
                if (player_color == 1) {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.whites, state.blacks);
                } else {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.blacks, state.whites);
                }
                break;
            } else {
                printf("There has been a tie.\n\n");
                if (player_color == 1) {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.whites, state.blacks);
                } else {
                    printf("Score:\n\twhite: %i\n\tblack: %i\n", state.blacks, state.whites);
                }
                break;
            }
        }
        
    }
    // print board
    if (player_color == 1) {
        printBoard(board, 1);
    } else {
        printBoard(board, 0);
    }
    // for saving to file
    if (trace) {
        int save_out = dup(fileno(stdout));
        dup2(f, fileno(stdout));
        if (player_color == 1) {
            printBoard2(board, 1);
        } else {
            printBoard2(board, 0);
        }
        fflush(stdout);
        dup2(save_out, fileno(stdout));
        close(save_out);
    }
  
    printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");

    return 0;
}