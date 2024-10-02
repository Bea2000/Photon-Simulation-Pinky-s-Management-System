#include "visualizer.h"

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define VISUALIZER "./visualizer_core"

#define USECS_PER_FRAME 16667

static FILE* visualizer = NULL;

static clock_t previous_frame;

/** Abre una ventana para mostrar una matriz de las dimensiones dadas */
void visualizer_open(int height, int width, int particle_count) {
  setlocale(LC_ALL, "C");

  char command[256];

  sprintf(command, "%s %d %d %d", VISUALIZER, height, width, particle_count);

  if (visualizer) visualizer_close();

  visualizer = popen(command, "w");

  previous_frame = clock();
}

/** Selecciona un color para las siguientes operaciones de dibujo */
void visualizer_set_color(double R, double G, double B) {
  if (visualizer) {
    if (fprintf(visualizer, "C %lf %lf %lf\n", R, G, B) < 0) {
      visualizer_close();
    } else {
      fflush(visualizer);
    }
  }
}

/** Dibuja un segmento de recta de un punto a otro del color seleccionado */
void visualizer_draw_segment(Segment s) {
  if (visualizer) {
    if (fprintf(visualizer, "S %lf %lf %lf %lf\n", s.pi.x, s.pi.y, s.pf.x, s.pf.y) < 0) {
      visualizer_close();
    } else {
      fflush(visualizer);
    }
  }
}

/** Dibuja una caja del color seleccionado */
void visualizer_draw_box(BoundingBox box)
{
  visualizer_draw_segment((Segment){(Vector){box.min_point.x, box.min_point.y}, (Vector){box.min_point.x, box.max_point.y}, 0});
  visualizer_draw_segment((Segment){(Vector){box.min_point.x, box.min_point.y}, (Vector){box.max_point.x, box.min_point.y}, 0});
  visualizer_draw_segment((Segment){(Vector){box.max_point.x, box.min_point.y}, (Vector){box.max_point.x, box.max_point.y}, 0});
  visualizer_draw_segment((Segment){(Vector){box.min_point.x, box.max_point.y}, (Vector){box.max_point.x, box.max_point.y}, 0});
}

/** Dibuja un fotón en la posición especificada */
static void visualizer_draw_particle(int index, double ox, double oy, double r) {
  if (visualizer) {
    if (fprintf(visualizer, "P %d %lf %lf %lf\n", index, ox, oy, r) < 0) {
      visualizer_close();
    } else {
      fflush(visualizer);
    }
  }
}

/** Dibuja los fotones en las posiciones dadas */
void visualizer_draw_particles(Particle* particles, int particle_count) {
  for (int i = 0; i < particle_count; i++) {
    Circle circ = particles[i].body;
    visualizer_draw_particle(i, circ.center.x, circ.center.y, circ.radius);
  }
  if (visualizer) {
    if (fprintf(visualizer, "R\n") < 0) {
      visualizer_close();
    } else {
      fflush(visualizer);
      clock_t this_frame = clock();
      double elapsed_secs = (double)(this_frame - previous_frame) / CLOCKS_PER_SEC;
      int elapsed_usecs = elapsed_secs * 1000000;
      if(elapsed_usecs < USECS_PER_FRAME)
      {
        usleep(USECS_PER_FRAME - elapsed_usecs);
      }
      previous_frame = this_frame;
    }
  }  
}

/** Genera una imagen PNG con el contenido actual de la ventana */
void visualizer_snapshot(char* filename) {
  if (visualizer) {
    if (fprintf(visualizer, "W %s\n", filename) < 0) {
      visualizer_close();
    } else {
      fflush(visualizer);
    }
  }
}

/** Cierra y libera los recursos de la ventana */
void visualizer_close() {
  if (visualizer) {
    if (fprintf(visualizer, "%s\n", "X") >= 0) {
      fflush(visualizer);
      pclose(visualizer);
    }
    visualizer = NULL;
  }
}
