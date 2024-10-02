/**
 * HERE BE DRAGONS
 * No intentes entender este código
 * Leelo bajo tu propio riesgo
 * Para más información, visita https://www.youtube.com/watch?v=siwpn14IE7E
 */

#include "./geometry.h"
#include <math.h>

#define PRECISION_DELTA 0.0000001

static bool geometry_point_in_circle(Vector pos_c, double radius_c, Vector point) {
  double r_sqr = pow(radius_c, 2);
  double dist_sqr = pow(point.x - pos_c.x, 2) + pow(point.y - pos_c.y, 2);
  return r_sqr >= dist_sqr;
}

// // TODO: REMOVE UNUSED
// static bool geometry_point_in_rectangle(Vector point, Vector rect_1, Vector rect_2) {
//   return (point.x >= rect_1.x && point.x <= rect_2.x) &&
//          (point.y >= rect_1.y && point.y <= rect_2.y);
// }

static double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;
}

// // TODO: REMOVE UNUSED
// static bool geometry_circle_coll(Vector pos_a, double radius_a, Vector pos_b,
//                       double radius_b) {
//   double dist_sqr = pow(pos_a.x - pos_b.x, 2) + pow(pos_a.y - pos_b.y, 2);
//   double rad_sum_sqr = pow(radius_a + radius_b, 2);

//   return dist_sqr <= rad_sum_sqr;
// }

// Don't mess with cross product gang.
static bool geometry_vect_are_parallel(Vector v1, Vector v2) {
  v1 = vector_normalize(v1);
  v2 = vector_normalize(v2);
  return fabs(v1.x * v2.y - v1.y * v2.x) < PRECISION_DELTA;
}

static Vector geometry_segment_normal(Vector s) {
  Vector segment = vector_normalize(s);
  Vector new;
  new.x = segment.y * 1;
  new.y = segment.x * -1;
  return new;
}

bool geometry_circle_bbox_coll(Circle c, BoundingBox bbox) {
  Vector pos_c = c.center;
  double radius_c = c.radius;
  Vector bbox_1 = bbox.min_point;
  Vector bbox_2 = bbox.max_point;

  float close_x = clamp(pos_c.x, bbox_1.x, bbox_2.x);
  float close_y = clamp(pos_c.y, bbox_1.y, bbox_2.y);

  float x_dist = pos_c.x - close_x;
  float y_dist = pos_c.y - close_y;

  float sqr_dist = (x_dist * x_dist) + (y_dist * y_dist);
  return sqr_dist <= (radius_c * radius_c);
}

bool geometry_circle_segment_coll(Circle circle, Segment segment) {
  Vector center = circle.center;
  double radius = circle.radius;
  Vector s1 = segment.pi;
  Vector s2 = segment.pf;

  if (geometry_point_in_circle(center, radius, s1) ||
      geometry_point_in_circle(center, radius, s2)) {
    return true;
  }

  Vector d = vector_substract(s2, s1);
  Vector f = vector_substract(s1, center);
  double a = vector_dot(d, d);
  double b = 2 * vector_dot(f, d);
  double c = vector_dot(f, f) - pow(radius, 2);

  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return false;
  } else {
    discriminant = sqrt(discriminant);

    float t1 = (-b - discriminant) / (2 * a);
    float t2 = (-b + discriminant) / (2 * a);

    if (t1 >= 0 && t1 <= 1) {
      return true;
    }

    if (t2 >= 0 && t2 <= 1) {
      return true;
    }
    return false;
  }
}

Vector geometry_segment_reflection(Vector dir, Segment segment) {
  Vector s1 = segment.pi;
  Vector s2 = segment.pf;
  Vector s = vector_substract(s2, s1);
  if (vector_dot(s, dir) == 0 || geometry_vect_are_parallel(s, dir)) {
    return vector_scale(dir, -1);
  }
  Vector normal = geometry_segment_normal(s);
  double len = 2 * vector_dot(dir, normal);
  normal = vector_scale(normal, len);

  Vector reflection = vector_substract(dir, normal);

  return reflection;
}
