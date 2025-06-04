#include <stdio.h>
#include <stdlib.h>

// Constants
#define C 6
#define TAU_0 0.5
#define TAU_d 0.5
#define null NULL
#define N 16
#define WINDOW_SIZE 4

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
// ################# EOF HELPER FUNCTIONS ##############

// ################# BEGIN STRUCTS #################
typedef struct lla_node lla_node;
typedef struct lla lla;

struct lla_node
{
    int window_start;
    int window_end;
    int size;
    lla_node *left;
    lla_node *right;
};

struct lla
{
    lla_node *root;
    int *arr;
};
// ################# EOF STRUCTS #################

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

    return node;
}

void init_balancing_tree(lla_node *node)
{
    if (!node)
        return;

    int parent_window_start = node->window_start;
    int parent_window_end = node->window_end;
    int mid_point = (parent_window_start + parent_window_end) / 2;

    printf("%d, %d, %d\n", parent_window_end, parent_window_start, mid_point);
    if(parent_window_end == 0 || mid_point == 0){
        return;
    }

    // Base case: if window size reached, stop recursion
    if (parent_window_end - parent_window_start + 1 < WINDOW_SIZE)
    {
        return;
    }

    node->left = create_lla_node(parent_window_start, mid_point);
    node->right = create_lla_node(mid_point + 1, parent_window_end);

    init_balancing_tree(node->left);
    init_balancing_tree(node->right);
}

lla *create_lla()
{
    lla *my_lla = (lla *)malloc(sizeof(lla));

    my_lla->arr = (int *)malloc(sizeof(int) * N * C);

    // Init array first
    zero_array(my_lla->arr, N * C);

    // Create balancing tree
    lla_node *root = create_lla_node(0, (N * C) - 1);
    my_lla->root = root;

    // Init the balancing tree on the array
    init_balancing_tree(root);

    return my_lla;
}

// void print_tree_helper(lla_node* node){
//     if (node == null) {
//         return;
//     }

//     printf("Node: window_start = %d, window_end = %d, size = %d\n",
//            node->window_start, node->window_end, node->size);

//     print_tree_helper(node->left);
//     print_tree_helper(node->right);
// }

// void print_lla(lla* my_lla){
//     lla_node* root = my_lla->root;
//     if (root == null) {
//         printf("The LLA is empty.\n");
//         return;
//     }

//     print_tree_helper(root);

//     return;
// }

int main()
{
    lla *my_lla = create_lla();
    // print_lla(my_lla);
    return 0;
}