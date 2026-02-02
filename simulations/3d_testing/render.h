#ifndef RENDER_H
#define RENDER_H

#include "util.h"

typedef struct {
    float fov;
    float aspect_ratio;
    float far;
    float near;
} camera;

mat4 createProjMat(camera cam) {
    mat4 m = mat4_create();
    m.m[0][0] = cam.aspect_ratio / tanf(cam.fov / 2);
    m.m[1][1] = 1 / tanf(cam.fov / 2);
    m.m[2][2] = cam.far / (cam.far - cam.near);
    m.m[2][3] = (-cam.far * cam.near) / (cam.far - cam.near);
    m.m[3][2] = 1;
    return m;
}



#endif
