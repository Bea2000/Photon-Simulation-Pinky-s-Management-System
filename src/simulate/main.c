#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulation.h"
#include "../visualizer/visualizer.h"
#include "segment.h"
#include "../engine/math/geometry.h"

int main(int argc, char **argv)
{
  // Por defecto se abre la ventana
  bool visualize = true;
  if (argc == 4 && !strcmp(argv[3], "--novis"))
  {
    visualize = false;
  }
  else if (argc < 3 || argc >= 4)
  {
    printf("Modo de uso: %s INPUT OUTPUT [--novis]\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta al archivo de input que describe la escena\n");
    printf("\tOUTPUT es la ruta al archivo en donde se reportarán las colisiones\n");
    printf("\tEl flag opcional --novis indica que no se debe abrir la visualización del programa\n");
    // Exit code 1: Programa llamado sin todos los argumentos
    return 1;
  }

  // Inicializa los elementos de la simulación, y abre la ventana si visualize es true
  Simulation *sim = simulation_init_from_file(argv[1], visualize);

  // El archivo donde iremos reportando las colisiones
  FILE *output_file = fopen(argv[2], "w");

  Segment** segs = malloc(sizeof(Segment*)*sim->segment_count);
  for (int i=0; i<sim->segment_count; i++){
    segs[i] = &(sim->segments)[i];
  }

  struct aabb* box = malloc(sizeof(BoundingBox));
  double min_x = (double)sort(sim->segments, malloc(sizeof(float)*sim->segment_count), sim->segment_count, true)[0];
  double min_y = (double)sort(sim->segments, malloc(sizeof(float)*sim->segment_count), sim->segment_count, false)[0];
  double max_x = (double)sort(sim->segments, malloc(sizeof(float)*sim->segment_count), sim->segment_count, true)[sim->segment_count - 1];
  double max_y = (double)sort(sim->segments, malloc(sizeof(float)*sim->segment_count), sim->segment_count, false)[sim->segment_count - 1];
  box->min_point.x = min_x;
  box->min_point.y = min_y;
  box->max_point.x = max_x;
  box->max_point.y = max_y;
  struct near_segment* root = n_segment_init(segs, sim->segment_count, find_median(sim->segments, sim->segment_count, true));
  generate_tree(root,sim->segment_count,true);

  // Para cada frame
  for (int f = 0; f < sim->frames; f++)
  {
    // Para cada particula
    for (int p = 0; p < sim->particle_count; p++)
    {
      // Inicialmente, esta particula no ha chocado con ningun segmento
      sim->particles[p].intersected_segment = NULL;
      // Por cada segmento
      // TODO: No revisar todos los segmentos, usar BVH para descartar los que estén muy lejos

      struct near_segment* segment_root = find_path(box, root, sim->particles[p], true);
      //for( int s = 0; s < sim->segment_count; s++)
      if(segment_root){
        for (int s = 0; s < segment_root->qty; s++)
        {
          // Si la particula choca con el segmento
          //if (particle_segment_collision(sim->particles[p], sim->segments[s]))
          if (particle_segment_collision(sim->particles[p], *segment_root->value[s]))
          {
            // Si es que no ha chocado con nada, o si no desempatamos por ID
            //if (!sim->particles[p].intersected_segment || sim->segments[s].ID < sim->particles[p].intersected_segment->ID)
            if (!sim->particles[p].intersected_segment || (*segment_root->value[s]).ID < sim->particles[p].intersected_segment->ID)
            {
              //sim->particles[p].intersected_segment = &sim->segments[s];
              sim->particles[p].intersected_segment = &*segment_root->value[s];
            }
          }
        }
      }
        // Si hubo intersección
      if (sim->particles[p].intersected_segment)
      {
        // Desviamos la partícula según el segmento con el que chocó
        particle_bounce_against_segment(&sim->particles[p], *sim->particles[p].intersected_segment);
        // Reportamos la colisión en el archivo de output
        fprintf(output_file, "%d %d %d\n", f, sim->particles[p].ID, sim->particles[p].intersected_segment->ID);
      }
      // Finalmente, la particula avanza
      particle_move(&sim->particles[p]);
    }
    // Si la ventana está abierta, dibujar las particulas en sus nuevas posiciones
    visualizer_draw_particles(sim->particles, sim->particle_count);
  }
  // Hace free de todo lo que se creo en simulation_init. Cierra la ventana.
  segment_destroy(root);
  simulation_destroy(sim);

  fclose(output_file);

  return 0;
}
