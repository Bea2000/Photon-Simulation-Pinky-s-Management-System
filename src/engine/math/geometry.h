#pragma once

#include <stdbool.h>
#include "./vector.h"

/** Representa una caja alineada a los ejes */
typedef struct aabb {
  /** Esquina superior izquierda de la caja */
  Vector min_point;
  /** Esquina inferior derecha de la caja */
  Vector max_point;
} BoundingBox;

/** Representa un círculo */
typedef struct circle {
  /** Posición del centro del círculo */
  Vector center;
  /** Radio del círculo */
  double radius;
} Circle;

/** Representa un segmento de recta */
typedef struct segment {
  /** Punto inicial del segmento */
  Vector pi;
  /** Punto final del segmento*/
  Vector pf;
  /** Identificador del segmento */
  int ID;
} Segment;

/** Computa la intersección entre un circulo y una caja */
bool geometry_circle_bbox_coll(Circle circle, BoundingBox bbox);
/** Computa la interseccion entre un circulo y un segmento */
bool geometry_circle_segment_coll(Circle circle, Segment segment);
/** Computa el ángulo de reflexion de un vector respecto a un segmento */
Vector geometry_segment_reflection(Vector dir, Segment segment);
