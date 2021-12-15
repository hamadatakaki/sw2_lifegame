#include "stage.h"

Stage *new_stage(const int height, const int width)
{
    Stage *stage = INITIALIZE(Stage);
    stage->height = height;
    stage->width = width;

    srand((unsigned)time(NULL));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < height; x++)
        {
            stage->cell[y][x] = GEN_RAND;
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < height; x++)
        {
            stage->nutrition[y][x] = INITIAL_NUTRITION;
        }
    }

    return stage;
}

void _echo_stage_hline(const int width, int line[width], int nutrs[width])
{
    printf("|");
    for (int i = 0; i < width; i++)
    {
        int g = MIN(nutrs[i] * 2, 192) + 15;
        printf("\e[48;2;0;%d;0m", g);
        printf("%c", line[i] ? 'o' : ' ');
    }
    printf("\x1b[49m|\n");
}

void _echo_stage_hedge(const int width)
{
    printf("x");
    for (int i = 0; i < width; i++)
    {
        printf("-");
    }
    printf("x\n");
}

void echo_stage(Stage *stage)
{
    _echo_stage_hedge(stage->width);
    for (int j = 0; j < stage->height; j++)
    {
        _echo_stage_hline(stage->width, stage->cell[j], stage->nutrition[j]);
    }
    _echo_stage_hedge(stage->width);
}

void _debug_stage_hline(const int width, int nutrs[width])
{
    printf("|");
    for (int i = 0; i < width; i++)
    {
        int g = MIN(nutrs[i], 9);
        printf("%d", g);
    }
    printf("|\n");
}

void debug_stage(Stage *stage)
{
    echo_stage(stage);

    _echo_stage_hedge(stage->width);
    for (int j = 0; j < stage->height; j++)
    {
        _debug_stage_hline(stage->width, stage->nutrition[j]);
    }
    _echo_stage_hedge(stage->width);
}

void step_stage(Stage *stage)
{
    int H = stage->height;
    int W = stage->width;
    int alive_cell_count[H][W];
    int consume_nurition_count[H][W];
    int dying_cell_count[H][W];

    // cellとnutritionの計算用の配列を定義・初期化
    int _cells[H + 2][W + 2];
    int _nutrs[H + 2][W + 2];

    for (int i = 0; i < H + 2; i++)
    {
        _cells[0][i] = 0;
        _cells[H + 1][i] = 0;
    }
    for (int j = 0; j < W + 2; j++)
    {
        _cells[j][0] = 0;
        _cells[j][W + 1] = 0;
    }
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            _cells[j + 1][i + 1] = stage->cell[j][i];
        }
    }

    for (int i = 0; i < H + 2; i++)
    {
        _nutrs[0][i] = 0;
        _nutrs[H + 1][i] = 0;
    }
    for (int j = 0; j < W + 2; j++)
    {
        _nutrs[j][0] = 0;
        _nutrs[j][W + 1] = 0;
    }
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            _nutrs[j + 1][i + 1] = stage->nutrition[j][i];
        }
    }

    // 栄養の消費を計算
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int alives = conv_add(H + 2, W + 2, _cells, j, i, 3, 3);
            int nurt = stage->nutrition[j][i];
            consume_nurition_count[j][i] = nurt - alives;
        }
    }

    // 消費の結果でcellの生死を決定
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int consume = consume_nurition_count[j][i];
            int is_alive = stage->cell[j][i];

            dying_cell_count[j][i] = 0;

            if (is_alive)
            {
                _nutrs[j + 1][i + 1] = MAX(0, consume);
                if (consume < 0)
                {
                    stage->cell[j][i] = 0;
                    dying_cell_count[j][i] = 1;
                }
            }
            else
            {
                if (consume >= CELL_BORN_CAPACITY)
                {
                    stage->cell[j][i] = 1;
                }
            }
        }
    }

    // cellの死による栄養の発生
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int delta_nutrs = NUTRITION_BY_DEATH * dying_cell_count[j][i];
            for (int ii = 0; ii < 3; ii++)
            {
                for (int jj = 0; jj < 3; jj++)
                {
                    _nutrs[j + jj][i + ii] += delta_nutrs;
                }
            }
        }
    }

    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            stage->nutrition[j][i] = _nutrs[j + 1][i + 1];
        }
    }
}