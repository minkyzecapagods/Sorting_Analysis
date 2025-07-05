#include "sorts.hpp"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

// ========================= SELECTION SORT =========================
void selection_sort(vector<int>& v, int n, long long& num_comparisons, long long& num_movements) {
    // Para cada posição do vetor, procura o menor elemento no restante e troca
    for (int i = 0; i < n - 1; i++) {
        int min = i; // Índice do menor elemento encontrado
        for (int j = i + 1; j < n; j++) {
            num_comparisons++; // Conta a comparação
            if (v[j] < v[min]) {
                min = j; // Atualiza o índice do menor elemento
            }
        }
        // Se o menor não for o próprio elemento da posição, troca
        if (min != i) {
            swap(v[i], v[min]);
            num_movements += 3; // Cada swap = 3 movimentações
        }
    }
}

// ========================= INSERTION SORT =========================
void insertion_sort(vector<int>& v, int n, long long& num_comparisons, long long& num_movements) {
    // Insere cada elemento na posição correta de um subvetor crescente
    for (int i = 1; i < n; i++) {
        int insert = v[i]; // Elemento a ser inserido
        num_movements++; // Atribuição para variável temporária
        int j = i - 1;
        while (j >= 0) {
            num_comparisons++; // Conta a comparação
            if (v[j] > insert) {
                v[j + 1] = v[j]; // Desloca elemento para direita
                num_movements++; // Atribuição
                j--;
            } else {
                break; // Parar se a posição correta for encontrada
            }
        }
        v[j + 1] = insert; // Insere o elemento na posição correta
        num_movements++; // Atribuição final
    }
}

// ========================= BUBBLE SORT =========================
void bubble_sort(vector<int>& v, int n, long long& num_comparisons, long long& num_movements) {
    bool swapped;
    // Passa pelo vetor repetidamente, trocando elementos adjacentes fora de ordem
    for (int i = 0; i < n - 1; i++) {
        swapped = false; // Marca se houve troca nesta passagem
        for (int j = 0; j < n - i - 1; j++) {
            num_comparisons++; // Conta a comparação
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]); // Troca os elementos adjacentes
                num_movements += 3;  // Cada swap = 3 movimentações
                swapped = true;
            }
        }
        if (!swapped) break; // Para se o vetor já estiver ordenado
    }
}

// ========================= MERGE SORT AUXILIAR =========================
void merge(vector<int>& v, int left, int mid, int right, long long& num_comparisons, long long& num_movements) {
    // Cria vetores auxiliares para as duas metades
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    // Copia os elementos das duas metades para os vetores auxiliares
    for (int i = 0; i < n1; i++) {
        L[i] = v[left + i];
        num_movements++; // Conta a cópia
    }
    for (int j = 0; j < n2; j++) {
        R[j] = v[mid + 1 + j];
        num_movements++; // Conta a cópia
    }

    // Mescla os dois vetores auxiliares de volta em v[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        num_comparisons++; // Conta a comparação
        if (L[i] <= R[j]) {
            v[k] = L[i];
            num_movements++; // Atribuição
            i++;
        } else {
            v[k] = R[j];
            num_movements++; // Atribuição
            j++;
        }
        k++;
    }

    // Copia os elementos restantes de L (se houver)
    while (i < n1) {
        v[k] = L[i];
        num_movements++;
        i++;
        k++;
    }

    // Copia os elementos restantes de R (se houver)
    while (j < n2) {
        v[k] = R[j];
        num_movements++;
        j++;
        k++;
    }
}

// ========================= MERGE SORT =========================
void merge_sort(vector<int>& v, int left, int right, long long& num_comparisons, long long& num_movements) {
    // Divide o vetor recursivamente até ter subvetores de tamanho 1
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    merge_sort(v, left, mid, num_comparisons, num_movements);      // Ordena metade esquerda
    merge_sort(v, mid + 1, right, num_comparisons, num_movements); // Ordena metade direita
    merge(v, left, mid, right, num_comparisons, num_movements);    // Mescla as duas metades
}

// ========================= QUICK SORT AUXILIAR =========================
void choose_pivot(vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements) {
    if (low == high) return; // Caso base: apenas um elemento

    int pivotIndex = low; // Inicializa o índice do pivô

    switch (pivotType) {
        case PIVOT_BEGIN:
            pivotIndex = low; // Pivô no início
            break;
        case PIVOT_END:
            pivotIndex = high; // Pivô no fim
            break;
        case PIVOT_MIDDLE:
            pivotIndex = low + (high - low) / 2; // Pivô no meio
            break;
        case PIVOT_MEDIAN_OF_THREE: {
            // Pivô como a mediana entre três elementos (início, meio, fim)
            int mid = low + (high - low) / 2;
            int a = v[low], b = v[mid], c = v[high];
            num_comparisons += 3; // Três comparações para encontrar a mediana
            if ((a < b && b < c) || (c < b && b < a))
                pivotIndex = mid;
            else if ((b < a && a < c) || (c < a && a < b))
                pivotIndex = low;
            else
                pivotIndex = high;
            break;
        }
        case PIVOT_RANDOM:
            pivotIndex = low + rand() % (high - low + 1); // Pivô aleatório
            break;
    }
    swap(v[pivotIndex], v[high]); // Coloca o pivô na última posição
    num_movements += 3;           // Cada swap = 3 movimentações
}

// ========================= PARTITION =========================
int partition(vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements) {
    // Escolhe o pivô e organiza elementos menores à esquerda e maiores à direita
    choose_pivot(v, low, high, pivotType, num_comparisons, num_movements);
    int pivot = v[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        num_comparisons++; // Conta a comparação
        if (v[j] < pivot) {
            i++;
            swap(v[i], v[j]); // Troca elemento menor para a partição da esquerda
            num_movements += 3;
        }
    }
    swap(v[i + 1], v[high]); // Coloca o pivô na posição correta
    num_movements += 3;
    return i + 1; // Retorna o índice do pivô
}

// ========================= QUICK SORT =========================
void quick_sort(vector<int>& v, int low, int high, PivotType pivotType, long long& num_comparisons, long long& num_movements) {
    if (low < high) {
        // Particiona o vetor e ordena recursivamente as duas metades
        int pi = partition(v, low, high, pivotType, num_comparisons, num_movements);
        quick_sort(v, low, pi - 1, pivotType, num_comparisons, num_movements);  // Subvetor esquerdo
        quick_sort(v, pi + 1, high, pivotType, num_comparisons, num_movements); // Subvetor direito
    }
}
