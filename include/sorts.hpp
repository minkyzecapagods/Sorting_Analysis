#ifndef SORTS_HPP
#define SORTS_HPP

#include <vector>

enum PivotType {
    PIVOT_BEGIN,
    PIVOT_END,
    PIVOT_MIDDLE,
    PIVOT_MEDIAN_OF_THREE,
    PIVOT_RANDOM
};

void selection_sort(std::vector<int>& v, int n, long long& num_comparisons, long long& num_movements);
void insertion_sort(std::vector<int>& v, int n, long long& num_comparisons, long long& num_movements);
void bubble_sort(std::vector<int>& v, int n, long long& num_comparisons, long long& num_movements);
void merge_sort(std::vector<int>& v, int left, int right, long long& num_comparisons, long long& num_movements);
void quick_sort(std::vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements);

void merge(std::vector<int>& v, int left, int mid, int right, long long& num_comparisons, long long& num_movements);
int partition(std::vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements);
void choose_pivot(std::vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements);

#endif // SORTS_HPP