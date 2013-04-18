#include "Core.h"

Real squaredMagnitude(Real* vector, int len) {
	Real result = 0;
	while (len--) {
		result += vector[len]*vector[len];
	}
	return result;
}

void sub(Real* v1, Real* v2, Real* out, int len) {
	while (len--) {
		out[len] = v1[len] - v2[len];
	}
}

void add(Real* v1, Real* v2, Real* out, int len) {
	while (len--) {
		out[len] = v1[len] + v2[len];
	}
}

void addScaled(Real* v1, Real* v2, Real* out, Real s, int len) {
	while (len--) {
		out[len] = v1[len] + v2[len]*s;
	}
}

void mul(Real* v, Real s, Real* out, int len) {
	while (len--) {
		out[len] = v[len] * s;
	}
}