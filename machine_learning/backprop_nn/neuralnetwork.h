#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "matrix.h"

typedef enum {
    NONE,
    RELU,
    SIGMOID,
} activation_func;

typedef struct {
    matrix *input;
    matrix *output;

    matrix **ws;
    matrix **bs;
    matrix **as;

    matrix **g_ws;
    matrix **g_bs;
    matrix **g_as;

    activation_func act_hidden;
    activation_func act_output;
    int *arch; // 0 index: input layer, n input: output layer
    int n_arch; 
    float training_rate;
} NN;

NN* nn_alloc(
        int arch[], int n_arch, float training_rate,
        activation_func act_hidden, activation_func act_output);
void nn_free(NN *nn);
void nn_setInput(NN *nn, matrix *input);
void nn_setOutput(NN *nn, matrix *output);
void nn_print(NN *nn);
void nn_printResult(NN *nn);

void nn_forward(NN *nn);
void nn_train(NN *nn, matrix *td_in, matrix *td_out, int epochs);
void nn_backprop(NN *nn, matrix *td_in, matrix *td_out);;
void nn_learn(NN *nn);


#endif
