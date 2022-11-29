#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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
    FILE* input;
    FILE* output;
    
    // checking the file
    if ((input = fopen(filename, "r")) == NULL) {
        printf("Wrong filename");
        exit(1);
    }
    
    char* info = (char*)malloc(54);
    
    // reading .bmp info
    for (int i = 0; i < 54; ++i) {
        info[i] = fgetc(input);
    }
    
    int width = *(int*)&info[18];
    int height = *(int*)&info[22];
    char r, g, b;
    int x;
    
    // allocating memory
    char** base = (char**)malloc(height * sizeof(char*));
    char** res = (char**)malloc(height * sizeof(char*));
    for (int i = 0; i < height; ++i) {
        base[i] = (char*)malloc(width);
        res[i] = (char*)malloc(width);
    }
    
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // reading red, green and blue bytes and getting one grayscale byte
            r = fgetc(input);
            g = fgetc(input);
            b = fgetc(input);
            x = (int)(0.299 * r + 0.587 * g + 0.114 * b) / 3;
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
    
    // first thread
    struct data* args1 = (struct data*)malloc(sizeof(struct data));
    args1->width = width;
    args1->start = 1;
    args1->end = height / 4;
    args1->base = base;
    args1->res = res;
    pthread_t a_thread;
    pthread_create(&a_thread, NULL, sobel, (void*)args1);
    
    
    // second thread
    struct data* args2 = (struct data*)malloc(sizeof(struct data));
    args2->width = width;
    args2->start = height / 4 + 1;
    args2->end = 2 * (height / 4);
    args2->base = base;
    args2->res = res;
    pthread_t b_thread;
    pthread_create(&b_thread, NULL, sobel, (void*)args2);
    
    // third thread
    struct data* args3 = (struct data*)malloc(sizeof(struct data));
    args3->width = width;
    args3->start = 2 * (height / 4) + 1;
    args3->end = 3 * (height / 4);
    args3->base = base;
    args3->res = res;
    pthread_t c_thread;
    pthread_create(&c_thread, NULL, sobel, (void*)args3);
    
    // fourth thread
    struct data* args4 = (struct data*)malloc(sizeof(struct data));
    args4->width = width;
    args4->start = 3 * (height / 4) + 1;
    args4->end = height - 2;
    args4->base = base;
    args4->res = res;
    pthread_t d_thread;
    pthread_create(&d_thread, NULL, sobel, (void*)args4);
    
    pthread_join(a_thread, NULL);
    pthread_join(b_thread, NULL);
    pthread_join(c_thread, NULL);
    pthread_join(d_thread, NULL);
    
    free(args1);
    free(args2);
    free(args3);
    free(args4);
    
    output = fopen("output.bmp", "w+");
    for (int i = 0; i < 54; ++i) {
        fputc(info[i], output);
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            for (int k = 0; k < 3; ++k) {
                fputc(res[i][j], output);
            }
        }
    }
    
    free(info);
    for (int i = 0; i < height; ++i) {
        free(base[i]);
        free(res[i]);
    }
    free(base);
    free(res);
}
