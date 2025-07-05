#include "utils.hpp"
#include "sorts.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <string>
#include <iomanip>
#include <numeric>
#include <map>

using namespace std;

// ========================== WRAPPERS ==========================
// Funções wrapper para cada algoritmo, com a mesma assinatura esperada pelo framework
void selection_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    selection_sort(v, v.size(), comparisons, movements);
}

void insertion_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    insertion_sort(v, v.size(), comparisons, movements);
}

void bubble_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    bubble_sort(v, v.size(), comparisons, movements);
}

void merge_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    merge_sort(v, 0, v.size() - 1, comparisons, movements);
}

void quick_begin_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    quick_sort(v, 0, v.size() - 1, PIVOT_BEGIN, comparisons, movements);
}

void quick_end_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    quick_sort(v, 0, v.size() - 1, PIVOT_END, comparisons, movements);
}

void quick_middle_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    quick_sort(v, 0, v.size() - 1, PIVOT_MIDDLE, comparisons, movements);
}

void quick_median_of_three_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    quick_sort(v, 0, v.size() - 1, PIVOT_MEDIAN_OF_THREE, comparisons, movements);
}

void quick_random_wrapper(vector<int>& v, long long& comparisons, long long& movements) {
    quick_sort(v, 0, v.size() - 1, PIVOT_RANDOM, comparisons, movements);
}

// ========================== GERADORES DE VETORES ==========================
// Gera um vetor com números aleatórios uniformemente distribuídos
vector<int> generate_random_vector(int size) {
    vector<int> v(size);
    random_device rd;                           // Seed aleatória de hardware
    mt19937 gen(rd());                           // Gerador Mersenne Twister
    uniform_int_distribution<> dis(1, size * 10); // Distribuição no intervalo [1, 10*n]

    for (int i = 0; i < size; i++) {
        v[i] = dis(gen);                         // Preenche o vetor com números aleatórios
    }
    return v;
}

// Gera um vetor ordenado de forma decrescente (pior caso para alguns algoritmos)
vector<int> generate_reverse_sorted_vector(int size) {
    vector<int> v(size);
    for (int i = 0; i < size; i++) {
        v[i] = size - i;                         // Ex: [n, n-1, ..., 1]
    }
    return v;
}

// Gera um vetor 90% ordenado e 10% permutado aleatoriamente
vector<int> generate_90_percent_sorted_vector(int size) {
    vector<int> v(size);
    iota(v.begin(), v.end(), 1);                 // Preenche com sequência crescente [1, 2, ..., n]
    int num_to_randomize = size / 10;            // 10% do tamanho para permutar

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, size - 1);

    for (int i = 0; i < num_to_randomize; i++) {
        int idx1 = dist(gen);
        int idx2 = dist(gen);
        swap(v[idx1], v[idx2]);                   // Permuta dois elementos aleatórios
    }
    return v;
}

// Converte o tipo de vetor para uma string (para nomes de arquivos)
string vector_type_to_string(VectorType type) {
    switch (type) {
        case RANDOM_ORDER: return "random";
        case INVERSE_ORDER: return "inverse";
        case ALMOST_SORTED: return "almost_sorted";
        default: return "unknown";
    }
}

