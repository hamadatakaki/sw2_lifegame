#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
#include <time.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define GEN_RAND ((rand() / 10) % 10 == 3)

typedef enum
{
    Life106,
    RLE,
    RandomInitialize,
    None,
} FileType;

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp, FileType ftype);
void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width]);
int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width]);
void my_update_cells(const int height, const int width, int cell[height][width]);
void my_logging_cells(FILE *fp, const int height, const int width, int cell[height][width]);

FileType eval_file_type(char *filename);

int main(int argc, char **argv)
{
    FILE *fp = stderr;
    FileType ftype;
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
            ftype = eval_file_type(argv[1]);
            my_init_cells(height, width, cell, lgfile, ftype); // ファイルによる初期化
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
        my_init_cells(height, width, cell, NULL, RandomInitialize); // デフォルトの初期値を使う
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

        if (gen < 10000 && gen % 100 == 0)
        {
            char path[12];
            sprintf(path, "gen%04d.lif", gen);
            FILE *cell_log_fp = fopen(path, "w");
            fprintf(cell_log_fp, "#Life 1.06\n");
            my_logging_cells(cell_log_fp, height, width, cell);
            fclose(cell_log_fp);
        }
    }

    return EXIT_SUCCESS;
}

void _init_cells_as_lif(const int height, const int width, int cell[height][width], FILE *fp)
{
    int x, y;
    char line[MAX_LINE_LENGTH];

    fgets(line, MAX_LINE_LENGTH, fp);
    while (!feof(fp))
    {
        fgets(line, MAX_LINE_LENGTH, fp);
        sscanf(line, "%d %d\n", &x, &y);
        cell[y][x] = 1;
    }
}

void _parse_rle_header(FILE *fp, int *x, int *y)
{
    char line[MAX_LINE_LENGTH];
    while (1)
    {
        fgets(line, MAX_LINE_LENGTH, fp);
        int count = sscanf(line, "x = %d, y = %d\n", x, y);
        if (count == 2)
        {
            break;
        }

        if (feof(fp))
        {
            fprintf(stderr, "faild parsing header.\n");
            exit(1);
        }
    }
}

void _init_cells_as_rle(const int height, const int width, int cell[height][width], FILE *fp)
{
    int x, y;
    _parse_rle_header(fp, &x, &y);
    char rle[MAX_LINE_LENGTH];
    fgets(rle, MAX_LINE_LENGTH, fp);

    // rleに指定された文字が入っているのでいい感じにする
}

void my_init_cells(const int height, const int width, int cell[height][width], FILE *fp, FileType ftype)
{
    switch (ftype)
    {
    case RandomInitialize:
        srand((unsigned)time(NULL));
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                cell[i][j] = GEN_RAND;
            }
        }
        break;
    case Life106:
        _init_cells_as_lif(height, width, cell, fp);
        break;
    case RLE:
        _init_cells_as_rle(height, width, cell, fp);
        break;
    default:
        fprintf(stderr, "[ERROR] unvalid file type is specified.\n");
        exit(1);
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

int _count_up_cells(const int height, const int width, int cell[height][width])
{
    int total = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            total += cell[i][j];
        }
    }
    return total;
}

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width])
{
    int c = _count_up_cells(height, width, cell);
    printf("generateion = %d, life rate: %d / %d\n", gen, c, height * width);
    _print_cell_horizontal_edge(fp, width);
    for (int i = 0; i < height; i++)
    {
        _print_cell_line(fp, width, cell[i]);
    }
    _print_cell_horizontal_edge(fp, width);
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

void my_logging_cells(FILE *fp, const int height, const int width, int cell[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (cell[i][j])
            {
                fprintf(fp, "%d %d\n", j, i);
            }
        }
    }
}

FileType eval_file_type(char *filename)
{
    int size = strlen(filename);
    char c1 = filename[size - 3];
    char c2 = filename[size - 2];
    char c3 = filename[size - 1];

    if (c1 == 'l' && c2 == 'I' && c3 == 'f')
    {
        return Life106;
    }
    else if (c1 == 'r' && c2 == 'l' && c3 == 'e')
    {
        return RLE;
    }
    else
    {
        return None;
    }
}
