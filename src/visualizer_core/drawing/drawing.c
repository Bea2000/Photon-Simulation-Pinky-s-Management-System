#include "drawing.h"

#include <cairo-pdf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/** El mutex de dibujo */
static pthread_mutex_t drawing_mutex;

Color photon_color = (Color){.R = 1.0, .G = 1.0, .B = 0.0};

/** Multiplica un color por un escalar, para aclararlo o oscurecerlo */
// static Color color_scale(Color color, double k)
// {
//   return (Color){.R = color.R * k,.G = color.G * k, .B = color.B * k};
// }

// /** Setea el color RGB de cairo */
// static void cairo_dip(cairo_t* context, Color color)
// {
//   cairo_set_source_rgb(context,color.R,color.G, color.B);
// }
//
static void cairo_dip_alpha(cairo_t* context, Color color, double alpha) {
  cairo_set_source_rgba(context, color.R, color.G, color.B, alpha);
}

static void cairo_circle(cairo_t* context, double xc, double yc, double rad) {
  cairo_arc(context, xc, yc, rad, 0, 2 * M_PI);
}

static void cairo_line(cairo_t* context, double x1, double y1, double x2,
                       double y2) {
  cairo_move_to(context, x1, y1);
  cairo_line_to(context, x2, y2);
}

/** Dibuja un segmento de recta entre dos puntos */
void drawing_draw_segment(Content* cont, Segment s) {
  cairo_t* context = cairo_create(cont->segment_layer);

  cairo_dip_alpha(context, cont->current_color, 1);
  cairo_line(context, s.pi.x, s.pi.y, s.pf.x, s.pf.y);
  cairo_stroke(context);
  cairo_destroy(context);
}

bool drawing_draw(cairo_t* restrict context, Content* restrict cont) {
  pthread_mutex_lock(&drawing_mutex);

  /* Color de fondo */
  cairo_set_source_rgb(context, 0, 0, 0);
  cairo_paint(context);

  /* Segmentos */
  cairo_set_source_surface(context, cont->segment_layer, 0, 0);
  cairo_paint(context);

  /* Fotones */
  cairo_dip_alpha(context, photon_color, 1);
  for (int i = 0; i < cont->particle_count; i++) {
    Circle c = cont->particle_colliders[i];
    cairo_circle(context, c.center.x, c.center.y, c.radius);
    cairo_fill(context);
  }

  pthread_mutex_unlock(&drawing_mutex);

  return true;
}

static cairo_surface_t* init_vector_image(int height, int width) {
  /* El formato de imagen: R G B de 8 bits cada uno */
  cairo_format_t format = CAIRO_FORMAT_ARGB32;
  /** La imagen vectorial misma */
  return cairo_image_surface_create(format, width, height);
}

/** Genera los contenedores para las dos imagenes superpuestas */
Content* drawing_init(int height, int width, int particle_count) {
  Content* cont = malloc(sizeof(Content));

  cont->height = height;
  cont->width = width;
  cont->particle_count = particle_count;

  cont->particle_colliders = calloc(particle_count, sizeof(Circle));

  cont->current_color = color_init(1, 1, 1);

  /* Inicializa las imagenes vectoriales */
  cont->segment_layer = init_vector_image(cont->height, cont->width);

  pthread_mutex_init(&drawing_mutex, NULL);

  return cont;
}

/** Geenera una imagen en pdf para un estado en particular */
void drawing_snapshot(Content* cont, char* filename) {
  double width = cont->width;
  double height = cont->height;

  /* Imprimimos las imagenes del tablero */
  cairo_surface_t* surface;
  cairo_t* context;

  surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);

  // surface = cairo_pdf_surface_create (filename, width, height);
  context = cairo_create(surface);

  /* Reseteamos los parámetros para generar correctamente la imagen */
  Content aux = *cont;

  /* Dibuja el estado actual */
  drawing_draw(context, &aux);

  cairo_surface_write_to_png(surface, filename);

  cairo_surface_destroy(surface);
  cairo_destroy(context);
}

/** Libera los recursos asociados a las herramientas de dibujo */
void drawing_free(Content* cont) {
  cairo_surface_destroy(cont->segment_layer);
  free(cont->particle_colliders);
  free(cont);
  pthread_mutex_destroy(&drawing_mutex);
}
