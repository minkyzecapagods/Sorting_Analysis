#include "utils.hpp"

int main() {
    run_tests("resultados/", RANDOM_ORDER);
    run_tests("resultados/", INVERSE_ORDER);
    run_tests("resultados/", ALMOST_SORTED);
    return 0;
}

