#include <stdio.h>
#include <stdlib.h>

char *VERSION = "Checkers v1.0 beta";
char *MESSAGE;

char BOARD[8][8] = {
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {2, 0, 2, 0, 2, 0, 2, 0},
    {0, 2, 0, 2, 0, 2, 0, 2},
};
const char *WHITE_COLOR = "\033[97m"; // Белый
const char *BLACK_COLOR = "\033[91m"; // В натуре красный

const char *WHITE_FIELD = "\033[100m";
const char *BLACK_FIELD = "\033[49m";
const char *BORDER = "\033[107m";

const char *EMPTY = "  ";
const char *CHECKER = "CH";
const char *QUEEN = "QU";

int turn = 0;

void set_color(const char *background, const char *checker)
{
    printf("%s%s", background, checker);
}

void reset_color()
{
    printf("\033[0m");
}

void print_border(int n)
{
    set_color(BORDER, "\033[30m");
    if (n > 0)
    {
        printf("%d ", n);
    }
    else
    {
        printf("  ");
    }
}

void draw_board()
{
    system("clear");
    printf("%s\n", VERSION);
    print_border(0);
    for (int x = 0; x < 8; ++x)
    {
        print_border(x + 1);
    }
    print_border(0);
    printf("\n");
    for (int y = 0; y < 8; ++y)
    {
        print_border(y + 1);
        for (int x = 0; x < 8; ++x)
        {
            char *back;
            if (y % 2 == 0)
            {
                if (x % 2 == 0)
                {
                    back = BLACK_FIELD;
                }
                else
                {
                    back = WHITE_FIELD;
                }
            }
            else
            {
                if (x % 2 == 0)
                {
                    back = WHITE_FIELD;
                }
                else
                {
                    back = BLACK_FIELD;
                }
            }

            char *field;
            if (BOARD[y][x] == 1)
            {
                set_color(back, BLACK_COLOR);
                field = CHECKER;
            }
            else if (BOARD[y][x] == 2)
            {
                set_color(back, WHITE_COLOR);
                field = CHECKER;
            }
            else if (BOARD[y][x] == 3)
            {
                set_color(back, BLACK_COLOR);
                field = QUEEN;
            }
            else if (BOARD[y][x] == 4)
            {
                set_color(back, WHITE_COLOR);
                field = QUEEN;
            }
            else
            {
                set_color(back, "");
                field = EMPTY;
            }
            printf("%s", field);
        }
        print_border(0);
        reset_color();
        printf("\n");
    }
    for (int x = 0; x < 10; ++x)
    {
        print_border(0);
    }
    reset_color();
    printf("\n");
    printf("%s", MESSAGE);
}

int modul(int n)
{
    if (n < 0)
        return -n;
    return n;
}

void check_queens()
{
    for (int x = 0; x < 8; ++x)
    {
        if (BOARD[0][x] == 2)
        {
            BOARD[0][x] = 4;
        }
        if (BOARD[7][x] == 1)
        {
            BOARD[7][x] = 3;
        }
    }
}

int can_move(int ox, int oy, int nx, int ny)
{
    // Проверка на выход за границы
    if (oy < 0 || oy > 7 ||
        ox < 0 || ox > 7 ||
        ny < 0 || ny > 7 ||
        nx < 0 || ny > 7)
    {
        MESSAGE = "Проверка на выход за границы\n";
        return 0;
    }

    // Проверка на наличие фигуры
    if (BOARD[oy][ox] == 0)
    {
        MESSAGE = "Проверка на наличие фигуры\n";
        return 0;
    }
    // Проверка на возможность хода в указанную точку
    if (BOARD[ny][nx] != 0)
    {
        MESSAGE = "Проверка на возможность хода в указанную точку\n";
        return 0;
    }
    // Проверка на диагональность хода
    if (modul(ox - nx) != modul(oy - ny))
    {
        MESSAGE = "Проверка на диагональность хода\n";
        return 0;
    }
    // Проверка на принадлежность фигуры
    if (turn % 2 == 0 && !(BOARD[oy][ox] == 2 || BOARD[oy][ox] == 4))
    {
        MESSAGE = "Проверка на принадлежность фигуры\n";
        return 0;
    }
    else if (turn % 2 == 1 && !(BOARD[oy][ox] == 1 || BOARD[oy][ox] == 3))
    {
        MESSAGE = "Проверка на принадлежность фигуры\n";
        return 0;
    }
    // Проверка на препядствия
    int yd = modul(oy - ny) / -(oy - ny);
    int xd = modul(ox - nx) / -(ox - nx);
    int someone_else = 0, se = 0;
    printf("yd: %d\nxd: %d\n", yd, xd);
    for (int i = 1; i < modul(ox - nx); ++i)
    {
        if (BOARD[oy + i * yd][ox + i * xd] != 0)
        {
            if (turn % 2 == 0 && (BOARD[oy + i * yd][ox + i * xd] == 2 || BOARD[oy + i * yd][ox + i * xd] == 4))
            {
                MESSAGE = "Собственная фигура на пути\n";
                return 0;
            }
            else if (turn % 2 == 1 && (BOARD[oy + i * yd][ox + i * xd] == 1 || BOARD[oy + i * yd][ox + i * xd] == 3))
            {
                MESSAGE = "Собственная фигура на пути\n";
                return 0;
            }
            else if (someone_else == 1)
            {
                MESSAGE = "Ряд из двух и более фигур на пути\n";
                return 0;
            }
            ++someone_else;
            se = 1;
        }
        else
        {
            someone_else = 0;
        }
    }
    // Проверка на доступность хода для фигуры
    if (BOARD[oy][ox] == 1 || BOARD[oy][ox] == 2)
    {
        printf("modul: %d\nse: %d\n", modul(ox - nx), se);
        if (modul(ox - nx) == 2)
            if (se != 1)
            {
                MESSAGE =  "Проверка на доступность хода для фигуры\n";
                return 0;
            }
        else if(modul(ox - nx) > 2);
    }
    return 1;
}

