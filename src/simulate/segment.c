#include <stdlib.h>
#include <stdbool.h>
#include "segment.h"
#include <stdio.h>

Near_Segment* n_segment_init(Segment** segments, int qty, float median)
    {
        Near_Segment* near_segment = malloc(sizeof(Near_Segment));

        *near_segment = (Near_Segment) {
            .value = segments,
            .median = median,
            .qty = qty,
            .right = NULL,
            .left = NULL,
        };
        return near_segment;
    }

struct near_segment* generate_tree(struct near_segment* root, int qty, bool x){
    if(qty > 2){
        if(x){
            int qty_xl = 0;
            int qty_xr = 0;
            Segment** left_x = malloc(sizeof(Segment*)*qty);
            Segment** right_x = malloc(sizeof(Segment*)*qty);
            for(int i=0; i<qty;i++){
                if((*root->value)[i].pi.x <= root->median || (*root->value)[i].pf.x <= root->median){
                    left_x[qty_xl]  = root->value[i];
                    qty_xl += 1;
                } if ((*root->value)[i].pi.x >= root->median || (*root->value)[i].pf.x >= root->median){
                    right_x[qty_xr]  = root->value[i];
                    qty_xr += 1;
                }
            }
            if(qty_xl >= 2){
                struct near_segment *new_root = (struct near_segment *)malloc(sizeof(struct near_segment));
                new_root->value = left_x;
                new_root->median = find_median(*left_x, qty_xl, false);
                new_root->qty = qty_xl;
                new_root->left = new_root->right = NULL;
                root->left = new_root;
            } if(qty_xr>=2){
                struct near_segment *new_root = (struct near_segment *)malloc(sizeof(struct near_segment));
                new_root->value = right_x;
                new_root->median = find_median(*right_x, qty_xr, false);
                new_root->qty = qty_xr;
                new_root->left = new_root->right = NULL;
                root->right = new_root;
            }
            generate_tree(root->left, qty_xl, false);
            generate_tree(root->right, qty_xr, false);
        
        }else{
            int qty_yl = 0;
            int qty_yr = 0;
            Segment** left_y = malloc(sizeof(Segment*)*qty);
            Segment** right_y = malloc(sizeof(Segment*)*qty);
            for(int i=0; i<qty;i++){
                if((*root->value)[i].pi.y <= root->median || (*root->value)[i].pf.y <= root->median){
                    left_y[qty_yl]  = root->value[i];
                    qty_yl += 1;
                } if ((*root->value)[i].pi.y >= root->median || (*root->value)[i].pf.y >= root->median){
                    right_y[qty_yr]  = root->value[i];
                    qty_yr += 1;
                }
            }
            if(qty_yl >= 2){
                struct near_segment *new_root = (struct near_segment *)malloc(sizeof(struct near_segment));
                new_root->value = left_y;
                new_root->median = find_median(*left_y, qty_yl, true);
                new_root->qty = qty_yl;
                new_root->left = new_root->right = NULL;
                root->left = new_root;
            } if(qty_yr>= 2){
                struct near_segment *new_root = (struct near_segment *)malloc(sizeof(struct near_segment));
                new_root->value = right_y;
                new_root->median = find_median(*right_y, qty_yr, true);
                new_root->qty = qty_yr;
                new_root->left = new_root->right = NULL;
                root->right = new_root;
            }
            generate_tree(root->left, qty_yl, true);
            generate_tree(root->right, qty_yr, true);
        }
    }
    return root;
};

float find_median(struct segment* seg, int n, bool x){
    float *a = malloc(sizeof(float)*n);
    float* array = sort(seg,a, n, x);
    if (n%2 == 0){
        float middle = array[n/2];
        free(a);
        return middle;
    } else{
        int pos_middle = (int)(n/2);
        float middle = (array[pos_middle]+array[pos_middle+1])/2;
        free(a);
        return middle;
    }
}

float* sort(struct segment* seg,float* a, int n, bool x){
    float t;
    for (int j = 0 ; j <= n-1 ; j++) {
        if(x){
            a[j] = (seg[j].pi.x + seg[j].pf.x)/2;
        } else{
            a[j] = (seg[j].pi.y + seg[j].pf.y)/2;
        }
    }
    for (int i = 0 ; i <= n-1 ; i++){ /* Trip-i begins */
        for (int j = 0 ; j <= n-i-2 ; j++) {
            if (a[j] >= a[j+1]) { /* Interchanging values */
                t = a[j];
                a[j] = a[j+1];
                a[j+1] = t;
            }
            else
            continue ;
        }
    }
    return a;
}

