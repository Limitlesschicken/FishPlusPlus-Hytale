/*
 * Copyright (c) FishPlusPlus.
 */
#pragma once

struct Color {
	float r;
	float g;
	float b;
	float a;

	static Color Normalize(float r, float g, float b, float a = 255) {
		return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	static Color Normalize(Color color) {
		return Color(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);
	}

	static Color Blend(Color c1, Color c2, float weight) {
		return Color(
			c1.r * weight + c2.r * (1.0f - weight),
			c1.g * weight + c2.g * (1.0f - weight),
			c1.b * weight + c2.b * (1.0f - weight),
			c1.a * weight + c2.a * (1.0f - weight)
		);
	}

	Color operator=(Color value) {
		r = value.r;
		g = value.g;
		b = value.b;
		a = value.a;
		return *this;
	}

	Color operator+=(Color value) {
		r += value.r;
		g += value.g;
		b += value.b;
		a += value.a;
		return *this;
	}

	static Color Red() {
		return Color(255, 0, 0, 255);
	}

	static Color Green() {
		return Color(0, 255, 0, 255);
	}

	static Color Blue() {
		return Color(0, 0, 255, 255);
	}

	static Color White() {
		return Color(255, 255, 255, 255);
	}

	static Color Black() {
		return Color(0, 0, 0, 255);
	}
};