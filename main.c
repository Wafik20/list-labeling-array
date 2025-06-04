#include <stdio.h>
#include <stdlib.h>

// ################# BEGIN MACROS ###################
#define null NULL
// ################# EOF STRUCTS ###################

// ################# BEGIN STRUCTS ###################
typedef struct lla_node lla_node;
typedef struct lla lla;

struct lla_node
{
    int window_start;
    int window_end;
    double TAU_K;
    int size;
    lla_node *left;
    lla_node *right;
};

struct lla
{
    lla_node *root;
    int *arr;
    int N;
    int C;
    double TAU_0;
    double TAU_d;
};
// ################# EOF STRUCTS ###################

// ################# HELPER FUNCTIONS ##############
void print_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
}

void zero_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

void print_tree_helper(lla_node* node, int depth){
    if (!node) {
        return;
    }

    printf("size: %d, depth: %d, tau_k: %.4f, win_start: %d, win_end: %d\n", node->size, depth, node->TAU_K, node->window_start, node->window_end);

    print_tree_helper(node->left, depth + 1);
    print_tree_helper(node->right, depth + 1);
}

void print_lla(lla* my_lla){
    lla_node* root = my_lla->root;
    if (root == null) {
        printf("The LLA is empty.\n");
        return;
    }

    print_tree_helper(root, 0);

    return;
}

int log_base_2(int n) {
    int result = 0;
    while (n >>= 1) {
        result++;
    }
    return result;
}
// ################# EOF HELPER FUNCTIONS ##############

// ################# BEGIN MAIN FUNCTIONS ###################
lla_node *create_lla_node(int start, int end)
{
    lla_node *node = (lla_node *)malloc(sizeof(lla_node));
    if (!node)
    {
        printf("Malloc failed\n");
        exit(1);
    }

    node->window_start = start;
    node->window_end = end;
    node->left = NULL;
    node->right = NULL;
    node->size = 0;
    node->TAU_K = 0;

    return node;
}

void init_balancing_tree(lla_node *node, int depth, int MAX_DEPTH, int WINDOW_SIZE, double TAU_0, double TAU_D)
{
    if (!node)
        return;

    int parent_window_start = node->window_start;
    int parent_window_end = node->window_end;
    int mid_point = (parent_window_start + parent_window_end) / 2;

    //printf("%d, %d, %d\n", parent_window_end, parent_window_start, mid_point, depth);
    //printf("WINDOW_SIZE = %d, TAU_K = %.2f, depth = %d, MAX_DEPTH = %d, start = %d, end = %d\n", WINDOW_SIZE, node->TAU_K, depth, MAX_DEPTH, parent_window_start, parent_window_end);


    // Base case: if window size reached, stop recursion
    //if (parent_window_end - parent_window_start + 1 <= WINDOW_SIZE)
    if(depth == MAX_DEPTH)
    {
        // Set tau_k = tau_d
        node->TAU_K = TAU_D;
        //printf("tau_k: %.2f\n", TAU_D);
        return;
    }

    double TAU_K = TAU_0 + ((TAU_D - TAU_0) * ((double)depth / MAX_DEPTH));
    //printf("tau_k = %f + ((%f - %f) * (%d / %d))\n", TAU_0, TAU_D, TAU_0, depth, MAX_DEPTH);
    node->TAU_K = TAU_K;
    //printf("tau_k: %.2f\n", TAU_K);

    node->left = create_lla_node(parent_window_start, mid_point);
    node->right = create_lla_node(mid_point + 1, parent_window_end);

    init_balancing_tree(node->left, depth + 1, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_D);
    init_balancing_tree(node->right, depth + 1, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_D);
}

lla *create_lla(int N, int C, double TAU_0, double TAU_d)
{
    lla *my_lla = (lla *)malloc(sizeof(lla));

    my_lla->arr = (int *)malloc(sizeof(int) * N * C);

    // Init array first
    zero_array(my_lla->arr, N * C);

    // Create balancing tree
    lla_node *root = create_lla_node(0, (N * C) - 1);
    my_lla->root = root;

    // Init the balancing tree on the array
    int WINDOW_SIZE = log_base_2(N);
    int num_leaves = (C * N) / WINDOW_SIZE;
    int MAX_DEPTH = log_base_2(num_leaves);
    //printf("window size: %d\n", window_size);
    init_balancing_tree(root, 0, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_d);

    return my_lla;
}
// ################# EOF MAIN FUNCTIONS ###################

// ################# BEGIN MAIN ###################
int main()
{
    int N = 4;
    int C = 2;
    double TAU_0 = 0.5;
    double TAU_D = 0.75;

    lla *my_lla = create_lla(N, C, TAU_0, TAU_D);
    print_lla(my_lla);
    return 0;
}
// ################# EOF MAIN ###################