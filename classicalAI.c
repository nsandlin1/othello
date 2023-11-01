// the algorithm implemented is mini-max, or minimizing the maximum score of you opponent.
// it is easily configurable to maxi-min
// ply is variable

// imports
#include "othello.c"

int debug = 1;

typedef struct {
    int i;
    int j;
} s_Tile;

// define the heuristic
// AI plays as black
// defined as number of black tokens minus number of white tokens
int heuristic(Token board[][8]) {
    BoardState boardState = getBoardState(board);
    return boardState.blacks - boardState.whites;
}

Token** deepCopyTokenMatrix(Token board[][8], Token aux_board[][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            aux_board[i][j] = board[i][j];
        }
    }
    return aux_board;
}

int minimax(Token board[][8], int ply) {
    Token aux_board[8][8];
    deepCopyTokenMatrix(board, aux_board);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            // check if is adjacent to another tile
            for (int p = 0; p < ply; p++) {

            }
        }
    }
}

int main() {

    printf("\nWelcome to Othello!!!\n\n");

    // initialize  board w/ starting state
    // initializeBoard(board);
    Token board[8][8];
    initializeBoard(board);

    char buffer[100];
    int row, col;
    while (1) {
        printBoard(board);
        printf("\nEnter where you want to play your chip in the form <i, j>: ");
        fgets(buffer, 99, stdin);
        int num_items = sscanf(buffer, "%i, %i", &row, &col);
        if (num_items != 2) {
            printf("\nIncorrect format.\n\n");
            continue;
        }
        if (debug == 1) {
            printf("i: %i | j; %i, num_itmes: %i\n", row, col, num_items);
        }

        placeChip(row, col, white, board);
        printBoard(board);

        // if chip was not placed
        if (board[row][col] == none) {
            printf("\nInvalid move.\n\n");
            continue;
        }

        minimax(board, 1);
        
    }

    return 0;
}