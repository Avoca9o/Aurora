#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const int INFO_BYTES = 54;
const int HEIGHT_BYTE = 22;
const int WIDTH_BYTE = 18;
const int NUM_THREADS = 4;
const double RED = 0.299;
const double GREEN = 0.587;
const double BLUE = 0.114;

extern void* sobel(void* args);

// struct for transporting data between threads
struct data {
    int width;
    int start;
    int end;
    char** base;
    char** res;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("No input file or too much arguments\n");
        exit(1); 
    }
    
    char* filename = argv[1];
    FILE* input = NULL;
    FILE* output = NULL;
    
    // checking the file
    if ((input = fopen(filename, "r")) == NULL) {
        printf("Wrong filename");
        exit(1);
    }
    
    char* info = malloc(INFO_BYTES);
    
    // reading .bmp info
    for (int i = 0; i < INFO_BYTES; ++i) {
        info[i] = fgetc(input);
    }
    
    int width = *(int*)&info[WIDTH_BYTE];
    int height = *(int*)&info[HEIGHT_BYTE];
    char r, g, b;
    int x;
    
    // allocating memory
    char** base = malloc(height * sizeof(char*));
    char** res = malloc(height * sizeof(char*));
    for (int i = 0; i < height; ++i) {
        base[i] = malloc(width);
        res[i] = malloc(width);
    }
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // reading red, green and blue bytes and getting one grayscale byte
            r = fgetc(input);
            g = fgetc(input);
            b = fgetc(input);
            x = (int)(RED * r + GREEN * g + BLUE * b) / 3;
            base[i][j] = (char)x;
        }
    }
    
    for (int i = 0; i < height; ++i) {
        res[i][0] = (char)0;
        res[i][width - 1] = (char)0;
    }
    for (int i = 0; i < width; ++i) {
        res[0][i] = (char)0;
        res[height - 1][i] = (char)0;
    }
    
    pthread_t threads[NUM_THREADS];
    struct data** args = malloc(NUM_THREADS * sizeof(struct data *));
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        args[i] = malloc(sizeof(struct data));
        args[i]->width = width;
        args[i]->start = i * (height / NUM_THREADS) + 1;
        if (i == 3) {
            args[i]->end = height - 2;
        } else {
            args[i]->end = (i + 1) * (height / NUM_THREADS);
        }
        args[i]->base = base;
        args[i]->res = res;
    }
    
    for (int i = 0; i < 4; ++i) {
        pthread_create(&threads[i], NULL, sobel, (void *)args[i]);
    }

    
    for (int i = 0; i < 4; ++i) {
       pthread_join(threads[i], NULL);
    }
    
    output = fopen("output.bmp", "w+");
    for (int i = 0; i < INFO_BYTES; ++i) {
        fputc(info[i], output);
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < 3; ++k) {
                fputc(res[i][j], output);
            }
        }
    }
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        free(args[i]);
    }
    free(args);
    free(info);
    for (int i = 0; i < height; ++i) {
        free(base[i]);
        free(res[i]);
    }
    free(base);
    free(res);
}
