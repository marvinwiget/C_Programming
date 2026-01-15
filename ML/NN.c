#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define LEARNING_RATE 0.01
#define N_TRAINING 100
#define N_NEURONS 9
#define INIT_BIAS 0

/* ... */

// Neural Network should figure out to square.

double from[] = {-3, -2, -1, 0, 1, 2, 3};
double to[] = {9, 4, 1, 0, 1, 4, 9};

enum Activation {
	RELU,
	SOFTMAX
};

typedef struct {
	double *weights;
	double *biase;
	double *output;
	int n_input, n_output;
} Hidden_Layer;



void activation_ReLU(double *input, int n);
void activation_softmax(double *input, int n);

Hidden_Layer* init_hiddenLayer(int n_input, int n_output);
void forward(double *prev_output, Hidden_Layer *next, enum Activation flag);
void updateWeights(Hidden_Layer *l);
void printLayer(Hidden_Layer *l);
void destroy_hiddenLayer(Hidden_Layer *l);

double Loss();

int main() {
	srand(time(NULL));
	
	Hidden_Layer *layer1 = init_hiddenLayer(1, N_NEURONS);
	Hidden_Layer *layer2 = init_hiddenLayer(N_NEURONS, N_NEURONS);
	Hidden_Layer *output_layer = init_hiddenLayer(N_NEURONS, 1);

	for (int T=0; T < N_TRAINING; T++) {
		for (int i=0; i<7; i++) {
			enum Activation ACTIVATION_FLAG = RELU;

			forward(&from[i], layer1, ACTIVATION_FLAG );
			forward(layer1->output, layer2, ACTIVATION_FLAG);
			ACTIVATION_FLAG = SOFTMAX;
			forward(layer2->output, output_layer, ACTIVATION_FLAG);
			

			double error = output_layer->output[0] - to[i];
			printf("%f - ", output_layer->output[0]);
			//printf("%f - ", error);

			updateWeights(layer1);
			updateWeights(layer2);
			updateWeights(output_layer);


		}
		printf("\n");
	}

	
	



	destroy_hiddenLayer(layer1);
	destroy_hiddenLayer(layer2);
	destroy_hiddenLayer(output_layer);
	return 0;
}



void activation_ReLU(double *input, int n) {
	for (int i=0; i<n; i++) {
		input[i] = (input[i] > 0) ? input[i] : 0;
	}
	return;
}

void activation_softmax(double *input, int n) {
	double max = input[0];
	for (int i=0; i<n; i++) {
		max = (max < input[i]) ? input[i] : max;
	}
	double sum = 0;
	for (int i=0; i<n; i++) {
		input[i] = exp(input[i] - max);
		sum += input[i];
	}
	for (int i=0; i<n; i++) {
		input[i] = input[i] / sum;
	}
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
		l->weights[i] = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
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

void forward(double *prev_output, Hidden_Layer *next, enum Activation flag) {
	// dot product
	for (int i=0; i<next->n_output; i++) {
		double sum = 0.0;
		for (int j=0; j<next->n_input; j++) {
			sum += prev_output[j] * next->weights[i * next->n_input + j];
		}
		next->output[i] = sum + next->biase[i];
	}
	// activation function
	switch(flag) {
		case RELU:
			activation_ReLU(next->output, next->n_output);
			break;
		case SOFTMAX:
			//activation_softmax(next->output, next->n_output); //not necessary for this
			break;
	}
}

void updateWeights(Hidden_Layer *l) {
	for (int i=0; i<l->n_input*l->n_output; i++) {
		l->weights[i] = (((double)rand() / (double)RAND_MAX) * 2.0 - 1.0);
	}
}

void printLayer(Hidden_Layer *l) {
	
}

void destroy_hiddenLayer(Hidden_Layer *l) {
	free(l->weights);
	free(l->biase);
	free(l->output);
	free(l);
	return;
}


