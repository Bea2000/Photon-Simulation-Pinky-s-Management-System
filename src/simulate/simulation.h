#pragma once

#include "../engine/particle.h"

typedef struct simulation
{
  /** Cantidad total de frames que debe durar la simulación */
  int frames;
  /** Arreglo de partículas */
  Particle* particles;
  /** Largo del arreglo "particles" */
  int particle_count;
  /** Arreglo de segmentos */
  Segment* segments;
  /** Largo del arreglo "segments" */
  int segment_count;
} Simulation;

/** Lee una simulación a partir de un archivo, y abre la interfaz gráfica para visualizarla
 * @param input_file ruta al archivo que describe la simulación
 * @param visualize indica si debe abrirse o no la interfaz gráfica
 * @returns objeto Simulation* que contiene las entidades de la simulación
 */
Simulation* simulation_init_from_file(char* input_file, bool visualize);

/** Libera todos los recursos asociados a la simulación 
 * @param sim simulación construida por la función simulation_init_from_file()
 */
void   simulation_destroy(Simulation* sim);

