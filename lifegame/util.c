#include "util.h"

int conv_add(const int height, const int width, int arr[height][width], int x, int y, int w, int h)
{
    assert((0 <= x) && (x + w - 1 < width) && (0 <= y) && (y + h - 1 < height));
    int sum = 0;
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            sum += arr[j + x][i + y];
        }
    }
    return sum;
}