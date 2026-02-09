#pragma once

struct Vector2 {
	float x;
	float y;

	Vector2 operator=(float value) {
		x = value;
		y = value;
	}

	Vector2 operator=(Vector2 value) {
		x = value.x;
		y = value.y;

		return value;
	}

	Vector2 operator-(Vector2 value) {
		return Vector2(x - value.x, y - value.y);
	}

	Vector2 operator+(Vector2 value) {
		return Vector2(x + value.x, y + value.y);
	}

	Vector2 operator+(float value) {
		return Vector2(x + value, y + value);
	}
};