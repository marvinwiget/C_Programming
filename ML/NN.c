#include <stdio.h>
#include <stdlib.h>

#define N_HIDDEN_LAYERS 5
#define BIAS 0

/* ... */

typedef struct {
	double *weights;
	double *biase;
	double *output;
	int n_input, n_output;
} Layer;


void activation_ReLU();

int main() {
	return 0;
}
