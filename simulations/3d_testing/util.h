#ifndef UTIL_H
#define UTIL_H

#include <math.h>

#define MAX(a, b) (a > b) ? a : b
#define MIN(a, b) (a < b) ? a : b


typedef struct {
    float x, y, z, w;
} vec4;

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    float m[4][4];
} mat4;

vec4 vec4_create(float x, float y, float z, float w) {
    return (vec4) {x, y, z, w};
}

vec3 vec3_create(float x, float y, float z) {
    return (vec3) {x, y, z};
}

vec3 vec3_add(vec3 v1, vec3 v2) {
    return vec3_create(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3 vec3_scale(vec3 v, float scalar) {
    return vec3_create(v.x * scalar, v.y * scalar, v.z * scalar);
}

mat4 mat4_create(void) {
    mat4 m = {0};
    return m;
}

mat4 mat4_identity(void) {
    mat4 m = mat4_create();
    for (int i = 0; i < 4; i++) m.m[i][i] = 1.0f;
    return m;
}

mat4 mat4_mul(mat4 a, mat4 b) {
    mat4 res = {0};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            for (int k = 0; k < 4; k++) {
                res.m[row][col] += a.m[row][k] * b.m[k][col];
            }
        }
    }
    return res;
}

vec4 mat4_mul_vec4(mat4 mat, vec4 vec) {
    return (vec4) {
        vec.x*mat.m[0][0] + vec.y*mat.m[1][0] + vec.z*mat.m[2][0] + vec.w*mat.m[3][0],
        vec.x*mat.m[0][1] + vec.y*mat.m[1][1] + vec.z*mat.m[2][1] + vec.w*mat.m[3][1],
        vec.x*mat.m[0][2] + vec.y*mat.m[1][2] + vec.z*mat.m[2][2] + vec.w*mat.m[3][2],
        vec.x*mat.m[0][3] + vec.y*mat.m[1][3] + vec.z*mat.m[2][3] + vec.w*mat.m[3][3]
    };
}

#endif
