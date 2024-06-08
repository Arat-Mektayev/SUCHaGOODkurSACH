#pragma once

double* inverted_vector(double* d) {
	double result[] = { -d[0], -d[1], -d[2] };
	return result;
}

double* Norm(double* a, double* b, double* c) {
	double BA[] = { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
	double BC[] = { -b[0] + c[0], -b[1] + c[1], -b[2] + c[2] };
	double n[] = { BA[1] * BC[2] - BC[1] * BA[2], -BA[0] * BC[2] + BC[0] * BA[2], BA[0] * BC[1] - BC[0] * BA[1] };
	double length = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	double result[] = { -n[0] / length, -n[1] / length, -n[2] / length };
	return result;
}