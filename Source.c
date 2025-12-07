#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define EMPTY 0
#define BLACK -1
#define WHITE 0

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int dx;
    int dy;
} Direction;

Direction directions[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int** create_field(int rows, int cols) {
    int** field = (int**)malloc(rows * sizeof(int*));
    
    for (int i = 0; i < rows; i++) {
        field[i] = (int*)malloc(cols * sizeof(int));
        
        for (int j = 0; j < cols; j++) {
            field[i][j] = EMPTY;
        }
    }
    
    return field;
}

void free_field(int** field, int rows) {
    for (int i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

int is_valid(int x, int y, int rows, int cols) {
    if (x >= 0 && x < rows && y >= 0 && y < cols) 
        return 1; 
    else 
        return 0;  
}

int can_place_black(int** field, int x, int y, int rows, int cols) {
    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i].dx;
        int ny = y + directions[i].dy;
        
        if (is_valid(nx, ny, rows, cols) && field[nx][ny] == BLACK) {
            return 0;
        }
    }
    
    return 1;
}

int draw_line(int** field, int x, int y, Direction dir, int rows, int cols, int id) {
    int len = 0;
    
    int cx = x + dir.dx;
    int cy = y + dir.dy;
    
    int max_len = 2 + rand() % 3;
    
    while (is_valid(cx, cy, rows, cols) == 1 && 
           field[cx][cy] == EMPTY && len < max_len) {
        field[cx][cy] = id;
        len++;
        
        cx += dir.dx;
        cy += dir.dy;
    }
    
    return len;
}

int** generate_puzzle(int rows, int cols) {
    int** solution = create_field(rows, cols);
    
    int black_count = 3 + rand() % ((rows * cols) / 6);
    
    Point* blacks = (Point*)malloc(black_count * sizeof(Point));
    int* lengths = (int*)calloc(black_count, sizeof(int));
    
    int placed = 0;
    int attempts = 0, max_attempts = rows * cols * 3;
    
    while (placed < black_count && attempts < max_attempts) {
        int x = rand() % rows;
        int y = rand() % cols;
        
        if (solution[x][y] == EMPTY && can_place_black(solution, x, y, rows, cols) == 1) {
            solution[x][y] = BLACK;
            
            blacks[placed].x = x;
            blacks[placed].y = y;
            
            placed++;
        }
        
        attempts++;
    }
    
    if (placed < black_count) {
        black_count = placed;
    }
    
    for (int i = 0; i < black_count; i++) {
        int dirs_to_draw[4] = {0, 1, 2, 3};
        
        for (int j = 0; j < 4; j++) {
            int k = rand() % 4;
            
            int temp = dirs_to_draw[j];
            dirs_to_draw[j] = dirs_to_draw[k];
            dirs_to_draw[k] = temp;
        }
        
        int lines_to_draw = 1 + rand() % 2;
        
        for (int j = 0; j < lines_to_draw; j++) {
            Direction dir = directions[dirs_to_draw[j]];
            
            lengths[i] += draw_line(solution, blacks[i].x, blacks[i].y, dir, rows, cols, i+1);
        }
        
        if (lengths[i] == 0) {
            Direction dir = directions[rand() % 4];
            lengths[i] = draw_line(solution, blacks[i].x, blacks[i].y, dir, rows, cols, i+1);
        }
        
        if (lengths[i] > 4) lengths[i] = 4;
    }
    
    int** puzzle = create_field(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (solution[i][j] == BLACK) {
                int found = 0;
                for (int k = 0; k < black_count && found == 0; k++) {
                    if (blacks[k].x == i && blacks[k].y == j) {
                        puzzle[i][j] = lengths[k];
                        found = 1;
                    }
                }
                
                if (found == 0) {
                    puzzle[i][j] = 2;
                }
            } else {
                puzzle[i][j] = WHITE;
            }
        }
    }
    
    free(blacks);
    free(lengths);
    free_field(solution, rows);
    
    return puzzle;
}

void print_field(int** field, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (field[i][j] == WHITE) {
                printf("*");
            } else {
                printf("%d", field[i][j]);
            }
            
            printf(" ");
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    int rows, cols;
    
    scanf("%d %d", &rows, &cols);
    
    if (rows < 3 || cols < 3) {
        return 1;
    }
    
    for (int gen = 0; gen < 3; gen++) {
        int** puzzle = generate_puzzle(rows, cols);
        
        if (puzzle == NULL) {
            return 1;
        }
        
        print_field(puzzle, rows, cols);
        
        if (gen < 2) {
            printf("\n");
        }
        
        free_field(puzzle, rows);
    }
    
    return 0;
}