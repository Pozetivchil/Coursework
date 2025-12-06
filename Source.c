#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Константы для обозначения клеток
#define EMPTY 0      // Пустая клетка (будет белой)
#define BLACK -1     // Черная клетка (временно, до установки цифры)
#define WHITE 0      // Белая клетка (пустая)

// Структура для хранения клетки
typedef struct {
    int x;
    int y;
} Point;

// Структура для хранения направления линии
typedef struct {
    int dx;
    int dy;
} Direction;

// Направления: вверх, вниз, влево, вправо
Direction directions[4] = {
    {-1, 0},  // вверх
    {1, 0},   // вниз
    {0, -1},  // влево
    {0, 1}    // вправо
};

// Функция для создания пустого поля
int** create_field(int rows, int cols) {
    int** field = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        field[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            field[i][j] = EMPTY;  // Изначально все клетки пустые
        }
    }
    return field;
}

// Функция для освобождения памяти поля
void free_field(int** field, int rows) {
    for (int i = 0; i < rows; i++) {
        free(field[i]);
    }
    free(field);
}

// Функция для проверки, находится ли точка в пределах поля
bool is_valid_point(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

// Функция для проверки, можно ли разместить черную клетку в данной позиции
// Черные клетки не должны касаться друг друга сторонами
bool can_place_black(int** field, int x, int y, int rows, int cols) {
    // Проверяем все соседние клетки (по сторонам)
    int neighbors[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int i = 0; i < 4; i++) {
        int nx = x + neighbors[i][0];
        int ny = y + neighbors[i][1];

        if (is_valid_point(nx, ny, rows, cols) && field[nx][ny] == BLACK) {
            return false;  // Рядом уже есть черная клетка
        }
    }

    return true;
}

// Функция для случайного выбора направления линии
Direction get_random_direction() {
    return directions[rand() % 4];
}

// Функция для рисования линии от черной клетки в заданном направлении
// Возвращает количество белых клеток в линии
int draw_line(int** solution, int x, int y, Direction dir, int rows, int cols, int black_id) {
    int count = 0;
    int current_x = x + dir.dx;
    int current_y = y + dir.dy;

    // Пока не вышли за границы и не встретили черную клетку
    while (is_valid_point(current_x, current_y, rows, cols) &&
        solution[current_x][current_y] == EMPTY) {

        // Помечаем клетку как часть линии от данной черной клетки
        solution[current_x][current_y] = black_id;
        count++;

        // Переходим к следующей клетке в том же направлении
        current_x += dir.dx;
        current_y += dir.dy;
    }

    return count;
}

// Функция для проверки, остались ли неохваченные белые клетки
bool has_uncovered_cells(int** solution, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (solution[i][j] == EMPTY) {
                return true;
            }
        }
    }
    return false;
}

// Функция для поиска неохваченной белой клетки
Point find_uncovered_cell(int** solution, int rows, int cols) {
    Point p = { -1, -1 };

    // Простой поиск первой попавшейся неохваченной клетки
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (solution[i][j] == EMPTY) {
                p.x = i;
                p.y = j;
                return p;
            }
        }
    }

    return p;
}

