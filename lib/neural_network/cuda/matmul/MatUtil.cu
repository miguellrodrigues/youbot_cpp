//
// Created by miguel on 11/03/2021.
//

#include "MatUtil.cuh"
#include <stdio.h>

#define BLOCK_SIZE 16

__global__ void matrixMultiplyKernel(const double *a, const double *b, double *c, int m, int n, int k) {
    unsigned int row = blockIdx.y * blockDim.y + threadIdx.y;
    unsigned int col = blockIdx.x * blockDim.x + threadIdx.x;

    if (row < m && col < k) {
        double aux = .0;

        for (int i = 0; i < n; i++) {
            aux += a[row * n + i] * b[i * k + col];
        }

        c[row * k + col] = aux;
    }
}

__global__ void matrixTransposeKernel(double *a, double *b, unsigned int rows, unsigned int cols) {
    unsigned int idx = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned int idy = blockIdx.y * blockDim.y + threadIdx.y;

    if (idx < cols && idy < rows) {
        unsigned int pos = idy * cols + idx;
        unsigned int transposedPos = idx * rows + idy;

        b[transposedPos] = a[pos];
    }
}

void MatUtil::matrixMultiply(double *a, double *b, double *c, int m, int n, int k) {
    dim3 dimGrid((k + BLOCK_SIZE - 1) / BLOCK_SIZE, (m + BLOCK_SIZE - 1) / BLOCK_SIZE);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);

    matrixMultiplyKernel<<<dimGrid, dimBlock>>>(a, b, c, m, n, k);
}

void MatUtil::matrixTranspose(double *a, double *b, unsigned int rows, unsigned int cols) {
    unsigned int n = rows * cols;

    dim3 dim_grid((n - 1) / BLOCK_SIZE + 1, (n - 1) / BLOCK_SIZE + 1, 1);
    dim3 dim_block(BLOCK_SIZE, BLOCK_SIZE, 1);

    matrixTransposeKernel<<<dim_grid, dim_block>>>(a, b, rows, cols);
}
