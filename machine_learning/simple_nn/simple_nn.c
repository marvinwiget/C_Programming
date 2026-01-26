#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define MAX(x, y) ((x > y) ? x : y)
#define MIN(x, y) ((x < y) ? x : y)
#define MAT_AT(m, i, j) m->data[i * m->cols + j]

#define INIT_BIAS 0
#define TRAINING_COUNT 10000
#define EPS 0.0001
#define RATE 0.0001
#define GLOBAL_ACTIVATION relu

typedef struct {
    float *data;
    int rows, cols;
} matrix;

typedef enum {
    relu,
    sigmoid
} ACTIVATION;

typedef struct {
    matrix **w; // n = n_layers
    matrix **b; // n = n_layers
    matrix **a; // n = n_arch
    int n_layers;
    ACTIVATION activation;
} NN;


matrix* mat_alloc(int rows, int cols);
void mat_copy(matrix *dst, matrix *src);
void mat_randomise(matrix *mat);
void mat_set(matrix *mat, float num);
void mat_mult(matrix *out, matrix *m1, matrix *m2);
void mat_add(matrix *dst, matrix *src);
void mat_activation(matrix *mat, ACTIVATION activation);

NN* nn_create(int *arch, int n_arch);
void nn_forward(NN *nn);
float nn_cost(NN *nn);
void nn_diff(NN *nn, NN *gradient);
void nn_learn(NN *nn, NN *gradient);

int td_in[] = {-3,-2,-1,0,1,2,3};
int td_out[] = {9,4,1,0,1,4,9};
int n_td = sizeof(td_in)/sizeof(td_in[0]);

int main() {
    srand(time(NULL));
    
    int arch[] = {1, 8, 8,1};
    int n_arch = sizeof(arch) / sizeof(arch[0]);

    NN *nn = nn_create(arch, n_arch);
    NN *gradient = nn_create(arch, n_arch);

    for (int i=1; i<=TRAINING_COUNT; i++) {
        nn_diff(nn, gradient);
        nn_learn(nn, gradient);
        if (i % 100 == 0) {
            printf("iteration: %d, cost: %f\n", i, nn_cost(nn));
        }
    }

    for (int i=0; i<n_td; i++) {
        nn->a[0]->data[0] = td_in[i];
        nn_forward(nn);
        printf("Input: %d, Expected Output: %d, Actual Output: %f\n",
            td_in[i], td_out[i], nn->a[nn->n_layers]->data[0]);
    }

    return 0;
}


