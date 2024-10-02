#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"
#include "../engine/particle.h"
#include "../engine/math/geometry.h"

#pragma once

struct Near_Segment;
typedef struct near_segment Near_Segment;

struct near_segment {
  Segment** value;
  float median;
  int qty;
  struct near_segment *left;
  struct near_segment *right;
};

Near_Segment* find_path(struct aabb* box, struct near_segment* root, Particle particle, bool x);
Near_Segment* n_segment_init(Segment** segments, int qty, float median);
struct near_segment* generate_tree(struct near_segment* root, int qty, bool x);
float find_median(struct segment* seg, int n, bool x);
float* sort(struct segment* seg,float* a, int n, bool x);
void segment_destroy(struct near_segment* root);