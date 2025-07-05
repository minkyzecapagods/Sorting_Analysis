#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>

enum VectorType {
    RANDOM_ORDER,
    INVERSE_ORDER,
    ALMOST_SORTED
};

struct AlgorithmResult {
    std::string name;
    int size;
    long long time_us;
    long long comparisons;
    long long movements;
};

std::vector<int> generate_random_vector(int size);
std::vector<int> generate_reverse_sorted_vector(int size);
std::vector<int> generate_90_percent_sorted_vector(int size);
void run_tests(const std::string& base_output_path, VectorType vector_type);
std::string vector_type_to_string(VectorType type);

#endif // UTILS_HPP