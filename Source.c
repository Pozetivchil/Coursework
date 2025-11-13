#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 10

int grid[SIZE][SIZE] = {
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int solution[SIZE][SIZE];
int temp_grid[SIZE][SIZE];



// Направления: 1-вверх, 2-вниз, 3-влево, 4-вправо, 0-выход

int checkNearBox(int x, int y, int lastMove)
{
    if (lastMove = 4)
    {
        if (x + 1 <= SIZE && solution[x + 1][y] == 4)
        {
            return 1;
        }
    }
    if (lastMove = 3)
    {
        if (x - 1 >= SIZE && solution[x - 1][y] == 3)
        {
            return 1;
        }
    }
    if (lastMove = 2)
    {
        if (y + 1 >= SIZE && solution[x][y + 1] == 2)
        {
            printf("\n\n Solution: %d \n\n", solution[x][y + 1]);
            return 1;
        }
    }
    if (lastMove = 1)
    {
        if (y - 1 >= SIZE && solution[x][y - 1] == 1)
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

void checkField()
{
    printf("\n\n CHECH FIELD \n\n");

    int amountOfWhiteBoxes = 0;
    int correctField = 0;

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (grid[x][y] != 0)
            {
                amountOfWhiteBoxes++;

                int currentState = grid[x][y];
                int count = 0;
                if (x == 0 && y == 0)
                {
                    for (int i = 0; i < 9; i++)
                    {
                        if (checkNearBox(x + i, y, 4))
                        {
                            count++;
                        }
                        else if (checkNearBox(x, y + i, 2))
                        {
                            printf("Just count ++");
                            count++;
                        }
                    }                    
                }
                else if (x == 9 && y == 9)
                {

                }
                else if (x == 0 && y == 9)
                {

                }
                else if (x == 9 && y == 0)
                {

                }
                else if (x = 0 && y != 0 && y != 9) // только то, что справа снизу и вверху 
                {

                }
                else if (x = 9 && y != 0 && y != 9) // только то, что слева, снизу и вверху
                {

                }
                else if (y = 0 && x != 0 && x != 9) // только то, что снизу, срава и слева
                {

                }
                else if (y = 9 && x != 0 && x != 9) // только то, что сверху, срава и слева
                {

                }

                if (count == currentState)
                {
                    correctField++;
                    count = 0;
                }
            }
        }
    }

    printf("\n amountOfWhiteBoxes = %d \n correctField = %d \n", amountOfWhiteBoxes, correctField);

    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            if (x % 10 == 9) printf("\n");
            printf("%d ", solution[x][y]);
        }
    }

    if (correctField == amountOfWhiteBoxes)
    {
        printf("\n\n !!!GOOD JOB!!! \n\n");
        return 1;
    }
}

void initField()
{
    for (int x = 0; x < SIZE; x++)
    {
        for (int y = 0; y < SIZE; y++)
        {
            solution[x][y] = 0;
            temp_grid[x][y] = 0;
        }
    }
}

void printGrid()
{
    int i, j;
    printf("\n");
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (grid[i][j] != 0)
            {
                printf("%d ", grid[i][j]);
            }
            else if (solution[i][j] == 1)
            {
                printf("- ");
            }
            else if (solution[i][j] == 2)
            {
                printf("| ");
            }
            else if (solution[i][j] == 3)
            {
                printf("+ ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

void clearSolution()
{
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            solution[i][j] = 0;
        }
    }
}

void playGame()
{
    int row, col, dir;
    char input[10];

    while (1)
    {
        printGrid();

        printf("Введи координаты и направление (например: 0 1 2 для строки 0, столбца 1, направления вниз):\n");
        printf("Направления: 1-вверх, 2-вниз, 3-влево, 4-вправо, 0-выход\n");
        printf("Введите: ");

        if (scanf("%d %d %d", &row, &col, &dir) != 3)
        {
            printf("Ошибка ввода!\n");
            continue;
        }

        if (dir == 0) break;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE)
        {
            printf("Координаты вне поля!\n");
            continue;
        }

        if (grid[row][col] != 0)
        {
            printf("Это черная клетка с числом!\n");
            continue;
        }

        // Упрощённое добавление линии
        if (dir == 1 && row > 0)
            solution[row][col] = 2;
        else if (dir == 2 && row < SIZE - 1)
            solution[row][col] = 2;
        else if (dir == 3 && col > 0)
            solution[row][col] = 1;
        else if (dir == 4 && col < SIZE - 1)
            solution[row][col] = 1;
        else
            printf("Неверное направление или граница поля!\n");

        checkField();
    }

}

int main()
{
    system("chcp 1251");

    printf("Игра Роза Ветров (Four Winds)\n");

    initField();
    clearSolution();

    playGame();

    printf("Спасибо за игру!\n");
    return 0;

}