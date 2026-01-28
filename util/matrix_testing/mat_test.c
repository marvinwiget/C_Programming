#include "../matrix.h"

int main(void) {
    matrix *m1 = mat_alloc(5, 5);
    matrix *m2 = mat_alloc(5, 5);
    matrix *out = mat_alloc(5, 5);

    mat_randomize(m1);
    mat_fill(m2, 3);
    mat_add(m2, m1);
    mat_scale(m1, 2);

    mat_print(m1);
    mat_print(m2);
    mat_print(out);
    mat_mult(out, m1, m2);

    mat_print(out);

    mat_relu(out, m1);
    mat_sigmoid(m2, m1);
    mat_print(out);
    mat_print(m2);

    mat_free(m1);
    mat_free(m2);
    mat_free(out);
    return 0;
}
