#include <stdio.h>
#include <stdlib.h>
#include "lla.h"

// ################# HELPER FUNCTIONS ##############
void print_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}

void zero_array(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        arr[i] = 0;
    }
}

void print_tree_helper(lla_node *node, int depth)
{
    if (!node)
    {
        return;
    }

    printf("is_leaf: %d, size: %d, depth: %d, tau: %.4f, tau_k: %.4f, win_start: %d, win_end: %d\n", node->is_leaf, node->size, depth, node->tau, node->TAU_K, node->window_start, node->window_end);
    if (node->parent)
    {
        // printf("parent: s: %d, e: %d\n", node->parent->window_start, node->parent->window_end);
    }

    print_tree_helper(node->left, depth + 1);
    print_tree_helper(node->right, depth + 1);
}

void print_lla(lla *my_lla)
{
    lla_node *root = my_lla->root;
    if (root == null)
    {
        printf("The LLA is empty.\n");
        return;
    }
    printf("Array snapshot:\n");
    int arr_size = my_lla->N * my_lla->C;
    print_array(my_lla->arr, arr_size);

    print_tree_helper(root, 0);

    return;
}

int log_base_2(int n)
{
    int result = 0;
    while (n >>= 1)
    {
        result++;
    }
    return result;
}
// ################# EOF HELPER FUNCTIONS ##############

// ################# BEGIN MAIN FUNCTIONS ###################
lla_node *create_lla_node(lla_node *parent, int start, int end)
{
    lla_node *node = (lla_node *)malloc(sizeof(lla_node));
    if (!node)
    {
        printf("Malloc failed\n");
        exit(1);
    }

    node->window_start = start;
    node->window_end = end;
    node->left = NULL;     // to be init later in init_balancing_tree()
    node->right = NULL;    // to be init later in init_balancing_tree()
    node->size = 0;        // set as zero before any insertions happen
    node->tau = 0;         // set as zero before any insertions happen
    node->TAU_K = 0;       // to be init later in init_balancing_tree()
    node->is_leaf = false; // initially set as false, and set later true only for leafs in init_balancing_tree()
    node->parent = parent;

    return node;
}

void init_balancing_tree(lla_node *node, int depth, int MAX_DEPTH, int WINDOW_SIZE, double TAU_0, double TAU_D)
{
    if (!node)
        return;

    int parent_window_start = node->window_start;
    int parent_window_end = node->window_end;
    int mid_point = (parent_window_start + parent_window_end) / 2;

    // printf("%d, %d, %d\n", parent_window_end, parent_window_start, mid_point, depth);
    // printf("WINDOW_SIZE = %d, TAU_K = %.2f, depth = %d, MAX_DEPTH = %d, start = %d, end = %d\n", WINDOW_SIZE, node->TAU_K, depth, MAX_DEPTH, parent_window_start, parent_window_end);

    // Base case: if window size reached, stop recursion
    // if (parent_window_end - parent_window_start + 1 <= WINDOW_SIZE)
    if (depth == MAX_DEPTH)
    {
        // Set tau_k = tau_d
        node->TAU_K = TAU_D;
        node->is_leaf = true;
        // printf("tau_k: %.2f\n", TAU_D);
        return;
    }

    double TAU_K = TAU_0 + ((TAU_D - TAU_0) * ((double)depth / MAX_DEPTH));
    // printf("tau_k = %f + ((%f - %f) * (%d / %d))\n", TAU_0, TAU_D, TAU_0, depth, MAX_DEPTH);
    node->TAU_K = TAU_K;
    // printf("tau_k: %.2f\n", TAU_K);

    node->left = create_lla_node(node, parent_window_start, mid_point);
    node->right = create_lla_node(node, mid_point + 1, parent_window_end);

    init_balancing_tree(node->left, depth + 1, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_D);
    init_balancing_tree(node->right, depth + 1, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_D);
}

lla *create_lla(int N, int C, double TAU_0, double TAU_D)
{
    lla *my_lla = (lla *)malloc(sizeof(lla));
    if (!my_lla)
    {
        printf("Malloc failed\n");
        exit(1);
    }
    
    my_lla->arr = (int *)malloc(sizeof(int) * N * C);
    if(!my_lla->arr){
        printf("Malloc failed\n");
        exit(1);
    }

    my_lla->N = N;
    my_lla->C = C;
    my_lla->TAU_0 = TAU_0;
    my_lla->TAU_D = TAU_D;

    // Init array first
    zero_array(my_lla->arr, N * C);

    // Create balancing tree
    lla_node *root = create_lla_node(null, 0, (N * C) - 1);
    my_lla->root = root;

    // Init the balancing tree on the array
    int WINDOW_SIZE = log_base_2(N);
    int num_leaves = (C * N) / WINDOW_SIZE;
    int MAX_DEPTH = log_base_2(num_leaves);

    my_lla->WINDOW_SIZE = WINDOW_SIZE;
    my_lla->MAX_DEPTH = MAX_DEPTH;
    // printf("window size: %d\n", window_size);
    init_balancing_tree(root, 0, MAX_DEPTH, WINDOW_SIZE, TAU_0, TAU_D);

    return my_lla;
}

