#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <random>
#include <chrono>
#include <ctime>
#include <zconf.h>

using namespace std;

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {

        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

void mergeSortParallel(int arr[], int l, int r) {
    int sumk = 8;

    int diff = (r - l + 1) / sumk;
    int divide[sumk + 1];
    divide[0] = l - 1;
    for (int i = 1; i < sumk; i++) {
        divide[i] = l + diff * i;
    }
    divide[sumk] = r;


    omp_set_num_threads(sumk);
#pragma omp parallel
    {
        int num = omp_get_thread_num();
        mergeSort(arr, divide[num] + 1, divide[num + 1]);
    }

    omp_set_num_threads(4);
#pragma omp parallel
    {
        int num = omp_get_thread_num();
        merge(arr, divide[num * 2] + 1, divide[num * 2 + 1], divide[num * 2 + 2]);
    }
    omp_set_num_threads(2);
#pragma omp parallel
    {
        int num = omp_get_thread_num();
        merge(arr, divide[num * 4] + 1, divide[num * 4 + 2], divide[num * 4 + 4]);
    }
    merge(arr, divide[0] + 1, divide[4], divide[8]);

}

void printArray(int A[], int size) {
    int i;
    for (i = 0; i < size; i++)
        cout << A[i] << " ";
    cout << endl;
}

int main() {
    int arr_size = 25000;
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<> dist(0, arr_size);
    int arr[arr_size];
    int arr2[arr_size];
    int x;
    for (int i = 0; i < arr_size; i++) {
        x = dist(rng);
        arr[i] = x;
        arr2[i] = x;
    }

    cout << "Given array is" << endl;
    printArray(arr, arr_size);

    auto start = chrono::system_clock::now();
    mergeSort(arr, 0, arr_size - 1);

    auto end = chrono::system_clock::now();

    cout << "\n-------------------------------------------------------------------------------------------------------------- \n";
    cout << "\nSorted array by mergeSort is: \n";
    printArray(arr, arr_size);

    chrono::duration<double> elapsed_seconds = end - start;
    cout << "\n-------------------------------------------------------------------------------------------------------------- \n";
    cout << "Elapsed time when using mergeSort: " << elapsed_seconds.count() << "s\n";

    start = chrono::system_clock::now();
    mergeSortParallel(arr2, 0, arr_size - 1);

    end = chrono::system_clock::now();
    cout << "\n-------------------------------------------------------------------------------------------------------------- \n";
    cout << "\nSorted array by mergeSortParallel: \n";
    printArray(arr2, arr_size);

    elapsed_seconds = end - start;
    cout << "\n-------------------------------------------------------------------------------------------------------------- \n";
    cout << "Elapsed time when using mergeSortParallel: " << elapsed_seconds.count() << "s\n";

    return 0;
}