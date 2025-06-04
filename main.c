#include "lla.h"

int main() {
    int N = 16;
    int C = 8;
    double TAU_0 = 0.5;
    double TAU_D = 0.75;

    lla *my_lla = create_lla(N, C, TAU_0, TAU_D);
    insert(my_lla, 10);
    insert(my_lla, 1);
    insert(my_lla, 5);
    insert(my_lla, 2);
    insert(my_lla, 20);
    print_lla(my_lla);
    cleanup_lla(&my_lla);

    return 0;
}
