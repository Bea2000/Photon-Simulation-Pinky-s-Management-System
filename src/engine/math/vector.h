#pragma once

typedef struct vector {
  double x;
  double y;
} Vector;

Vector vector_add(Vector a, Vector b);
Vector vector_substract(Vector a, Vector b);
Vector vector_scale(Vector a, double b);
Vector vector_normalize(Vector a);
double vector_dot(Vector a, Vector b);