// Основная функция генерации поля
int** generate_puzzle(int rows, int cols, int* black_cells_count) {
    // Создаем решение (поле с линиями)
    int** solution = create_field(rows, cols);

    // Список черных клеток
    Point* black_cells = NULL;
    int black_count = 0;
    int max_black_cells = (rows * cols) / 8;  // Примерное максимальное количество черных клеток

    if (max_black_cells < 3) max_black_cells = 3;
    black_cells = (Point*)malloc(max_black_cells * sizeof(Point));

    // Этап 1: Размещаем черные клетки
    printf("Размещаем черные клетки...\n");

    int attempts = 0;
    int max_attempts = rows * cols * 10;

    while (black_count < max_black_cells && attempts < max_attempts) {
        int x = rand() % rows;
        int y = rand() % cols;

        // Если клетка пустая и можно разместить черную клетку
        if (solution[x][y] == EMPTY && can_place_black(solution, x, y, rows, cols)) {
            solution[x][y] = BLACK;
            black_cells[black_count].x = x;
            black_cells[black_count].y = y;
            black_count++;
        }

        attempts++;
    }

    printf("Размещено %d черных клеток\n", black_count);

    // Этап 2: Рисуем линии от черных клеток
    printf("Рисуем линии от черных клеток...\n");

    int* line_lengths = (int*)malloc(black_count * sizeof(int));

    // Сначала рисуем основные линии в случайных направлениях
    for (int i = 0; i < black_count; i++) {
        line_lengths[i] = 0;

        // Каждая черная клетка рисует 1-2 линии
        int lines_to_draw = 1 + (rand() % 2);

        for (int j = 0; j < lines_to_draw; j++) {
            Direction dir = get_random_direction();
            int line_len = draw_line(solution, black_cells[i].x, black_cells[i].y,
                dir, rows, cols, i + 1);  // +1 чтобы не было 0
            line_lengths[i] += line_len;
        }
    }

    // Этап 3: Убеждаемся, что все белые клетки охвачены
    printf("Проверяем покрытие всех белых клеток...\n");

    while (has_uncovered_cells(solution, rows, cols)) {
        // Находим неохваченную клетку
        Point uncovered = find_uncovered_cell(solution, rows, cols);
        if (uncovered.x == -1) break;

        // Ищем ближайшую черную клетку
        int closest_black = -1;
        int min_distance = rows + cols;

        for (int i = 0; i < black_count; i++) {
            int distance = abs(black_cells[i].x - uncovered.x) +
                abs(black_cells[i].y - uncovered.y);

            if (distance < min_distance) {
                min_distance = distance;
                closest_black = i;
            }
        }

        if (closest_black != -1) {
            // Прокладываем линию от ближайшей черной клетки к неохваченной
            // Определяем направление
            Direction dir;
            if (black_cells[closest_black].x != uncovered.x) {
                dir.dx = (uncovered.x > black_cells[closest_black].x) ? 1 : -1;
                dir.dy = 0;
            }
            else {
                dir.dx = 0;
                dir.dy = (uncovered.y > black_cells[closest_black].y) ? 1 : -1;
            }

            // Рисуем линию
            int line_len = draw_line(solution, black_cells[closest_black].x,
                black_cells[closest_black].y, dir, rows, cols, closest_black + 1);
            line_lengths[closest_black] += line_len;
        }
    }

    // Этап 4: Создаем головоломку (стираем линии, оставляем только цифры)
    printf("Создаем головоломку...\n");

    int** puzzle = create_field(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (solution[i][j] == BLACK) {
                // Находим, какой это черная клетка и ставим ее цифру
                for (int k = 0; k < black_count; k++) {
                    if (black_cells[k].x == i && black_cells[k].y == j) {
                        // Убеждаемся, что цифра от 1 до 4
                        int number = line_lengths[k];
                        if (number < 1) number = 1;
                        if (number > 4) number = 4;
                        puzzle[i][j] = number;
                        break;
                    }
                }
            }
            else {
                puzzle[i][j] = WHITE;  // Белая клетка
            }
        }
    }

    // Сохраняем количество черных клеток
    *black_cells_count = black_count;

    // Очищаем память
    free(black_cells);
    free(line_lengths);
    free_field(solution, rows);

    printf("Головоломка успешно сгенерирована!\n");

    return puzzle;
}

