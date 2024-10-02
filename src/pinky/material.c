#include <stdlib.h>
#include <stdbool.h>
#include "material.h"
#include <stdio.h>

struct material* insert(struct material *root, int x)
{
    if(root==NULL){
        return new_node(x);
    }
    if(x>=root->value){ // si x es mayor, insertamos a la derecha
        root->right = insert(root->right ,x);
    }else{ // si x es menor insertamos a la izquierda
        root->left = insert(root->left ,x);
    }
    return root;
}

struct material *new_node(int x) {
    struct material *temp = (struct material *)malloc(sizeof(struct material));
    temp->value = x;
    temp->left = temp->right = NULL;
    return temp;
}

struct material* path(struct material* root, int x, FILE* file){
    // caso base: raiz vacía
    if (root == NULL){
        fprintf(file, "X\n");
        return root;
    }
    // caso base: el valor en la raíz
    if (root->value == x){
        fprintf(file, "%d \n",x);
        return root;
    }

    // x mayor a valor de la raíz
    if (root->value < x){
        fprintf(file, "%d ",root->value);
        return path(root->right, x, file);
    }

    // x menor a valor de la raíz
    fprintf(file, "%d ",root->value);
    return path(root->left, x, file);
}

struct material* deep(struct material* root, int x, FILE* file, int h){
    // caso base: raíz vacía
    if (root == NULL){
        fprintf(file, "X\n");
        return root;
    }
    // caso base: el valor en la raíz
    if (root->value == x){
        fprintf(file, "%d\n", h);
        return root;
    }

    // x mayor a valor de la raíz
    if (root->value < x){
        h = h + 1;
        return deep(root->right, x, file, h);
    }

    // x menor a valor de la raíz
    h = h + 1;
    return deep(root->left, x, file, h);
}

struct material* destroy(struct material* root, int x, FILE* file, int qty){
    if(qty == 0){
        root = path_destroy(root, x, file);
        if(root != NULL){
            if(root->left != NULL){
                if (root->left->value == x){
                    int nodes_qty = count_nodes(root->left);
                    //printf("HOLA\n");
                    fprintf(file, "%d\n", nodes_qty);
                    destroy(root->left, x, file, qty+1);
                    root->left = NULL;
                    free(root->left);
                } else if (root->right != NULL){
                    if (root->right->value == x){
                        int nodes_qty = count_nodes(root->right);
                        //printf("PAN\n");
                        fprintf(file, "%d\n", nodes_qty);
                        destroy(root->right,x,file,qty+1);
                        root->right = NULL;
                        free(root->right);
                    }
                }
            }
            else if(root->right != NULL){
                if (root->right->value == x){
                    int nodes_qty = count_nodes(root->right);
                    //printf("AWA\n");
                    fprintf(file, "%d\n", nodes_qty);
                    destroy(root->right,x,file,qty+1);
                    root->right = NULL;
                    free(root->right);
                }
            }
        } else{
            fprintf(file, "%d\n", 0);
        }
    }

    if (root == NULL || (root->left == NULL && root->right == NULL)){
        root = NULL;
        free(root);
        return root;
    }
    if (root->left != NULL && qty!=0){
        destroy(root->left, x, file, qty+1);
    }
    else if(root->right != NULL && qty!=0){
        destroy(root->right, x, file, qty+1);
    }
 }

int count_nodes(struct material* root){

        // caso base
        if(root==NULL)
            return 0;

        //se llama recursivamente a hijos
        // + 1 contando raíz
        return 1 + count_nodes(root->left) + count_nodes(root->right);
    }

void order(struct material* root, FILE* file){
    if (root == NULL){
        return ;
    }
    order(root->left, file);
    fprintf(file, "%d ", root->value);
    order(root->right, file);
}

// eliminar file ??
struct material* path_destroy(struct material* root, int x, FILE* file){
    // caso base: raíz vacía
    if (root == NULL){
        return root;
    }
    // caso base: siguiente es x
    if (root->right != NULL){
        if (root->right->value == x){
            return root;
        }
    }
    if (root->left != NULL){
        if(root->left->value == x){
            return root;
        }
    }

    // x es mayor a raíz
    if (root->value < x){
        return path_destroy(root->right, x, file);
    }

    // x es menor a raíz
    return path_destroy(root->left, x, file);
}