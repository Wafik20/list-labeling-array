#ifndef LLA_H
#define LLA_H

#include <stdio.h>
#include <stdlib.h>

// ################# MACROS ###################
#define null NULL
#define true 0
#define false 1

// ################# STRUCTS ###################
typedef struct lla_node {
    int window_start;
    int window_end;
    double tau;
    double TAU_K;
    int size;
    struct lla_node *left;
    struct lla_node *right;
    struct lla_node *parent;
    int is_leaf;
} lla_node;

typedef struct lla {
    lla_node *root;
    int *arr;
    int N;
    int C;
    double TAU_0;
    double TAU_D;
    int MAX_DEPTH;
    int WINDOW_SIZE;
} lla;

// ################# FUNCTION DECLARATIONS ###################

// Helpers
void print_array(int *arr, int size);
void zero_array(int *arr, int size);
void print_lla(lla *my_lla);
int log_base_2(int n);

// Tree setup
lla_node *create_lla_node(lla_node *parent, int start, int end);
void init_balancing_tree(lla_node *node, int depth, int MAX_DEPTH, int WINDOW_SIZE, double TAU_0, double TAU_D);
lla *create_lla(int N, int C, double TAU_0, double TAU_D);

// Insertions
void insert_and_distribute_array_range(int *arr, int start_index, int end_index, int x);
void insert(lla *lla, int x);

// Cleanup
void free_lla_tree(lla_node *node);
void free_lla(lla *my_lla);
void cleanup_lla(lla **my_lla);

#endif
