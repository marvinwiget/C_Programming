#include "neuralnetwork.h"
#include <time.h>
#include <stdlib.h>

#define TRAINING_RATE 0.01f
#define ACT_HIDDEN RELU
#define ACT_OUTPUT NONE

#define ARR_SIZE(arr) sizeof(arr)/sizeof(arr[0])


int main(void) {

    srand(time(NULL));

    int td_in_rows = 9;
    int td_in_cols = 1;
    float td_in_arr[] = 
    {
       -5,-3, -2, -1, 0, 1, 2, 3, 5
    };

    int td_out_rows = td_in_rows;;
    int td_out_cols = 1;
    float td_out_arr[] = 
    {
        25,9,4,1,0,1,4,9,25
    };

    matrix *td_in = mat_arrToMat(td_in_arr, td_in_rows, td_in_cols);
    matrix *td_out = mat_arrToMat(td_out_arr, td_out_rows, td_out_cols);
    float scale = 25;
    mat_scale(td_in, ((float)1/scale));
    mat_scale(td_out, ((float)1/scale));


    int test_rows = td_in_cols;
    int test_cols = td_out_cols;
    float test_arr[] = {
        4
    };

    matrix *test = mat_arrToMat(test_arr, test_rows, test_cols);

    mat_scale(test, ((float)1/scale));

    //mat_print(td_in);
    //mat_print(td_out);

    activation_func act_hidden = ACT_HIDDEN;
    activation_func act_output = ACT_OUTPUT;

    int arch[] = {1,16,16, 1};
    int n_arch = sizeof(arch)/sizeof(arch[0]);
    NN *nn = nn_alloc(arch, n_arch, TRAINING_RATE,
                      act_hidden, act_output);
    nn_train(nn, td_in, td_out, 500*1000);
    //nn_print(nn);
    nn_setInput(nn, test);
    nn_forward(nn);
    mat_scale(nn->input, scale);
    mat_scale(nn->output, scale);
    nn_printResult(nn);
    nn_free(nn);
    return 0;

}
