/**
 * @file adaptive_hybrid_sort.c
 * @brief An implementation of an Adaptive Hybrid Sort algorithm in C.
 *
 * This program demonstrates a "meta" sorting algorithm that first analyzes
 * the characteristics of an input array and then selects the most appropriate
 * sorting algorithm from a pool of candidates (Quicksort, Merge Sort, Radix Sort)
 * to achieve optimal performance.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// =============================================================================
// 1. CONSTANTS AND STRATEGY DEFINITIONS
// =============================================================================

#define INSERTION_SORT_THRESHOLD 32
#define ANALYSIS_SAMPLE_SIZE 100
#define NEARLY_SORTED_THRESHOLD 0.85 // 85% or more elements are in ascending order
#define LOW_CARDINALITY_THRESHOLD 0.20 // 20% or fewer unique elements

// Enum to define the sorting strategy chosen by the analysis engine.
typedef enum {
    STRATEGY_MERGESORT,  // Best for nearly sorted data (Timsort stand-in)
    STRATEGY_RADIXSORT,  // Best for non-negative integers
    STRATEGY_QUICKSORT   // Robust default, good for low cardinality
} SortStrategy;


// =============================================================================
// 2. FORWARD DECLARATIONS OF ALL FUNCTIONS
// =============================================================================

// Main adaptive sort function
void adaptiveHybridSort(int arr[], int n);

// Analysis function
SortStrategy analyzeData(const int arr[], int n);

// Core sorting algorithms
void insertionSort(int arr[], int left, int right);
void quickSort(int arr[], int low, int high);
void mergeSort(int arr[], int left, int right);
void radixSort(int arr[], int n);

// Utility functions
void printArray(const char* label, const int arr[], int n);
void swap(int* a, int* b);
int compareInts(const void* a, const void* b); // For qsort in analysis


// =============================================================================
// 3. THE MAIN ADAPTIVE HYBRID SORT FUNCTION
// =============================================================================

/**
 * @brief Sorts an array using the best strategy based on data analysis.
 * @param arr The integer array to sort.
 * @param n The number of elements in the array.
 */
void adaptiveHybridSort(int arr[], int n) {
    if (n <= 1) {
        return; // Already sorted
    }

    // For very small arrays, Insertion Sort is fastest.
    if (n < INSERTION_SORT_THRESHOLD) {
        printf(" -> Strategy: Insertion Sort (small array)\n");
        insertionSort(arr, 0, n - 1);
        return;
    }

    // Step 1: Analyze the data to determine the best strategy.
    SortStrategy strategy = analyzeData(arr, n);

    // Step 2: Execute the chosen sorting algorithm.
    switch (strategy) {
        case STRATEGY_MERGESORT:
            printf(" -> Strategy: Merge Sort (for nearly sorted data)\n");
            mergeSort(arr, 0, n - 1);
            break;
        case STRATEGY_RADIXSORT:
            printf(" -> Strategy: Radix Sort (for non-negative integers)\n");
            radixSort(arr, n);
            break;
        case STRATEGY_QUICKSORT:
        default:
            printf(" -> Strategy: Quicksort (robust default)\n");
            quickSort(arr, 0, n - 1);
            break;
    }
}


// =============================================================================
// 4. HEURISTIC ANALYSIS ENGINE
// =============================================================================

/**
 * @brief Analyzes a sample of the array to choose a sorting strategy.
 * @param arr The array to analyze.
 * @param n The size of the array.
 * @return The recommended SortStrategy.
 */
SortStrategy analyzeData(const int arr[], int n) {
    int sample_size = (n < ANALYSIS_SAMPLE_SIZE) ? n : ANALYSIS_SAMPLE_SIZE;
    bool has_negative = false;

    // --- Heuristic 1: Check for nearly sorted data & negative numbers ---
    int ascending_pairs = 0;
    for (int i = 0; i < sample_size - 1; ++i) {
        if (arr[i] < 0) has_negative = true;
        if (arr[i] <= arr[i + 1]) {
            ascending_pairs++;
        }
    }
    if ((double)ascending_pairs / (sample_size - 1) >= NEARLY_SORTED_THRESHOLD) {
        return STRATEGY_MERGESORT; // Merge sort is efficient for nearly sorted data.
    }

    // --- Heuristic 2: If no negatives, Radix Sort is a strong candidate ---
    if (!has_negative) {
        return STRATEGY_RADIXSORT;
    }

    // --- Heuristic 3: Check for low cardinality (many duplicates) ---
    // To do this, we sort a copy of the sample and count unique elements.
    int* sample_copy = (int*)malloc(sample_size * sizeof(int));
    if (!sample_copy) return STRATEGY_QUICKSORT; // Failsafe
    
    memcpy(sample_copy, arr, sample_size * sizeof(int));
    qsort(sample_copy, sample_size, sizeof(int), compareInts);

    int unique_count = 1;
    for (int i = 1; i < sample_size; ++i) {
        if (sample_copy[i] != sample_copy[i - 1]) {
            unique_count++;
        }
    }
    free(sample_copy);

    if ((double)unique_count / sample_size <= LOW_CARDINALITY_THRESHOLD) {
        return STRATEGY_QUICKSORT; // 3-Way Quicksort would be ideal, but standard is also good.
    }

    // --- Default Case ---
    return STRATEGY_QUICKSORT;
}


