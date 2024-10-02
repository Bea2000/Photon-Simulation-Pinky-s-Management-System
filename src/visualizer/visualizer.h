#pragma once

#include "../engine/particle.h"
/****************************************************************************/
/*                              Visualizer                                  */
/*                                                                          */
/* Módulo a cargo de la visualización del problema.                         */
/****************************************************************************/

/** Abre una ventana para mostrar una imagen de las dimensiones dadas */
void visualizer_open(int height, int width, int particle_count);
/** Selecciona un color para las siguientes operaciones de dibujo */
void visualizer_set_color(double R, double G, double B);
/** Dibuja un segmento de recta de un punto a otro del color seleccionado */
void visualizer_draw_segment(Segment segment);
/** Dibuja una caja del color seleccionado */
void visualizer_draw_box(BoundingBox box);
/** Dibuja las particulas dadas */
void visualizer_draw_particles(Particle* particles, int particle_count);
/** Genera una imagen PNG con el contenido actual de la ventana */
void visualizer_snapshot(char* filename);
/** Cierra y libera los recursos de la ventana */
void visualizer_close();