matrix* mat_alloc(int rows, int cols) {
    assert(rows > 0);
    assert(cols > 0);
    matrix *mat = malloc(sizeof(matrix));
    if (mat == NULL) {
        printf("mat alloc failed\n");
        exit(1);
    }
    mat->data = malloc(sizeof(float) * rows * cols);
    if (mat->data == NULL) {
        printf("mata data alloc failed\n");
        exit(1);
    }
    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

void mat_copy(matrix *dst, matrix *src) {
    assert(src->rows == dst->rows);
    assert(src->cols == dst->cols);

    for (int i=0; i<src->rows * src->cols; i++) {
        dst->data[i] = src->data[i];
    }
}

void mat_randomise(matrix *mat) {
    for (int i=0; i<mat->rows * mat->cols; i++) {
        mat->data[i] =  (float)rand() / (float)RAND_MAX * 2.0f - 1.0f;
    }
}

void mat_set(matrix *mat, float num) {
    for (int i=0; i<mat->rows * mat->cols; i++) {
        mat->data[i] = num;
    }
}

void mat_mult(matrix *out, matrix *m1, matrix *m2) {
    assert(m1->cols == m2->rows);
    assert(out->rows == m1->rows);
    assert(out->cols == m2->cols);
    mat_set(out, 0);
    for (int i=0; i<out->rows; i++) {
        for (int k=0; k<m1->cols; k++) {
            for (int j=0; j<out->cols; j++) {
                MAT_AT(out, i, j) += 
                    MAT_AT(m1, i, k) *
                    MAT_AT(m2, k, j);
            }
        }
    }
}

void mat_add(matrix *dst, matrix *src) {
    assert(dst->rows == src->rows);
    assert(dst->cols == src->cols);

    for (int i=0; i<src->rows * src->cols; i++) {
        dst->data[i] += src->data[i];
    }
}

void mat_activation(matrix *mat, ACTIVATION activation) {
    for (int i=0; i<mat->rows*mat->cols; i++) {
        switch (activation) {
            case relu: 
                mat->data[i] = MAX(mat->data[i], mat->data[i]*0.001f);
                break;
            case sigmoid:
                mat->data[i] = 1/(1 + expf(-mat->data[i]));
                break;
        } 
    }
}

NN* nn_create(int *arch, int n_arch) {
    NN *nn = malloc(sizeof(NN));
    if (nn == NULL) {
        printf("nn alloc failed\n");
        exit(1);
    }
    
    nn->n_layers = n_arch - 1;
    nn->activation = GLOBAL_ACTIVATION;
    
    // weights and biase init
    nn->w = malloc(sizeof(matrix*)*nn->n_layers);
    nn->b = malloc(sizeof(matrix*)*nn->n_layers);
    nn->a = malloc(sizeof(matrix*)*n_arch);

    nn->a[0] = mat_alloc(1, arch[0]);
    for (int i=1; i<n_arch; i++) {
        nn->w[i-1] = mat_alloc(nn->a[i-1]->cols, arch[i]);
        nn->b[i-1] = mat_alloc(1, arch[i]);
        nn->a[i] = mat_alloc(1, arch[i]);
    }

    for (int i=0; i<nn->n_layers; i++) {
        mat_randomise(nn->w[i]);
        mat_set(nn->b[i], INIT_BIAS);
    }
    
    return nn;
}

void nn_forward(NN *nn) {
    for (int i=0; i<nn->n_layers; i++) {
        mat_mult(nn->a[i+1], nn->a[i], nn->w[i]);
        mat_add(nn->a[i+1], nn->b[i]);
        if (i != nn->n_layers - 1) mat_activation(nn->a[i+1], nn->activation);
    }
}

float nn_cost(NN *nn) {
    float c = 0.0f;
    for (int j=0; j<n_td; j++) {
        nn->a[0]->data[0] = (float)td_in[j];
        nn_forward(nn);
        float d = td_out[j] - nn->a[nn->n_layers]->data[0];
        c += d * d;       
    }
    return c / n_td;
}
// FINITE DIFF
void nn_diff(NN *nn, NN *gradient) {
    float c = nn_cost(nn);
    for (int i=0; i<nn->n_layers; i++) {
        for (int j=0;j<nn->w[i]->cols * nn->w[i]->rows; j++) {
            float saved = nn->w[i]->data[j];
            nn->w[i]->data[j] += EPS;
            gradient->w[i]->data[j] = (nn_cost(nn) - c) / EPS;
            nn->w[i]->data[j] = saved;
        }
    }

    for (int i=0; i<nn->n_layers; i++) {
        for (int j=0;j<nn->b[i]->cols * nn->b[i]->rows; j++) {
            float saved = nn->b[i]->data[j];
            nn->b[i]->data[j] += EPS;
            gradient->b[i]->data[j] = (nn_cost(nn) - c) / EPS;
            nn->b[i]->data[j] = saved;
        }
    }
}

void nn_learn(NN *nn, NN *gradient) {
    for (int i=0; i<nn->n_layers; i++) {
        for (int j=0; j<nn->w[i]->cols * nn->w[i]->rows; j++) {
            nn->w[i]->data[j] -= RATE * gradient->w[i]->data[j];
        }
    }

    for (int i=0; i<nn->n_layers; i++) {
        for (int j=0; j<nn->b[i]->cols * nn->b[i]->rows; j++) {
            nn->b[i]->data[j] -= RATE * gradient->b[i]->data[j];
        }
    }

}


