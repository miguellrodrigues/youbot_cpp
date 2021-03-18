//
// Created by miguel on 30/01/2021.
//

#include "Matrix.hpp"
#include "../Numbers.hpp"
#include <string>
#include <iostream>

using namespace std;

Matrix::Matrix() = default;

Matrix::Matrix(unsigned int rows, unsigned int cols, bool isRandom) : rows(rows), cols(cols) {
    for (unsigned int i = 0; i < rows; ++i) {
        vector<double> values;

        for (unsigned int j = 0; j < cols; ++j) {
            if (isRandom) {
                values.push_back(Numbers::randomDouble(-1.0, 1.0));
            } else {
                values.push_back(.0);
            }
        }

        this->data.push_back(values);
    }
}

void Matrix::setValue(unsigned int row, unsigned int col, double value) {
    this->data.at(row).at(col) = value;
}

double Matrix::getValue(unsigned int row, unsigned int col) {
    return this->data.at(row).at(col);
}

Matrix *Matrix::copy() {
    auto *matrix = new Matrix(this->rows, this->cols, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(i, j, this->data[i][j]);
        }
    }

    return matrix;
}

Matrix *Matrix::transpose() {
    auto *matrix = new Matrix(this->cols, this->rows, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(j, i, getValue(i, j));
        }
    }

    return matrix;
}

Matrix *Matrix::hadamard(Matrix &mx) {
    auto *matrix = new Matrix(this->rows, this->cols, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(i, j, getValue(i, j) * mx.getValue(i, j));
        }
    }

    return matrix;
}

Matrix *Matrix::multiply(Matrix &mx) {
    auto *matrix = new Matrix(this->rows, mx.cols, false);

    double aux;

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < mx.cols; ++j) {
            aux = 0.0;

            for (int l = 0; l < mx.rows; ++l) {
                aux += getValue(i, l) * mx.getValue(l, j);
            }

            matrix->setValue(i, j, aux);
        }
    }

    return matrix;

    /*auto *matrix = new Matrix(this->rows, mx.cols, false);

    if (this->cols != mx.rows) {
        cout << "error: matmul bad arguments" << endl;
        return nullptr;
    }

    unsigned int size_a = this->rows * this->cols,
            size_b = mx.rows * mx.cols,
            size_c = this->rows * mx.cols;

    double *a = this->vectorize();
    double *b = mx.vectorize();

    double *h_c;
    cudaMallocHost((void **) &h_c, sizeof (double) * size_c);

    double *d_a, *d_b, *d_c;

    cudaMalloc((void **) &d_a, sizeof (double) * size_a);
    cudaMalloc((void **) &d_b, sizeof (double) * size_b);
    cudaMalloc((void **) &d_c, sizeof (double) * size_c);

    cudaMemcpy(d_a, a, sizeof (double) * size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, sizeof (double) * size_b, cudaMemcpyHostToDevice);

    MatUtil::matrixMultiply(d_a, d_b, d_c, this->rows, this->cols, mx.cols);

    cudaMemcpy(h_c, d_c, sizeof (double) * size_c, cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();

    for (unsigned int row = 0, count = 0; row < matrix->rows; row++) {
        for (unsigned int col = 0; col < matrix->cols; ++col) {
            matrix->setValue(row, col, h_c[count++]);
        }
    }

    cudaFreeHost(h_c);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    return matrix;*/
}

void Matrix::add(Matrix &mx) {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            setValue(i, j, getValue(i, j) + mx.getValue(i, j));
        }
    }
}

void Matrix::subtract(Matrix &mx) {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            setValue(i, j, getValue(i, j) - mx.getValue(i, j));
        }
    }
}

void Matrix::scalar(double x) {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->data[i][j] *= x;
        }
    }
}

void Matrix::split(double x) {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->data[i][j] /= x;
        }
    }
}

vector<double> Matrix::to_vector() {
    vector<double> d;

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            d.push_back(getValue(i, j));
        }
    }

    return d;
}

Matrix *Matrix::vectorToMatrix(vector<double> vector) {
    auto *matrix = new Matrix(vector.size(), 1, false);

    for (int i = 0; i < vector.size(); ++i) {
        matrix->setValue(i, 0, vector.at(i));
    }

    return matrix;
}


void Matrix::assign_matrix_array(double *array) {
    unsigned int aux = 0;

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            setValue(i, j, array[aux++]);
        }
    }
}

unsigned int Matrix::size() const {
    return this->rows * this->cols;
}

void Matrix::printToConsole() {
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->cols; j++) {
            cout << this->data.at(i).at(j) << "\t\t";
        }
        cout << endl;
    }
}


Matrix::~Matrix() {
    this->data.clear();
}