#pragma once

#include "math/geometry.h"

/** Representa una partícula dentro de la simulación */
typedef struct particle
{
  /** Cuerpo con el que la partícula interactúa con los segmentos */
  Circle body;
  /** Velocidad vectorial de la partícula */
  Vector velocity;
  /** Identificador de la partícula */
  int ID;
  /** Segmento con el que intersectó la partícula en el último tick */
  Segment* intersected_segment;
} Particle;

/** Indica si una particula está chocando con una caja
 * @param particle la particula
 * @param boundingbox la caja
 * @return true si la partícula está chocando con la caja, false si no
 */
bool particle_boundingbox_collision(Particle particle, BoundingBox boundingbox);

/** Indica si una partícula está chocando con un segmento de recta
 * @param particle la particula
 * @param segment el segmento
 * @return true si la partícula está chocando con el segmento, false si no
 */
bool particle_segment_collision(Particle particle, Segment segment);

/** Cambia la dirección de la partícula luego de chocar con un segmento. 
 * No chequea colisión.
 * @param particle puntero a la partícula. Modifica su campo "velocity"
 * @param segment segmento según el cual se calcula la nueva dirección.
 */
void particle_bounce_against_segment(Particle* particle, Segment segment);

/** Actualiza la posición de la particula según su velocidad.
 * @param particle puntero a la particula. Modifica su campo "body.center"
 */
void particle_move(Particle* particle);