// Should return first ansestor in threshhold or a leaf indicating that insertion is legal.
lla_node *insert_help(lla_node *node, int *arr, int depth, int MAX_DEPTH, int x)
{
    if (!node)
    {
        return null;
    }

    int window_start = node->window_start;
    int window_end = node->window_end;
    int partition_size = window_end - window_start + 1;
    int element_at_window_end = arr[window_end];
    int new_size = node->size + 1;
    double new_tau = ((double)new_size / (partition_size));

    if (new_tau > node->TAU_K)
    {
        //printf("exceeded threshhold, return nearest anscestor in threshhold\n");
        return node->parent; // should be node->parent
    }

    // Set new size and density after insertion
    node->size = new_size;
    node->tau = new_tau;

    if (depth == MAX_DEPTH)
    {
        return node;
    }

    if (x > element_at_window_end)
    { /* traverse right */
        return insert_help(node->right, arr, depth + 1, MAX_DEPTH, x);
    }
    else
    { /* traverse left */
        return insert_help(node->left, arr, depth + 1, MAX_DEPTH, x);
    }
}

// a very long sorted array, and an element
// You get the whole array, the start index where to start spreading out, the end index where to stop spreading out, the element to be inserted.
// input arr=[......, 1, 5, 0, 0, 0, ......], start_index, end_index, x = 2
// insert the element between start_index, end_index and include them, and then do the spreading out
// output: [......, 1, 0, 2, 0, 5, .......] // sorted and spread out
//
void insert_and_distribute_array_range(int *arr, int start_index, int end_index, int x)
{

    int range_size = end_index - start_index + 1;
    int non_zero_count = 0;
    int *temp = (int *)malloc(range_size * sizeof(int));
    if(!temp){
        printf("Malloc failed\n");
        exit(1);
    }
    int temp_idx = 0;
    int x_inserted = 0;

    for (int i = start_index; i <= end_index; i++)
    {
        if (arr[i] != 0)
        {
            if (!x_inserted && x < arr[i])
            {
                temp[temp_idx++] = x;
                x_inserted = 1;
                non_zero_count++;
            }
            temp[temp_idx++] = arr[i];
            non_zero_count++;
        }
    }

    if (!x_inserted)
    {
        temp[temp_idx++] = x;
        non_zero_count++;
    }

    for (int i = start_index; i <= end_index; i++)
    {
        arr[i] = 0;
    }

    if (non_zero_count > 0)
    {
        int spacing = range_size / non_zero_count;
        int remainder = range_size % non_zero_count;
        int pos = 0;

        for (int i = 0; i < non_zero_count; i++)
        {
            arr[start_index + pos] = temp[i];

            int step = spacing;
            if (i < remainder)
            {
                step++;
            }
            pos += step;

            if (pos >= range_size && i < non_zero_count - 1)
            {
                pos = range_size - (non_zero_count - 1 - i);
            }
        }
    }

    free(temp);
}

void insert(lla *lla, int x)
{
    lla_node *root = lla->root;
    int *arr = lla->arr;

    if (root->tau >= lla->TAU_0)
    { /* Don't insert when the array's density exceeds TAU_0 */
        printf("insert failed: Root exceeded treshhold density. Increase N!");
        exit(1);
    }

    lla_node *node = insert_help(root, arr, 0, lla->MAX_DEPTH, x); // either a leaf, or nearest ancestor in threshhold

    if (!node)
    {
        printf("insert failed");
        exit(1);
    }

    if (node->is_leaf)
    { // we succeeded, got a leaf
        insert_and_distribute_array_range(arr, node->window_start, node->window_end, x);
        //printf("insert %d, and redistribute range [%d, %d]\n", x, node->window_start, node->window_end);
    }
    else
    { // we need to rebalance, got first ansestor in threshhold
        lla_node *parent = node->parent;

        if (!parent)
        {
            printf("insert failed");
            exit(1);
        }

        insert_and_distribute_array_range(arr, parent->window_start, parent->window_end, x);
        //printf("insert %d, and redistribute range [%d, %d]\n", x, parent->window_start, parent->window_end);
    }
    return;
}
// ################# EOF MAIN FUNCTIONS ###################

// ################# BEGIN CLEANUP FUNCTIONS ###################
void free_lla_tree(lla_node *node)
{
    if (!node)
        return;

    free_lla_tree(node->left);
    free_lla_tree(node->right);

    free(node);
}

void free_lla(lla *my_lla)
{
    if (!my_lla)
        return;

    free_lla_tree(my_lla->root);

    if (my_lla->arr)
        free(my_lla->arr);

    free(my_lla);
}

void cleanup_lla(lla **my_lla)
{
    if (my_lla && *my_lla)
    {
        free_lla(*my_lla);
        *my_lla = NULL;
    }
}
// ################# EOF CLEANUP FUNCTIONS ###################
