#include "neuralnetwork.h"

#define TRAINING_RATE 1.0f
#define ACT_HIDDEN SIGMOID
#define ACT_OUTPUT SIGMOID


int main(void) {
    /* TODO: IMPLEMENT EXAMPLE AND TEST NETWORK */ 
    
    activation_func act_hidden = ACT_HIDDEN;
    activation_func act_output = ACT_OUTPUT;

    int arch[] = {2, 2, 1};
    int n_arch = sizeof(arch)/sizeof(arch[0]);
    NN *nn = nn_alloc(arch, n_arch, TRAINING_RATE,
                      act_hidden, act_output);
    nn_print(nn);
    nn_train(nn, td_in, td_out, 1000);
    nn_print(nn);
    nn_free(nn);
    return 0;

}
