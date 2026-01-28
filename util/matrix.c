#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "matrix.h"

matrix* mat_alloc(int rows, int cols) {
    assert(rows > 0);
    assert(cols > 0);

    matrix *mat = malloc(sizeof(matrix));
    assert(mat != NULL);

    mat->data = malloc(sizeof(float) * rows * cols);
    assert(mat->data != NULL);

    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

/* TO DO */
matrix* mat_load(FILE *file, int rows, int cols) {
    (void) file;
    (void) rows;
    (void) cols;
    return (matrix*) malloc(sizeof(matrix));
}

void mat_free(matrix *mat) {
    if (mat == NULL) return;
    
    if (mat->data != NULL) {
        free(mat->data);
    }
    free(mat);
    return;
}

void mat_print(const matrix *mat) {
    assert(mat != NULL);
    assert(mat->data != NULL);
    
    printf("[\n");
    for (int i = 0; i < mat->rows; i++) {
        printf("\t[");
        for (int j = 0; j < mat->cols; j++) {
            printf("%f", MAT_AT(mat, i, j));
            if (j != mat->cols - 1) {
                printf(", ");
            }
        }
        printf("]");
        if (i != mat->rows - 1) {
            printf("\n");
        }
    }
    printf("\n]\n");
    return;
}


void mat_randomize(matrix *mat) {
    assert(mat != NULL);
    assert(mat->data != NULL);

    int size = mat->rows * mat->cols;
    for (int i = 0; i < size; i++) {
        mat->data[i] = ((float) rand() / (float) RAND_MAX) * 2.0f + 1.0f;
    }
}

void mat_fill(matrix *mat, float num) {
    assert(mat != NULL);
    assert(mat->data != NULL);

    int size = mat->rows * mat->cols;
    for (int i = 0; i < size; i++) {
        mat->data[i] = num;
    }
}

void mat_copy(matrix *dst, const matrix *src) {
    assert(src != NULL);
    assert(src->data != NULL);

    assert(dst != NULL);
    assert(dst->data != NULL);

    assert(src->rows == dst->rows);
    assert(src->cols == src->cols);

    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++) {
        dst->data[i] = src->data[i];
    }
}

/* TO DO */
void mat_transpose(matrix *mat) {
    assert(mat != NULL);

    int temp = mat->rows;
    mat->rows = mat->cols;
    mat->cols = temp;
}

float mat_max(const matrix *mat) {
    assert(mat != NULL);
    assert(mat->data != NULL);

    float max = MAT_AT(mat, 0, 0);
    int size = mat->rows * mat->cols;
    for (int i = 0; i < size; i++) {
        if (mat->data[i] > max) {
            max = mat->data[i];
        }
    }
    return max;
}

float mat_min(const matrix *mat) {
    assert(mat != NULL);
    assert(mat->data != NULL);

    float min = MAT_AT(mat, 0, 0);
    int size = mat->rows * mat->cols;
    for (int i = 0;i < size; i++) {
        if (mat->data[i] < min) {
            min = mat->data[i];
        }
    }
    return min;
}

void mat_scale(matrix *mat, float scalar) {
    assert(mat != NULL);
    assert(mat->data != NULL);

    int size = mat->rows * mat->cols;
    for (int i = 0; i < size; i++) {
        mat->data[i] *= scalar;
    }
}
void mat_add(matrix *dst, const matrix *src) {
    assert(src != NULL);
    assert(src->data != NULL);

    assert(dst != NULL);
    assert(dst->data != NULL);

    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++) {
        dst->data[i] += src->data[i];
    }
}

void mat_sub(matrix *dst, const matrix *src) {
    assert(src != NULL);
    assert(src->data != NULL);

    assert(dst != NULL);
    assert(dst->data != NULL);

    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++) {
        dst->data[i] -= src->data[i];
    }
}

void mat_mult(matrix *out, const matrix *x1, const matrix *x2) {
    assert(x1 != NULL);
    assert(x1->data != NULL);

    assert(x2 != NULL);
    assert(x2->data != NULL);

    assert(x1->cols == x2->rows);
    assert(out->rows == x1->rows);
    assert(out->cols == x2->cols);

    mat_fill(out, 0);
    for (int i = 0; i < out->rows; i++) {
        for (int k = 0; k < x1->cols; k++) {
            for (int j = 0; j < out->cols; j++) {
                MAT_AT(out, i, j) += MAT_AT(x1, i, k) * MAT_AT(x2, k, j); 
            }
        }
    }
}

void mat_relu(matrix *dst, const matrix *src) {
    assert(src != NULL);
    assert(src->data != NULL);

    assert(dst != NULL);
    assert(dst->data != NULL);

    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++) {
        dst->data[i] = (src->data[i] > 0) ? src->data[i] : 0;
    }
}

void mat_sigmoid(matrix *dst, const matrix *src) {
    assert(src != NULL);
    assert(src->data != NULL);

    assert(dst != NULL);
    assert(dst->data != NULL);

    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    int size = src->rows * src->cols;
    for (int i = 0; i < size; i++) {
        dst->data[i] = 1 / (1 + expf(-src->data[i]));
    }
}

