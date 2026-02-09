#pragma once

struct Vector4 {
	float x;
	float y;
	float z;
	float w;

	Vector4 operator=(float value) {
		x = value;
		y = value;
		z = value;
		w = value;
	}

	Vector4 operator=(Vector4 value) {
		x = value.x;
		y = value.y;
		z = value.z;
		w = value.w;

		return value;
	}

	Vector4 operator+=(Vector4 value) {
		x += value.x;
		y += value.y;
		z += value.z;
		w += value.w;

		return Vector4(x, y, z, w);
	}
};