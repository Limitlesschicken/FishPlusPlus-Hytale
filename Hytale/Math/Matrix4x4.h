#pragma once

#include "Vector4.h"

struct Matrix4x4 {
	float m[4][4];


    static Matrix4x4 Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        Matrix4x4 result = {};

        result.m[0][0] = 2.0f / (right - left);
        result.m[1][1] = 2.0f / (top - bottom);
        result.m[2][2] = -2.0f / (farPlane - nearPlane);

        result.m[3][0] = -(right + left) / (right - left);
        result.m[3][1] = -(top + bottom) / (top - bottom);
        result.m[3][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        result.m[3][3] = 1.0f;

        return result;
    }
};