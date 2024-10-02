#include "./vector.h"
#include <math.h>
/**
 * Define operaciones vectoriales 2D comunes
 */

#define INTERNAL_PI 3.14159265358979323846

Vector vector_add(Vector a, Vector b) {
  a.x += b.x;
  a.y += b.y;
  return a;
};

Vector vector_substract(Vector a, Vector b) {
  a.x -= b.x;
  a.y -= b.y;
  return a;
}

Vector vector_scale(Vector a, double b) {
  a.x *= b;
  a.y *= b;
  return a;
}

Vector vector_normalize(Vector a) {
  double norm = sqrt(pow(a.x, 2) + pow(a.y, 2));
  a.x /= norm;
  a.y /= norm;
  return a;
}

double vector_dot(Vector a, Vector b) { return a.x * b.x + a.y * b.y; }
