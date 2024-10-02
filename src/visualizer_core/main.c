#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "drawing/color.h"
#include "drawing/drawing.h"
#include "window.h"

/** Revisa que la cantidad de parámetros entregada sea correcta */
static bool check_parameters(int argc) { return argc == 4; }

int main(int argc, char** argv) {
  /* Se revisa que los parametros entregados sean correctos */
  if (!check_parameters(argc)) return 1;

  /* Las dimensiones de la imagen */
  int height = atoi(argv[1]);
  int width = atoi(argv[2]);
  int particle_count = atoi(argv[3]);

  /* Inicializa los contenedores */
  Content* cont = drawing_init(height, width, particle_count);

  /* Crea la ventana */
  window_create(cont);

  /* Libera los contenedores */
  drawing_free(cont);

  return 0;
}
