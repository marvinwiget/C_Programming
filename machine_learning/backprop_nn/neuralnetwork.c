#include "neuralnetwork.h"
#include "matrix.h"
#include "activation.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


NN* nn_alloc(
        int *arch, int n_arch, float training_rate, 
        activation_func act_hidden, activation_func act_output) 
{
    NN *nn = malloc(sizeof(NN));
    assert(nn != NULL); 

    nn->ws = malloc(sizeof(matrix*) * (n_arch - 1));
    nn->bs = malloc(sizeof(matrix*) * (n_arch - 1));
    nn->as = malloc(sizeof(matrix*) * n_arch);

    nn->g_ws = malloc(sizeof(matrix*) * (n_arch - 1));
    nn->g_bs = malloc(sizeof(matrix*) * (n_arch - 1));
    nn->g_as = malloc(sizeof(matrix*) * n_arch); 

    assert(nn->ws != NULL);
    assert(nn->bs != NULL);
    assert(nn->as != NULL);
    
    assert(nn->g_ws != NULL);
    assert(nn->g_bs != NULL);
    assert(nn->g_as != NULL);
 
    nn->as[0] = mat_alloc(1, arch[0]);
    nn->g_as[0] = mat_alloc(1, arch[0]);
    for (int i = 0; i < n_arch - 1; i++) {
       nn->ws[i] = mat_alloc(nn->as[i]->cols, arch[i+1]);
       nn->bs[i] = mat_alloc(1, arch[i+1]);
       nn->as[i+1] = mat_alloc(1, arch[i+1]);

       nn->g_ws[i] = mat_alloc(nn->as[i]->cols, arch[i+1]);
       nn->g_bs[i] = mat_alloc(1, arch[i+1]);
       nn->g_as[i+1] = mat_alloc(1, arch[i+1]);
    }

    for (int i = 0; i < n_arch - 1; i++) {
        mat_randomize(nn->ws[i]);
        mat_randomize(nn->bs[i]);
        mat_fill(nn->g_ws[i], 0);
        mat_fill(nn->g_bs[i], 0);
    }

    nn->act_hidden = act_hidden;
    nn->act_output = act_output;

    nn->arch = arch;
    nn->n_arch = n_arch;
    nn->training_rate = training_rate;

    return nn;
}

void nn_free(NN *nn) {
    if (nn == NULL) return;
    
    if (nn->input != NULL) mat_free(nn->input);
    if (nn->output != NULL) mat_free(nn->output);

    if (nn->as[0] != NULL) free(nn->as[0]);
    for (int i = 0; i < nn->n_arch - 1; i++) {
        if (nn->ws[i] != NULL) mat_free(nn->ws[i]);
        if (nn->bs[i] != NULL) mat_free(nn->bs[i]);
        if (nn->as[i+1] != NULL) mat_free(nn->as[i+1]);
    }
    free(nn->arch);
    free(nn);
}

void nn_setInput(NN *nn, matrix *input) {
    mat_copy(nn->input, input);
}

void nn_setOutput(NN *nn, matrix *output) {
    mat_copy(nn->output, output);
}

void nn_print(NN *nn) {
    printf("-> NEURAL NETWORK <-\n"
           "--------------------\n\n");
    printf("Input (as[0]):\n");
    mat_print(nn->input);
    for (int i = 0; i < nn->n_arch - 1; i++) {
        printf("ws[%d]:\n", i);
        mat_print(nn->ws[i]);
        printf("bs[%d]:\n", i);
        mat_print(nn->bs[i]);
        printf("as[%d]\n", i + 1);
        mat_print(nn->as[i+1]);
    }
    printf("\nOutput (as[%d]):\n", nn->n_arch - 1);
    mat_print(nn->output);
}

void nn_printResult(NN *nn) {
    printf("-> NEURAL NETWORK <-\n"
           "--------------------\n\n");

    printf("Input (as[0]):\n");
    mat_print(nn->input);
    
    printf("Output (as[%d]):\n", nn->n_arch - 1);
    mat_print(nn->output);
}

