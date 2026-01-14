#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define u32 uint32_t
#define i32 int32_t

#define N_HIDDEN_LAYERS 5
#define INIT_BIAS 0

/* ... */

// Neural Network should figure out to square.

double from[] = {-3, -2, -1, 0, 1, 2, 3};
double to[] = {9, 4, 1, 0, 1, 4, 9};

typedef struct {
	double *weights;
	double *biase;
	double *output;
	int n_input, n_output;
} Hidden_Layer;


double dotProduct(double *x, double *y);

Hidden_Layer* init_hiddenLayer(int n_input, int n_output);
void forward(double *prev_output, int n_prev_output, Hidden_Layer *next);
void destroy_hiddenLayer(Hidden_Layer *l);

void activation_ReLU(int *x) {
	if (*x < 0) *x = 0;
	return;
}

int main() {
	srand(time(NULL));
	
	Hidden_Layer *layer1 = init_hiddenLayer(3, 3);
	forward(&from[0], 7, layer1);

	Hidden_Layer *layer2 = init_hiddenLayer(3,3);
	forward(layer1->output, layer1->n_output, layer2);



	destroy_hiddenLayer(layer1);
	return 0;
}

double dotProdut(double *x, double *y, int n) {
	double sum;
	for (int i=0; i<n; i++) {
		sum += x[i] + y[i];
	}
	return sum;
}

Hidden_Layer* init_hiddenLayer(int n_input, int n_output) {
	Hidden_Layer *l = malloc(sizeof(Hidden_Layer));
	// init weights
	l->weights = malloc(sizeof(double) * n_input * n_output);
	if (l->weights == NULL) {
		printf("weights mem alloc failed");
		return NULL;
	}
	for (int i=0; i<n_input * n_output; i++) {
		l->weights[i] = rand();
	}
	// init biase
	l->biase = malloc(sizeof(double) * n_output);
	for (int i=0; i<n_output; i++) {
		l->biase[i] = INIT_BIAS;
	}
	// init output
	l->output = malloc(sizeof(double) * n_output);
	if (l->output == NULL) {
		printf("output mem alloc failed");
		return NULL;
	}
	// init rest
	l->n_input = n_input;
	l->n_output = n_output;
	return l;
}

void forward(double *prev_output, int n_prev_output, Hidden_Layer *next) {
	
}

void destroy_hiddenLayer(Hidden_Layer *l) {
	free(l->weights);
	free(l->biase);
	free(l->output);
	free(l);
	return;
}
