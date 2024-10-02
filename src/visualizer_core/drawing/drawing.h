#pragma once

#include <cairo.h>
#include <pthread.h>
#include <stdbool.h>

#include "../../engine/particle.h"
#include "color.h"

/** Contiene la información de lo que ha de ser dibujado en el canvas */
struct content {
  /** Ancho de la ventana */
  int width;
  /** Alto de la ventana */
  int height;
  /** Color actual */
  Color current_color;
  /** Capa que guarda los segmentos dibujados */
  cairo_surface_t* segment_layer;
  /** Arreglo de las partículas */
  Circle* particle_colliders;
  /** Cantidad de partículas */
  int particle_count;
};
/** Contiene la información de lo que ha de ser dibujado en el canvas */
typedef struct content Content;

/** Inicializa las herramientas de dibujo*/
Content* drawing_init(int height, int width, int particle_count);
/** Dibuja sobre el canvas dado */
bool drawing_draw(cairo_t* cr, Content* cont);
/** Dibuja un segmento de recta entre dos puntos */
void drawing_draw_segment(Content* cont, Segment segment);
/** Genera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename);
/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont);