Near_Segment* find_path(struct aabb* box, struct near_segment* root, Particle particle, bool x){
    if(!x){
        if(root->right != NULL){
            struct aabb* new_box = malloc(sizeof(BoundingBox));
            double min_y = (double)sort(*root->right->value, malloc(sizeof(float)*root->right->qty), root->right->qty, false)[0];
            double max_y = (double)sort(*root->right->value, malloc(sizeof(float)*root->right->qty), root->right->qty, false)[root->right->qty - 1];
            new_box->min_point.y = min_y;
            new_box->max_point.y = max_y;
            new_box->min_point.x = box->min_point.x;
            new_box->max_point.x = box->max_point.x;
            if (particle_boundingbox_collision(particle, *new_box)){
                return find_path(new_box, root->right, particle, false);
            } else if (root->left != NULL){
                struct aabb* new_box = malloc(sizeof(BoundingBox));
                double min_y = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, false)[0];
                double max_y = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, false)[root->left->qty - 1];
                new_box->min_point.y = min_y;
                new_box->max_point.y = max_y;
                new_box->min_point.x = box->min_point.x;
                new_box->max_point.x = box->max_point.x;
                if(particle_boundingbox_collision(particle, *new_box)){
                    return find_path(new_box, root->left, particle, false);
                }
            } else {
                free(box);
                return root;
            }
        } else if(root->left != NULL){
            struct aabb* new_box = malloc(sizeof(BoundingBox));
            double min_y = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, false)[0];
            double max_y = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, false)[root->left->qty - 1];
            new_box->min_point.y = min_y;
            new_box->max_point.y = max_y;
            new_box->min_point.x = box->min_point.x;
            new_box->max_point.x = box->max_point.x;
            if(particle_boundingbox_collision(particle, *new_box)){
                return find_path(new_box, root->left, particle, false);
            }
        } else {
            free(box);
            return root;
        }
    }else{
        if(root->right != NULL){
            struct aabb* new_box = malloc(sizeof(BoundingBox));
            double min_x = (double)sort(*root->right->value, malloc(sizeof(float)*root->right->qty), root->right->qty, true)[0];
            double max_x = (double)sort(*root->right->value, malloc(sizeof(float)*root->right->qty), root->right->qty, true)[root->right->qty - 1];
            new_box->min_point.x = min_x;
            new_box->max_point.x = max_x;
            new_box->min_point.y = box->min_point.y;
            new_box->max_point.y = box->max_point.y;
            if (particle_boundingbox_collision(particle, *new_box)){
                return find_path(new_box, root->right, particle, true);
            } else if (root->left != NULL){
                struct aabb* new_box = malloc(sizeof(BoundingBox));
                double min_x = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, true)[0];
                double max_x = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, true)[root->left->qty - 1];
                new_box->min_point.x = min_x;
                new_box->max_point.x = max_x;
                new_box->min_point.y = box->min_point.y;
                new_box->max_point.y = box->max_point.y;
                if(particle_boundingbox_collision(particle, *new_box)){
                    return find_path(new_box, root->left, particle, true);
                }
            } else {
                free(box);
                return root;
            }
        } else if(root->left != NULL){
                struct aabb* new_box = malloc(sizeof(BoundingBox));
                double min_x = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, true)[0];
                double max_x = (double)sort(*root->left->value, malloc(sizeof(float)*root->left->qty), root->left->qty, true)[root->left->qty - 1];
                new_box->min_point.x = min_x;
                new_box->max_point.x = max_x;
                new_box->min_point.y = box->min_point.y;
                new_box->max_point.y = box->max_point.y;
            if(particle_boundingbox_collision(particle, *new_box)){
                return find_path(new_box, root->left, particle, true);
            }
        } else {
            free(box);
            return root;
        }
    }
    free(box);
    return NULL;
}

void segment_destroy(struct near_segment* root){
    if(root != NULL && (root->left != NULL || root->right != NULL)){
        if(root->left != NULL){
            segment_destroy(root->left);
        } if(root->right != NULL){
            segment_destroy(root->right);
        }
    } else if(root != NULL) {
        free(root->value);
        free(root);
    }
}