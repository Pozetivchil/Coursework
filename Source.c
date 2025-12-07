#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY_CELL 0
#define BLACK_CELL -1
#define WHITE_CELL 0

struct cell_pos {
    int row;
    int col;
};

struct move_dir {
    int row_change;
    int col_change;
};

struct move_dir all_dirs[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int** make_board(int r, int c) {
    int** board = (int**)malloc(r * sizeof(int*));
    int i, j;
    for (i = 0; i < r; i++) {
        board[i] = (int*)malloc(c * sizeof(int));
        for (j = 0; j < c; j++) {
            board[i][j] = EMPTY_CELL;
        }
    }
    return board;
}

void clear_board(int** board, int r) {
    int i;
    for (i = 0; i < r; i++) {
        free(board[i]);
    }
    free(board);
}

int in_bounds(int row, int col, int r, int c) {
    return (row >= 0 && row < r && col >= 0 && col < c) ? 1 : 0;
}

int check_black_spot(int** board, int row, int col, int r, int c) {
    int dir_idx;
    for (dir_idx = 0; dir_idx < 4; dir_idx++) {
        int next_row = row + all_dirs[dir_idx].row_change;
        int next_col = col + all_dirs[dir_idx].col_change;
        if (in_bounds(next_row, next_col, r, c) && board[next_row][next_col] == BLACK_CELL) {
            return 0;
        }
    }
    return 1;
}

int make_line(int** board, int start_row, int start_col, struct move_dir way, int r, int c, int line_id) {
    int line_size = 0;
    int current_row = start_row + way.row_change;
    int current_col = start_col + way.col_change;
    
    int limit = 2 + rand() % 3;
    
    while (line_size < limit && in_bounds(current_row, current_col, r, c) == 1 && board[current_row][current_col] == EMPTY_CELL) {
        board[current_row][current_col] = line_id;
        line_size++;
        current_row += way.row_change;
        current_col += way.col_change;
    }
    
    return line_size;
}

int** make_game(int r, int c) {
    int** board_state = make_board(r, c);
    
    int black_limit = (r * c) / 6;
    if (black_limit < 3) black_limit = 3;
    if (black_limit > 8) black_limit = 8;
    int black_total = 3 + rand() % (black_limit - 2);
    
    struct cell_pos* blacks = (struct cell_pos*)malloc(black_total * sizeof(struct cell_pos));
    int* line_sizes = (int*)calloc(black_total, sizeof(int));
    
    int placed = 0;
    int tries = 0;
    int max_tries = r * c * 3;
    
    while (placed < black_total && tries < max_tries) {
        int rand_row = rand() % r;
        int rand_col = rand() % c;
        
        if (board_state[rand_row][rand_col] == EMPTY_CELL && check_black_spot(board_state, rand_row, rand_col, r, c) == 1) {
            board_state[rand_row][rand_col] = BLACK_CELL;
            blacks[placed].row = rand_row;
            blacks[placed].col = rand_col;
            placed++;
        }
        tries++;
    }
    
    if (placed < black_total) {
        black_total = placed;
    }
    
    int idx;
    for (idx = 0; idx < black_total; idx++) {
        int dir_list[4] = {0, 1, 2, 3};
        int swap_idx;
        for (swap_idx = 0; swap_idx < 4; swap_idx++) {
            int swap_with = rand() % 4;
            int temp = dir_list[swap_idx];
            dir_list[swap_idx] = dir_list[swap_with];
            dir_list[swap_with] = temp;
        }
        
        int lines_made = 1 + rand() % 2;
        int dir_idx;
        for (dir_idx = 0; dir_idx < lines_made; dir_idx++) {
            struct move_dir chosen_dir = all_dirs[dir_list[dir_idx]];
            line_sizes[idx] += make_line(board_state, blacks[idx].row, blacks[idx].col, chosen_dir, r, c, idx + 1);
        }
        
        if (line_sizes[idx] == 0) {
            struct move_dir random_dir = all_dirs[rand() % 4];
            line_sizes[idx] = make_line(board_state, blacks[idx].row, blacks[idx].col, random_dir, r, c, idx + 1);
        }
        
        if (line_sizes[idx] > 4) {
            line_sizes[idx] = 4;
        }
    }
    
    int** final_puzzle = make_board(r, c);
    int i, j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            if (board_state[i][j] == BLACK_CELL) {
                int found_it = 0;
                int k;
                for (k = 0; k < black_total && found_it == 0; k++) {
                    if (blacks[k].row == i && blacks[k].col == j) {
                        final_puzzle[i][j] = line_sizes[k];
                        found_it = 1;
                    }
                }
                if (found_it == 0) {
                    final_puzzle[i][j] = 2;
                }
            } else {
                final_puzzle[i][j] = WHITE_CELL;
            }
        }
    }
    
    free(blacks);
    free(line_sizes);
    clear_board(board_state, r);
    
    return final_puzzle;
}

void show_board(int** board, int r, int c) {
    int i, j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            if (board[i][j] == WHITE_CELL) {
                printf(".");
            } else {
                printf("%d", board[i][j]);
            }
            if (j != c - 1) {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int rows, cols;
    scanf("%d %d", &rows, &cols);
    
    if (rows < 3 || cols < 3) {
        return 0;
    }
    
    int game_num;
    for (game_num = 0; game_num < 3; game_num++) {
        int** game_board = make_game(rows, cols);
        show_board(game_board, rows, cols);
        if (game_num < 2) {
            printf("\n");
        }
        clear_board(game_board, rows);
    }
    
    return 0;
}