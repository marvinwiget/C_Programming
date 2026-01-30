#include "neuralnetwork.h"
#include "matrix.h"
#include "activation.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


NN* nn_alloc(
        int *arch, int n_arch, float training_rate, 
        activation_func act_hidden, activation_func act_output) 
{
    NN *nn = malloc(sizeof(NN));
    assert(nn != NULL); 

    nn->input = mat_alloc(1, arch[0]);
    nn->output = mat_alloc(1, arch[n_arch - 1]);

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
        mat_fill(nn->bs[i], 0);;
        mat_fill(nn->g_ws[i], 0);
        mat_fill(nn->g_bs[i], 0);
    }

    nn->act_hidden = act_hidden;
    nn->act_output = act_output;

    nn->arch = malloc(sizeof(int) * n_arch);
    memcpy(nn->arch, arch, sizeof(int) * n_arch);
    nn->n_arch = n_arch;
    nn->training_rate = training_rate;

    return nn;
}

void nn_free(NN *nn) {
    if (nn == NULL) return;
    
    if (nn->input != NULL) mat_free(nn->input);
    if (nn->output != NULL) mat_free(nn->output);

    mat_free(nn->as[0]);
    mat_free(nn->g_as[0]);
    for (int i = 0; i < nn->n_arch - 1; i++) {
        mat_free(nn->ws[i]);
        mat_free(nn->bs[i]);
        mat_free(nn->as[i+1]);
        mat_free(nn->g_ws[i]);
        mat_free(nn->g_bs[i]);
        mat_free(nn->g_as[i+1]);
    }
    free(nn->ws);
    free(nn->bs);
    free(nn->as);
    free(nn->g_ws);
    free(nn->g_bs);
    free(nn->g_as);

    free(nn->arch);
    free(nn);
}

void nn_setInput(NN *nn, matrix *input) {
    //mat_normalize(input);
    mat_copy(nn->input, input);
    mat_copy(nn->as[0], input);
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
    for (int i = 0; i < nn->n_arch - 1; i++) {
        mat_mult(nn->as[i+1], nn->as[i], nn->ws[i]);
        mat_add(nn->as[i+1], nn->bs[i]);
        activation_func curr_act = (i == nn->n_arch - 2) ? 
                                    nn->act_output : nn->act_hidden;
        switch (curr_act) {
            case NONE: break;
            case RELU: mat_apply(nn->as[i+1], act_relu); break;
            case SIGMOID: mat_apply(nn->as[i+1], act_sigmoid); break;
            default: break;
        } 
    }
    nn_setOutput(nn, nn->as[nn->n_arch - 1]);
}

void nn_train(NN *nn, matrix *td_in, matrix *td_out, int epochs) {
    printf("TRAINING NEURAL NETWORK ...\n");
    for (int epoch = 0; epoch < epochs; epoch++) {
        nn_backprop(nn, td_in, td_out);
        nn_learn(nn);
        nn_forward(nn);

        float cost = 0.0f;
        for (int sample = 0; sample < td_in->rows; sample++) {
            float diff = 0.0f;
            for(int i = 0; i < td_in->cols; i++) {
               diff = MAT_AT(nn->output, 0, i) - MAT_AT(td_out, sample, i);
               cost += diff * diff;
            }
        }
        printf("training progress: %.2f%%\tcost: %f\r", ((float)epoch/(float)epochs)*100, cost / td_in->rows);
        fflush(stdout);
    }
    printf("\n");
}

void shuffle_indices(int *indices, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
                                            
    }
    
}

void nn_backprop(NN *nn, matrix *td_in, matrix *td_out) {
   int n_samples = td_in->rows;
   
   for (int i = 0; i < nn->n_arch - 1; i++) {
       mat_fill(nn->g_ws[i], 0);
       mat_fill(nn->g_bs[i], 0);
   }

   int *indices = malloc(sizeof(int) * n_samples);
   for (int i = 0; i < n_samples; i++) indices[i] = i;
   shuffle_indices(indices, n_samples);

   for (int sample_count = 0; sample_count < n_samples; sample_count++) {
       int sample = indices[sample_count];
       float *row = &td_in->data[sample * td_in->cols];
       for (int i = 0; i < td_in->cols; i++) {
           nn->as[0]->data[i] = row[i];
       }
       //mat_normalize(nn->as[0]);
       nn_forward(nn);

       for (int i = 0; i < nn->n_arch; i++) {
           mat_fill(nn->g_as[i], 0);
       }

       for (int i = 0; i < td_out->cols; i++) {
           MAT_AT(nn->g_as[nn->n_arch - 1], 0, i) = MAT_AT(nn->output, 0, i) -
                                     MAT_AT(td_out, sample, i);
       }
       for (int layer = nn->n_arch - 1; layer > 0; layer--) {
           activation_func curr_act = (layer == nn->n_arch - 1) ?
                                      nn->act_output : nn->act_hidden;

           for (int curr = 0; curr < nn->as[layer]->cols; curr++) {
               float a = MAT_AT(nn->as[layer], 0, curr);
               float delta_a;
               switch (curr_act) {
                   case NONE: delta_a = 1.0f; break;
                   case RELU: delta_a = act_relu_d(a); break;
                   case SIGMOID: delta_a = act_sigmoid_d(a); break;
                   default: delta_a = 1.0f; break;
               }
               float error = MAT_AT(nn->g_as[layer], 0, curr) * delta_a;

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

   for (int i = 0; i < nn->n_arch - 1; i++) {
           //printf("Layer %d weight gradient [0]: %f\n", i, nn->g_ws[i]->data[0]);
           
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


