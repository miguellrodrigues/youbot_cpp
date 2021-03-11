//
// Created by miguel on 11/03/2021.
//

#include "Matmul.cuh"

#define BLOCK_SIZE 16

__global__ void matrixMultiplyKernel(const double *a, const double *b, double *c, int m, int n, int k) {
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    double aux = .0;

    if (row < m && col < k) {
        for (int i = 0; i < n; i++) {
            aux += a[row * n + i] * b[i * k + col];
        }

        c[row * k + col] = aux;
    }
}

void Matmul::matrixMultiply(double *a, double *b, double *c, int m, int n, int k) {
    dim3 dimGrid((k + BLOCK_SIZE - 1) / BLOCK_SIZE, (m + BLOCK_SIZE - 1) / BLOCK_SIZE);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

/*    if (n*n > 512) {
        threadsPerBlock.x = 512;
        threadsPerBlock.y = 512;
        blocksPerGrid.x = ceil(double(n)/double(threadsPerBlock.x));
        blocksPerGrid.y = ceil(double(n)/double(threadsPerBlock.y));
    }*/

    matrixMultiplyKernel<<<dimGrid, dimBlock>>>(a, b, c, m, n, k);
}