int eat_everyone(int oy, int ox, int ny, int nx)
{
    int yd = modul(oy - ny) / -(oy - ny);
    int xd = modul(ox - nx) / -(ox - nx);
    int eaten = 0;
    for (int i = 1; i < modul(ox - nx); ++i)
    {
        if (BOARD[oy + i * yd][ox + i * xd] != 0)
        {
            BOARD[oy + i * yd][ox + i * xd] = 0;
            ++eaten;
        }
    }
    return eaten;
}

int move(int oy, int ox, int ny, int nx)
{
    if (oy < 0 || oy > 7 ||
        ox < 0 || ox > 7 ||
        ny < 0 || ny > 7 ||
        nx < 0 || ny > 7)
        return 0;
    int checker_code = BOARD[oy][ox];
    BOARD[oy][ox] = 0;
    BOARD[ny][nx] = checker_code;
    return 1;
}

int check_borders(int x, int y)
{
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return 0;
    return 1;
}

int check_kills(int ox, int oy, int xd, int yd)
{
    int max = 1;
    if (BOARD[oy][ox] == 1 || BOARD[oy][ox] == 2)
        max = 1;
    else
        max = 7;

    int someone_else = 0;
    for (int i = 0; check_borders(ox + i * xd, oy + i * yd) && i < max;)
    {
check_next:
        ++i;
        if (BOARD[oy + i * yd][ox + i * xd] != 0)
        {
            if (turn % 2 == 0 && (BOARD[oy + i * yd][ox + i * xd] == 2 || BOARD[oy + i * yd][ox + i * xd] == 4))
            {
                /*Собственная фигура на пути*/
                return 0;
            }
            if (turn % 2 == 1 && (BOARD[oy + i * yd][ox + i * xd] == 1 || BOARD[oy + i * yd][ox + i * xd] == 3))
            {
                /*Собственная фигура на пути*/
                return 0;
            }
            if (check_borders(ox + (i + 1) * xd, oy + (i + 1) * yd) && BOARD[oy + (i + 1)* yd][ox + (i + 1) * xd] != 0)
                return 0;
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    MESSAGE = malloc(100);
    int oy, ox, ny, nx, combo = 0, can_eat = 0;
    char y1, x1, y2, x2;
    while (1)
    {
        check_queens();
        draw_board();
        if (turn % 2 == 0)
        {
            set_color(BORDER, BLACK_COLOR);
            printf("WHITE:\033[0m ");
        }
        else
        {
            set_color(BLACK_FIELD, WHITE_COLOR);
            printf("BLACK:\033[0m ");
        }
        scanf("%d %d - %d %d", &oy, &ox, &ny, &nx);
        can_eat = 0;
        for (int y = 0; y < 8; ++y)
        {
            for (int x = 0; x < 8; ++x)
            {
                if (check_kills(x, y, 1, 1) || check_kills(x, y, -1, 1) || check_kills(x, y, 1, -1) || check_kills(x, y, -1, -1))
                    ++can_eat;
            }
        }
        if (can_move(ox - 1, oy - 1, nx - 1, ny - 1) == 1)
        {
            if (can_eat)
            {
                if (check_kills(ox - 1, oy - 1, (nx - ox) / modul(nx - ox), (ny - oy) / modul(ny - oy)) && move(oy - 1, ox - 1, ny - 1, nx - 1))
                {
                    combo = eat_everyone(oy - 1, ox - 1, ny - 1, nx - 1);
                    if ((check_kills(nx - 1, ny - 1, 1, 1) || check_kills(nx - 1, ny - 1, -1, 1) || check_kills(nx - 1, ny - 1, 1, -1) || check_kills(nx - 1, ny - 1, -1, -1)))
                    {
			            if (!combo)
                            ++turn;
                    }
                    else
                    {
                        ++turn;
                    }
                    MESSAGE = "Ход совершён\n";
                }
                else
                {
                    MESSAGE = "Не удалось совершить ход! !move(...)\n";
                }
            }
            else
            {
                if (move(oy - 1, ox - 1, ny - 1, nx - 1))
                {
                    ++turn;
                    combo = 0;
                    MESSAGE = "Ход совершён\n";
                }
                else
                {
                    MESSAGE = "Не удалось совершить ход! !move(...)\n";
                }
            }
        }
    }
    free(MESSAGE);
    return 0;
}