void nn_forward(NN *nn) {
    for (int i = 0; i < nn->n_arch; i++) {
        mat_mult(nn->as[i+1], nn->as[i], nn->ws[i]);
        mat_add(nn->as[i+1], nn->bs[i]);
        activation_func curr_act = (i < nn->n_arch - 1) ? 
                                    nn->act_hidden : nn->act_output;
        switch (curr_act) {
            case NONE: break;
            case RELU: mat_apply(nn->as[i+1], act_relu); break;
            case SIGMOID: mat_apply(nn->as[i+1], act_sigmoid); break;
            case SOFTMAX: break;
        } 
    }
    mat_copy(nn->output, nn->as[nn->n_arch - 1]);
}

void nn_train(NN *nn, matrix *td_in, matrix *td_out, int epochs) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        nn_backprop(NN *nn, matrix *td_in, matrix *td_out);
        nn_learn(NN *nn);
    }
}

void nn_backprop(NN *nn, matrix *td_in, matrix *td_out) {
   int n_samples = td_in->rows;
   
   for (int i = 0; i < nn->n_arch - 1; i++) {
       mat_fill(nn->g_ws[i], 0);
       mat_fill(nn->g_bs[i], 0);
   }

   for (int sample = 0; sample < n_samples; sample++) {
       nn_setInput(nn, mat_row(td_in, sample));
       nn_forward(nn);

       for (int i = 0; i < nn->n_arch; i++) {
           mat_fill(nn->g_as[i], 0);
       }

       for (int i = 0; i < td_out->cols; i++) {
           MAT_AT(nn->g_as[nn->n_arch - 1], 0, i) = MAT_AT(nn->output, 0, i) -
                                     MAT_AT(td_out, sample, i);
       }
       for (int layer = nn->n_arch - 1; layer > 0; layer--) {
           activation_func curr_act = (layer < nn->n_arch - 1) ?
                                      nn->act_hidden : nn->act_output;

           for (int curr = 0; curr < nn->as[layer]->cols; curr++) {
               float a = MAT_AT(nn->as[layer], 0, curr);
               float delta_a;
               switch (curr_act) {
                   case NONE: delta_a = 1.0f; break;
                   case RELU: delta_a = act_relu_d(a); break;
                   case SIGMOID: delta_a = act_sigmoid_d(a); break;
                   default: delta_a = 1.0f; break;
               }
               float error = MAT_AT(nn->g_as[nn->n_arch - 1], 0, curr) * delta_a;

               MAT_AT(nn->g_bs[layer - 1], 0, curr) += error;

               for (int prev = 0; prev < nn->as[layer - 1]->cols; prev++) {
                   float prev_a = MAT_AT(nn->as[layer - 1], 0, prev);
                   MAT_AT(nn->g_ws[layer - 1], prev, curr) += error * prev_a;
                   MAT_AT(nn->g_as[layer - 1], 0, prev) += error * MAT_AT(nn->ws[layer - 1], prev, curr);
               }
           }
       }
   }

   for (int i = 0; i < nn->n_arch - 1; i++) {
       int size = nn->g_ws[i]->rows * nn->g_ws[i]->cols;
       for (int j = 0; j < size; j++) {
           nn->g_ws[i]->data[j] /= n_samples;
       }

       size = nn->g_bs[i]->rows * nn->g_bs[i]->cols;
       for (int j = 0; j < size; j++) {
           nn->g_bs[i]->data[j] /= n_samples;
       }
   }
}

void nn_learn(NN *nn) {
    for (int i = 0; i < nn->n_arch - 1; i++) {
        int size = nn->ws[i]->rows * nn->ws[i]->cols;
        for (int j = 0; j < size; j++) {
            nn->ws[i]->data[j] -= nn->training_rate * nn->g_ws[i]->data[j];
        }
        size = nn->bs[i]->rows * nn->bs[i]->cols;
        for (int j = 0; j < size; j++) {
            nn->bs[i]->data[j] -= nn->training_rate * nn->g_bs[i]->data[j];
        }
    }
}


