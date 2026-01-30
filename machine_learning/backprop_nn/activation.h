#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <math.h>


float act_sigmoid(float x) {
    return 1 / (1 + expf(-x));
}

float act_sigmoid_d(float a) {
    return a * (1.0f - a);
}

float act_relu(float x) {
    return (x > 0) ? x : 0.0f;
}

float act_relu_d(float a) {
    return (a > 0) ? 1.0f : 0.0f;
}

float act_tanh(float x) {
    return (float) tanhf(x);
}

float act_tanh_d(float a) {
    return 1.0f - a * a;
}

#endif
