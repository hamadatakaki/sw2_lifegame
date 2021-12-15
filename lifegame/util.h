#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#define INITIALIZE(class) ((class *)malloc(sizeof(class)))
#define GEN_RAND ((rand() / 10) % 10 < 2)
#define MAX(u, v) (u > v ? u : v)
#define MIN(u, v) (u < v ? u : v)

int conv_add(const int height, const int width, int arr[height][width], int x, int y, int w, int h);
