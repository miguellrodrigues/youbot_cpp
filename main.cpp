#include "include/Train.hpp"
#include "include/Test.hpp"

#include "lib/neural_network/cuda/dev_array.h"
#include "lib/neural_network/cuda/matmul/MatUtil.cuh"
#include <vector>
#include <iostream>

using namespace std;

int main() {

    //new Train({1, 8, 16, 16, 8, 3}, 5, 1000, 30);
    //new Test();

    int m = 2, n = 3, k = 2;

    int size = 6;

    // Allocate memory on the host
    vector<double> h_A(size);
    vector<double> h_B(size);
    vector<double> h_C(size);

    unsigned int data_a[6] = {1, 2, 2, 2, 1, 3};
    unsigned int data_b[6] = {4, 4, 2, 5, 5, 3};

    unsigned int cont = 0;

    for (unsigned int i = 0; i < m; ++i) {
        for (unsigned int j = 0; j < n; ++j) {
            h_A[i * n + j] = data_a[cont++];
        }
    }

    cont = 0;

    for (unsigned int i = 0; i < n; ++i) {
        for (unsigned int j = 0; j < k; ++j) {
            h_B[i * k + j] = data_b[cont++];
        }
    }

    dev_array<double> d_A(size);
    dev_array<double> d_B(size);
    dev_array<double> d_C(size);

    d_A.set(&h_A[0], size);
    d_B.set(&h_B[0], size);

    MatUtil::matrixMultiply(d_A.getData(), d_B.getData(), d_C.getData(), 2, 3, 2);

    cudaDeviceSynchronize();

    d_C.get(&h_C[0], size);

    for (int ROW=0; ROW < m; ROW++){
        for (int COL=0; COL < k; COL++){
            //err += cpu_C[ROW * N + COL] - h_C[ROW * N + COL];

            cout << h_C[ROW * m + COL] << endl;
        }
    }


    return 0;
}