// Функция для вывода поля
void print_field(int** field, int rows, int cols) {
    printf("\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (field[i][j] == WHITE) {
                printf(" . ");  // Белая клетка
            }
            else if (field[i][j] == BLACK) {
                printf(" # ");  // Черная клетка (не должно быть в финальной головоломке)
            }
            else {
                printf(" %d ", field[i][j]);  // Черная клетка с цифрой
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Функция для вывода решения (для отладки)
void print_solution(int** solution, int rows, int cols) {
    printf("\nРешение (цифры показывают, к какой черной клетке относится линия):\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (solution[i][j] == BLACK) {
                printf(" # ");
            }
            else if (solution[i][j] == EMPTY) {
                printf(" . ");
            }
            else {
                printf(" %d ", solution[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Функция для проверки корректности головоломки
bool validate_puzzle(int** puzzle, int rows, int cols) {
    int black_count = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (puzzle[i][j] > 0) {  // Черная клетка с цифрой
                black_count++;

                // Проверяем, что цифра от 1 до 4
                if (puzzle[i][j] < 1 || puzzle[i][j] > 4) {
                    printf("Ошибка: черная клетка в (%d,%d) имеет недопустимую цифру %d\n",
                        i, j, puzzle[i][j]);
                    return false;
                }

                // Проверяем, что черные клетки не соприкасаются
                int neighbors[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
                for (int k = 0; k < 4; k++) {
                    int nx = i + neighbors[k][0];
                    int ny = j + neighbors[k][1];

                    if (is_valid_point(nx, ny, rows, cols) && puzzle[nx][ny] > 0) {
                        printf("Ошибка: черные клетки соприкасаются в (%d,%d) и (%d,%d)\n",
                            i, j, nx, ny);
                        return false;
                    }
                }
            }
        }
    }

    if (black_count == 0) {
        printf("Ошибка: нет черных клеток\n");
        return false;
    }

    printf("Проверка пройдена: %d черных клеток, все цифры от 1 до 4\n", black_count);
    return true;
}

// Основная функция
int main() {
    srand(time(NULL));  // Инициализация генератора случайных чисел

    int rows, cols;

    printf("=== Генератор головоломки 'Роза ветров' ===\n\n");

    // Запрос размеров поля
    printf("Введите количество строк (рекомендуется 5-10): ");
    scanf("%d", &rows);

    printf("Введите количество столбцов (рекомендуется 5-10): ");
    scanf("%d", &cols);

    if (rows < 3 || cols < 3) {
        printf("Ошибка: минимальный размер поля 3x3\n");
        return 1;
    }

    if (rows > 20 || cols > 20) {
        printf("Предупреждение: большие поля могут генерироваться долго\n");
    }

    int black_cells_count = 0;

    // Генерация головоломки
    int** puzzle = generate_puzzle(rows, cols, &black_cells_count);

    // Проверка корректности
    if (!validate_puzzle(puzzle, rows, cols)) {
        printf("Ошибка при генерации головоломки\n");
        free_field(puzzle, rows);
        return 1;
    }

    // Вывод головоломки
    printf("\n=== Сгенерированная головоломка ===\n");
    printf("Размер: %d x %d\n", rows, cols);
    printf("Черных клеток: %d\n", black_cells_count);
    printf("Условные обозначения: цифра - черная клетка, точка - белая клетка\n");

    print_field(puzzle, rows, cols);

    // Сохранение в файл (опционально)
    char save;
    printf("Сохранить головоломку в файл? (y/n): ");
    scanf(" %c", &save);

    if (save == 'y' || save == 'Y') {
        char filename[100];
        printf("Введите имя файла (например, puzzle.txt): ");
        scanf("%s", filename);

        FILE* file = fopen(filename, "w");
        if (file) {
            fprintf(file, "%d %d\n", rows, cols);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (puzzle[i][j] == WHITE) {
                        fprintf(file, ". ");
                    }
                    else {
                        fprintf(file, "%d ", puzzle[i][j]);
                    }
                }
                fprintf(file, "\n");
            }
            fclose(file);
            printf("Головоломка сохранена в файл %s\n", filename);
        }
        else {
            printf("Ошибка при сохранении файла\n");
        }
    }

    // Освобождение памяти
    free_field(puzzle, rows);

    printf("\nПрограмма завершена. Нажмите Enter для выхода...");
    getchar(); getchar();  // Ожидание нажатия Enter

    return 0;
}