// ========================== EXPORTAÇÃO DE RESULTADOS ==========================
// Exporta os resultados agrupados por tipo de vetor em arquivos CSV separados
void export_results_per_vector_type(
    const vector<AlgorithmResult>& results,      // Lista de resultados de algoritmos
    const string& base_filename,                 // Caminho base para arquivos
    VectorType vector_type                       // Tipo de vetor testado
) {
    string vector_suffix = vector_type_to_string(vector_type);
    // Ordem fixa dos algoritmos no arquivo de saída
    const vector<string> algorithm_order = {
        "BubbleSort",
        "InsertionSort",
        "MergeSort",
        "QuickSort_Begin",
        "QuickSort_End",
        "QuickSort_MedianOfThree",
        "QuickSort_Middle",
        "QuickSort_Random",
        "SelectionSort"
    };

    // Agrupa resultados por tamanho e algoritmo usando um mapa aninhado
    map<int, map<string, AlgorithmResult>> grouped_data;
    for (const auto& res : results) {
        grouped_data[res.size][res.name] = res;
    }

    // Função interna para gerar arquivos para cada métrica
    auto generate_metric_file = [&](const string& metric_name, auto metric_extractor) {
        string filename = base_filename + metric_name + "_" + vector_suffix + ".csv";
        ofstream file(filename);                  // Abre arquivo para escrita
        
        // Cabeçalho CSV
        file << "Size";
        for (const auto& algo : algorithm_order) {
            file << "," << algo;
        }
        file << "\n";

        // Escreve dados linha a linha
        for (const auto& [size, algo_map] : grouped_data) {
            file << size;
            for (const auto& algo_name : algorithm_order) {
                if (algo_map.find(algo_name) != algo_map.end()) {
                    file << "," << metric_extractor(algo_map.at(algo_name));
                } else {
                    file << ",";
                }
            }
            file << "\n";
        }
        file.close();
        cout << "Arquivo gerado: " << filename << endl;
    };

    // Gera arquivos CSV para cada métrica
    generate_metric_file("tempos",      [](const AlgorithmResult& r) { return r.time_us; });
    generate_metric_file("trocas",      [](const AlgorithmResult& r) { return r.movements; });
    generate_metric_file("comparacoes", [](const AlgorithmResult& r) { return r.comparisons; });
}

// ========================== EXECUÇÃO DOS TESTES ==========================
// Executa todos os algoritmos sobre os vetores de diferentes tamanhos e tipos
void run_tests(const string& base_output_path, VectorType vector_type) {
    srand(time(0)); // Inicializa o gerador rand com seed atual
    vector<int> sizes = {100, 1000, 5000, 10000, 50000}; // Tamanhos dos vetores para os testes
    
    // Lista dos algoritmos com seus nomes e wrappers correspondentes
    vector<pair<string, void (*)(vector<int>&, long long&, long long&)>> algorithms = {
        {"BubbleSort", bubble_wrapper},
        {"InsertionSort", insertion_wrapper},
        {"MergeSort", merge_wrapper},
        {"QuickSort_Begin", quick_begin_wrapper},
        {"QuickSort_End", quick_end_wrapper},
        {"QuickSort_MedianOfThree", quick_median_of_three_wrapper},
        {"QuickSort_Middle", quick_middle_wrapper},
        {"QuickSort_Random", quick_random_wrapper},
        {"SelectionSort", selection_wrapper}
    };

    vector<AlgorithmResult> all_results; // Armazena todos os resultados

    // Para cada tamanho de vetor
    for (int size : sizes) {
        vector<int> original;
        // Gera o vetor conforme o tipo especificado
        if (vector_type == RANDOM_ORDER) original = generate_random_vector(size);
        else if (vector_type == INVERSE_ORDER) original = generate_reverse_sorted_vector(size);
        else original = generate_90_percent_sorted_vector(size);

        // Executa cada algoritmo sobre uma cópia do vetor
        for (auto& [algo_name, algo_func] : algorithms) {
            vector<int> copy = original; // Faz cópia para manter o original intacto
            long long comparisons = 0;
            long long movements = 0;

            // Mede o tempo de execução do algoritmo
            auto start = chrono::high_resolution_clock::now();
            algo_func(copy, comparisons, movements);
            auto end = chrono::high_resolution_clock::now();

            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

            // Armazena o resultado no vetor de resultados
            all_results.push_back({
                algo_name,
                size,
                duration.count(), // Tempo em microssegundos
                comparisons,
                movements
            });

            // Imprime o resultado no console
            cout << "[" << vector_type_to_string(vector_type) << " - " << algo_name
                 << " n=" << size << "] Tempo: " << duration.count() << " μs, "
                 << "Comparações: " << comparisons << ", "
                 << "Movimentos: " << movements << "\n";
        }
    }

    // Exporta os resultados para arquivos CSV
    export_results_per_vector_type(all_results, base_output_path, vector_type);
}
