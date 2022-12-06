#include <math.h>
#include <stdio.h>

struct data {
    int width;
    int start;
    int end;
    char** base;
    char** res;
};

void* sobel(void* args) {
    struct data* d = (struct data*)args;
    int width = d->width;
    int start = d->start;
    int end = d->end;
    char** base = d->base;
    char** res = d->res;
    
    int kx[3][3];
    int ky[3][3];
    
    kx[0][0] = -1; kx[0][1] = 0; kx[0][2] = 1;
    kx[1][0] = -2; kx[1][1] = 0; kx[1][2] = 2;
    kx[2][0] = -1; kx[2][1] = 0; kx[2][2] = 1;
    
    ky[0][0] = 1; ky[0][1] = 2; ky[0][2] = 1;
    ky[1][0] = 0; ky[1][1] = 0; ky[1][2] = 0;
    ky[2][0] = -1; ky[2][1] = -2; ky[2][2] = -1;
    
    for (int i = start; i <= end; ++i) {
        for (int j = 1; j < width - 1; ++j) {
            double gx, gy, g;
            gx = kx[0][0] * base[i - 1][j - 1]
                 + kx[0][1] * base[i - 1][j]
                 + kx[0][2] * base[i - 1][j + 1]
                 + kx[1][0] * base[i][j - 1]
                 + kx[1][1] * base[i][j]
                 + kx[1][2] * base[i][j + 1]
                 + kx[2][0] * base[i + 1][j - 1]
                 + kx[2][1] * base[i + 1][j]
                 + kx[2][2] * base[i + 1][j + 1];
            gy = ky[0][0] * base[i - 1][j - 1]
                 + ky[0][1] * base[i - 1][j]
                 + ky[0][2] * base[i - 1][j + 1]
                 + ky[1][0] * base[i][j - 1]
                 + ky[1][1] * base[i][j]
                 + ky[1][2] * base[i][j + 1]
                 + ky[2][0] * base[i + 1][j - 1]
                 + ky[2][1] * base[i + 1][j]
                 + ky[2][2] * base[i + 1][j + 1];
            g = sqrt(gx * gx + gy * gy);
            res[i][j] = (int)g > 255 ? (char)255 : (char)(int)g;
        }
    }
}
