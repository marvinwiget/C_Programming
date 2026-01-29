#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <math.h>



float act_sigmoid(float x) {
    return 1 / (1 + expf(-x));
}

float act_sigmoid_d(float x) {
    return act_sigmoid(x) * (1 - act_sigmoid(x));
}

float act_relu(float x) {
    return (x > 0) ? x : 0.0f;
}

float act_relu_d(float x) {
    return (x > 0) ? 1.0f : 0.0f;
}

float act_tanh(float x) {
    return (float) tanh(x);
}

float act_tanh_d(float x) {
    return 1.0f - tanh(x) * tanh(x);
}

#endif