// =============================================================================
// 5. IMPLEMENTATIONS OF CORE SORTING ALGORITHMS
// =============================================================================

// --- Insertion Sort ---
void insertionSort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// --- Quicksort ---
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortRecursive(int arr[], int low, int high) {
    if (low < high) {
        // Switch to Insertion Sort for small subarrays
        if (high - low + 1 < INSERTION_SORT_THRESHOLD) {
            insertionSort(arr, low, high);
        } else {
            int pi = partition(arr, low, high);
            quickSortRecursive(arr, low, pi - 1);
            quickSortRecursive(arr, pi + 1, high);
        }
    }
}

void quickSort(int arr[], int low, int high) {
    quickSortRecursive(arr, low, high);
}


// --- Merge Sort ---
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));
    if (!L || !R) {
        if (L) free(L);
        if (R) free(R);
        return; // Memory allocation failed
    }

    for (i = 0; i < n1; i++) L[i] = arr[l + i];
    for (j = 0; j < n2; j++) R[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];

    free(L);
    free(R);
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// --- Radix Sort ---
int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

void countingSortForRadix(int arr[], int n, int exp) {
    int* output = (int*)malloc(n * sizeof(int));
    int count[10] = {0};
    if (!output) return; // Failsafe

    for (int i = 0; i < n; i++) count[(arr[i] / exp) % 10]++;
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    for (int i = 0; i < n; i++) arr[i] = output[i];
    
    free(output);
}

void radixSort(int arr[], int n) {
    int m = getMax(arr, n);
    for (int exp = 1; m / exp > 0; exp *= 10) {
        countingSortForRadix(arr, n, exp);
    }
}


// =============================================================================
// 6. UTILITY AND HELPER FUNCTIONS
// =============================================================================

void printArray(const char* label, const int arr[], int n) {
    printf("%s: [", label);
    for (int i = 0; i < n; ++i) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]\n");
}

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int compareInts(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}


// =============================================================================
// 7. DEMONSTRATION IN MAIN
// =============================================================================

int main() {
    srand(time(NULL));

    printf("--- Adaptive Hybrid Sort Demonstration ---\n\n");

    // Case 1: Nearly sorted data
    int nearly_sorted[] = {1, 2, 3, 10, 5, 6, 7, 8, 9, 4, 11, 12};
    int n1 = sizeof(nearly_sorted) / sizeof(nearly_sorted[0]);
    printArray("Case 1 (Nearly Sorted) - Before", nearly_sorted, n1);
    adaptiveHybridSort(nearly_sorted, n1);
    printArray("Case 1 (Nearly Sorted) - After ", nearly_sorted, n1);
    printf("\n--------------------------------------------\n\n");

    // Case 2: Non-negative integers (ideal for Radix Sort)
    int positive_ints[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n2 = sizeof(positive_ints) / sizeof(positive_ints[0]);
    printArray("Case 2 (Positive Integers) - Before", positive_ints, n2);
    adaptiveHybridSort(positive_ints, n2);
    printArray("Case 2 (Positive Integers) - After ", positive_ints, n2);
    printf("\n--------------------------------------------\n\n");

    // Case 3: Random data with negatives (default to Quicksort)
    int random_data[] = {9, -3, 5, 2, 6, 8, -6, 1, 3, 4, 15, 0, -10};
    int n3 = sizeof(random_data) / sizeof(random_data[0]);
    printArray("Case 3 (Random w/ Negatives) - Before", random_data, n3);
    adaptiveHybridSort(random_data, n3);
    printArray("Case 3 (Random w/ Negatives) - After ", random_data, n3);
    printf("\n--------------------------------------------\n\n");

    // Case 4: Small array (will use Insertion Sort)
    int small_array[] = {5, 1, 4, 2, 8};
    int n4 = sizeof(small_array) / sizeof(small_array[0]);
    printArray("Case 4 (Small Array) - Before", small_array, n4);
    adaptiveHybridSort(small_array, n4);
    printArray("Case 4 (Small Array) - After ", small_array, n4);
    printf("\n");

    return 0;
}
