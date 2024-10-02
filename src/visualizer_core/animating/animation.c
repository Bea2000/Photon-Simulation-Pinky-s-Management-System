#include "animation.h"

#include <locale.h>
#include <math.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../drawing/drawing.h"

/** El thread encargado de actualizar el contenido de la ventana */
static pthread_t* update_thread;

/** Lleva a cabo la actualización del tablero */
static void* update(void* ptr) {
  setlocale(LC_ALL, "C");

  /* Desencapsula los parámetros */
  void** param = ptr;
  GtkWidget* canvas = param[0];
  Content* cont = param[1];
  free(param);

  char command[64];

  while (update_thread) {
    /* Si alguno de los numeros falla, dejamos de llamar */
    if (fscanf(stdin, "%s", command)) {
      /* Cerrar ventana */
      if (!strcmp(command, "X")) {
        gtk_main_quit();
        break;
      }
      /* Elegir color */
      else if (!strcmp(command, "C")) {
        double R, G, B;
        fscanf(stdin, "%lf %lf %lf", &R, &G, &B) ?: abort();
        cont->current_color = color_init(R, G, B);
      }
      /* Dibujar línea */
      else if (!strcmp(command, "S")) {
        Segment s;
        fscanf(stdin, "%lf %lf %lf %lf", &s.pi.x, &s.pi.y, &s.pf.x, &s.pf.y) ?: abort();
        drawing_draw_segment(cont, s);
        gtk_widget_queue_draw(canvas);
      }
      /* Dibujar particula */
      else if (!strcmp(command, "P")) {
        int i;
        Circle c;
        fscanf(stdin, "%d %lf %lf %lf", &i, &c.center.x, &c.center.y, &c.radius) ?: abort();
        cont->particle_colliders[i] = c;
      }
      /* Actualiza la ventana */
      else if (!strcmp(command, "R")) {
        gtk_widget_queue_draw(canvas);
      }
      /* Guardar imagen */
      else if (!strcmp(command, "W")) {
        char filename[64];
        fscanf(stdin, "%s", filename) ?: abort();
        drawing_snapshot(cont, filename);
      }
      /* Comando desconocido */
      else {
        break;
      }
    } else {
      break;
    }
  }
  pthread_exit(NULL);
}

/** Inicializa el thread que animará el programa */
void animation_init(GtkWidget* canvas, gpointer user_data) {
  /* Encapsula ambos parámetros dentro de un mismo puntero */
  void** param = calloc(2, sizeof(void*));
  param[0] = canvas;
  param[1] = user_data;

  /* Inicializamos el thread */
  update_thread = malloc(sizeof(pthread_t));
  /* Y lo lanzamos */
  pthread_create(update_thread, NULL, update, param);
}

void animation_abort() {
  pthread_cancel(*update_thread);
  free(update_thread);
  update_thread = NULL;
}
