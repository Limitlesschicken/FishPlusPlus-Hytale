#pragma once

struct Color {
	float r;
	float g;
	float b;
	float a;

	static Color From255(float r, float g, float b, float a = 255) {
		return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	static Color From255(Color color) {
		return Color(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	}

	Color operator=(Color value) {
		r = value.r;
		g = value.g;
		b = value.b;
		a = value.a;

		return value;
	}

	Color operator+=(Color value) {
		r += value.r;
		g += value.g;
		b += value.b;
		a += value.a;

		return Color(r, g, b, a);
	}

	static Color Red() {
		return Color(1, 0, 0, 1);
	}

	static Color Green() {
		return Color(0, 1, 0, 1);
	}

	static Color Blue() {
		return Color(0, 0, 1, 1);
	}

	static Color White() {
		return Color(1, 1, 1, 1);
	}

	static Color Black() {
		return Color(0, 0, 0, 1);
	}
};