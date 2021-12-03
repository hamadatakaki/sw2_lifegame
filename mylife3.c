#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う

#define MAX_LINE_LENGTH 256

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp);

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]);

int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width]);

void my_update_cells(const int height, const int width, int cell[height][width]);

int main(int argc, char **argv)
{
    FILE *fp = stderr;
    const int height = 40;
    const int width = 70;

    int cell[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cell[y][x] = 0;
        }
    }

    /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
    if (argc > 2)
    {
        fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        FILE *lgfile;
        if ((lgfile = fopen(argv[1], "r")) != NULL)
        {
            my_init_cells(height, width, cell, lgfile); // ファイルによる初期化
        }
        else
        {
            fprintf(stderr, "cannot open file %s\n", argv[1]);
            return EXIT_FAILURE;
        }
        fclose(lgfile);
    }
    else
    {
        my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
    }

    my_print_cells(fp, 0, height, width, cell); // 表示する
    sleep(1);                                   // 1秒休止

    /* 世代を進める*/
    for (int gen = 1;; gen++)
    {
        my_update_cells(height, width, cell);         // セルを更新
        my_print_cells(fp, gen, height, width, cell); // 表示する
        sleep(1);                                     //1秒休止する
        fprintf(fp, "\e[%dA", height + 3);            //height+3 の分、カーソルを上に戻す(壁2、表示部1)
    }

    return EXIT_SUCCESS;
}

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp)
{
    int use_default = 0;
    if (fp == NULL)
    {
        use_default = 1;
        fp = fopen("default.lif", "r");
    }

    int *x, *y;
    char line[MAX_LINE_LENGTH];

    fgets(line, MAX_LINE_LENGTH, fp);
    while (!feof(fp))
    {

        fgets(line, MAX_LINE_LENGTH, fp);
        sscanf(line, "%d %d\n", x, y);
        cell[*y][*x] = 1;
    }

    if (use_default)
    {
        fclose(fp);
    }
}

void _print_cell_horizontal_edge(FILE *fp, const int width)
{
    fprintf(fp, "+");
    for (int i = 0; i < width; i++)
    {
        fprintf(fp, "-");
    }
    fprintf(fp, "+\n");
}

void _print_cell_line(FILE *fp, const int width, int cell[width])
{
    fprintf(fp, "|");
    for (int i = 0; i < width; i++)
    {
        fprintf(fp, cell[i] ? "\e[31m#\e[0m" : " ");
    }
    fprintf(fp, "|\n");
}

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width])
{
    printf("generateion = %d\n", gen);
    _print_cell_horizontal_edge(fp, width);
    for (int i = 0; i < height; i++)
    {
        _print_cell_line(fp, width, cell[i]);
    }
    _print_cell_horizontal_edge(fp, width);
}

void debug_cells(FILE *fp, const int height, const int width, int cell[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(fp, "%d", cell[i][j]);
        }
        fprintf(fp, "\n");
    }
}

int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width])
{
    int total = 0;

    if (h > 0)
    {
        if (w > 0)
        {
            total += cell[h - 1][w - 1];
        }
        total += cell[h - 1][w];
        if (w < width - 1)
        {
            total += cell[h - 1][w + 1];
        }
    }

    if (w > 0)
    {
        total += cell[h][w - 1];
    }
    if (w < width - 1)
    {
        total += cell[h][w + 1];
    }

    if (h < height - 1)
    {
        if (w > 0)
        {
            total += cell[h + 1][w - 1];
        }
        total += cell[h + 1][w];
        if (w < width - 1)
        {
            total += cell[h + 1][w + 1];
        }
    }

    return total;
}

void my_update_cells(const int height, const int width, int cell[height][width])
{
    int update_cell[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int current = cell[i][j];
            int total = my_count_adjacent_cells(i, j, height, width, cell);
            int cond = (total == 3) || (current && (total == 2));
            update_cell[i][j] = cond;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cell[i][j] = update_cell[i][j];
        }
    }
}
