#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

#define MIN(x, y) (x < y) ? x : y
#define MAX(x, y) (x > y) ? x : y
#define MAT_AT(m, i, j) (m)->data[i * (m)->cols + j]

typedef struct {
    float *data;
    int rows, cols;
} matrix;

// operations 
matrix* mat_alloc(int rows, int cols);
matrix* mat_load(FILE *file, int rows, int cols);
void mat_free(matrix *mat);
void mat_print(const matrix *mat);

void mat_randomize(matrix *mat); // between -1 and 1
void mat_fill(matrix *mat, float num);
void mat_copy(matrix *dst, const matrix *src);
void mat_transpose(matrix *mat);

float mat_max(const matrix *mat);
float mat_min(const matrix *mat);

void mat_scale(matrix *mat, float scalar);
void mat_add(matrix *dst, const matrix *src);
void mat_sub(matrix *dst, const matrix *src);
void mat_mult(matrix *out, const matrix *x1, const matrix *x2);

void mat_apply(matrix *mat, float (*operation)(float));

#endif
