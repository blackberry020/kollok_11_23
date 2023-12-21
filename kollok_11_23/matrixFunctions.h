#pragma once

#include "dataStructures.h"

template<class T>
T** createMatrix(int n, int m) {
    T** matrix = new T * [n];

    for (int i = 0; i < n; i++) {
        matrix[i] = new T[m];
    }

    return matrix;
}

template<class T>
void readMatrix(T** a, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            std::cin >> a[i][j];
    }

    return;
}

template<class T>
T** transposeMatrix(T** a, int n, int m) {

    T** result = createMatrix<T>(m, n);

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            result[i][j] = a[j][i];
        }
    }

    return result;
}
