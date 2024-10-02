#include <stdio.h>
#include <stdlib.h>
#pragma once

struct material;
typedef struct node Node;

struct material {
  int value;
  struct material *left;
  struct material *right;
};

struct material* insert(struct material *root, int x);
struct material *new_node(int x);
struct material* path(struct material* root, int x, FILE* file);
struct material* deep(struct material* root, int x, FILE* file, int deep);
struct material* destroy(struct material* root, int x, FILE* file, int qty);
struct material* path_destroy(struct material* root, int x, FILE* file);
void order(struct material* root, FILE* file);
int count_nodes(struct material *root);