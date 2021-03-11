//
// Created by miguel on 30/01/2021.
//

#include "Matrix.hpp"
#include "../../neural_network/cuda/matmul/MatUtil.cuh"
#include "../../neural_network/cuda/dev_array.h"
#include <cstdlib>
#include <string>
#include <random>
#include <iostream>

using namespace std;

double Matrix::randomDouble(double min, double max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(min, max);

    return (dis(gen));
}

Matrix::Matrix() = default;

Matrix::Matrix(unsigned int rows, unsigned int cols, bool isRandom) : rows(rows), cols(cols) {
    this->data = static_cast<double **>(malloc(rows * sizeof(double)));

    for (int i = 0; i < rows; ++i) {
        this->data[i] = static_cast<double *>(malloc(cols * sizeof(double)));
    }

    if (isRandom) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                this->data[i][j] = randomDouble(-.701, .701);
            }
        }
    } else {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                this->data[i][j] = 0.0;
            }
        }
    }
}

void Matrix::setValue(unsigned int row, unsigned int col, double value) const {
    this->data[row][col] = value;
}

double Matrix::getValue(unsigned int row, unsigned int col) const {
    return this->data[row][col];
}

Matrix *Matrix::copy() const {
    auto *matrix = new Matrix(this->rows, this->cols, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(i, j, this->data[i][j]);
        }
    }

    return matrix;
}

Matrix *Matrix::transpose() const {
    auto *matrix = new Matrix(this->cols, this->rows, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(j, i, getValue(i, j));
        }
    }

    return matrix;
}

Matrix *Matrix::hadamard(Matrix &mx) const {
    auto *matrix = new Matrix(this->rows, this->cols, false);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            matrix->setValue(i, j, getValue(i, j) * mx.getValue(i, j));
        }
    }

    return matrix;
}

Matrix *Matrix::multiply(Matrix &mx) const {
    if (this->cols != mx.rows) {
        cout << "error: matmul bad arguments" << endl;
        return nullptr;
    }

    auto *matrix = new Matrix(this->rows, mx.cols, false);

    unsigned int size_a = this->rows * this->cols,
                 size_b = mx.rows * mx.cols,
                 size_c = this->rows * mx.cols;

    double *a = this->vectorize();
    double *b = mx.vectorize();

    double *h_c;
    cudaMallocHost((void **) &h_c, sizeof (double) * size_c);

    dev_array<double> d_A(size_a);
    dev_array<double> d_B(size_b);
    dev_array<double> d_C(size_c);

    d_A.set(a, size_a);
    d_B.set(b, size_b);

    MatUtil::matrixMultiply(d_A.getData(), d_B.getData(), d_C.getData(), this->rows, this->cols, mx.cols);

    cudaDeviceSynchronize();

    d_C.get(h_c, size_c);

    cudaDeviceSynchronize();

    for (unsigned int row = 0, count = 0; row < matrix->rows; row++) {
        for (unsigned int col = 0; col < matrix->cols; ++col) {
            matrix->setValue(row, col, h_c[count++]);
        }
    }

    cudaFree(d_A.getData());
    cudaFree(d_B.getData());
    cudaFree(d_C.getData());

    cudaFreeHost(h_c);

    return matrix;
}

void Matrix::add(Matrix &mx) const {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            setValue(i, j, getValue(i, j) + mx.getValue(i, j));
        }
    }
}

void Matrix::subtract(Matrix &mx) const {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            setValue(i, j, getValue(i, j) - mx.getValue(i, j));
        }
    }
}

void Matrix::scalar(double x) const {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->data[i][j] *= x;
        }
    }
}

void Matrix::split(double x) const {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            this->data[i][j] /= x;
        }
    }
}

void Matrix::map(double (*function)(double)) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value = this->data[i][j];

            setValue(i, j, function(value));
        }
    }
}

double *Matrix::matrixToArray() const {
    auto *arr = static_cast<double *>(malloc(this->rows * sizeof(double)));

    for (int i = 0; i < this->rows; ++i) {
        arr[i] = this->data[i][0];
    }

    return arr;
}

Matrix *Matrix::arrayToMatrix(double *array, unsigned int size) {
    auto *matrix = new Matrix(size, 1, false);

    for (int i = 0; i < size; ++i) {
        matrix->setValue(i, 0, array[i]);
    }

    return matrix;
}

Matrix *Matrix::vectorToMatrix(vector<double> vector) {
    auto *matrix = new Matrix(vector.size(), 1, false);

    for (int i = 0; i < vector.size(); ++i) {
        matrix->setValue(i, 0, vector.at(i));
    }

    return matrix;
}

Matrix *Matrix::hadamard(Matrix &mx, Matrix &my) {
    auto *matrix = new Matrix(mx.rows, mx.cols, false);

    for (int i = 0; i < mx.rows; ++i) {
        for (int j = 0; j < mx.cols; ++j) {
            matrix->setValue(i, j, mx.getValue(i, j) * my.getValue(i, j));
        }
    }

    return matrix;
}

Matrix *Matrix::multiply(Matrix &mx, Matrix &my) {
    /*if (mx.cols != my.rows) {
        cout << "bad arguments" << endl;
        return nullptr;
    }

    auto *proof = new Matrix(mx.rows, my.cols, false);

    double aux;

    for (int i = 0; i < mx.rows; ++i) {
        for (int j = 0; j < my.cols; ++j) {
            aux = 0.0;

            for (int l = 0; l < my.rows; ++l) {
                aux += mx.getValue(i, l) * my.getValue(l, j);
            }

            proof->setValue(i, j, aux);
        }
    }*/
    return mx.multiply(my);
}

void Matrix::printToConsole() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << getValue(i, j) << " \n"[j == cols - 1];
        }
    }
}

string Matrix::matrixToString() const {
    string dataStr;

    for (unsigned int i = 0; i < this->rows; ++i) {
        for (unsigned int j = 0; j < this->cols; ++j) {
            dataStr.append(to_string(this->data[i][j]));
            dataStr.append(":");
            dataStr.append(to_string(i));
            dataStr.append(":");
            dataStr.append(to_string(j));
            dataStr.append("\n");
        }
    }

    return dataStr;
}

Matrix *Matrix::operator+(Matrix &mx) const {
    Matrix *c = copy();

    c->add(mx);

    return c;
}

Matrix *Matrix::operator-(Matrix &mx) const {
    Matrix *c = copy();

    c->subtract(mx);

    return c;
}

Matrix *Matrix::operator*(Matrix &mx) const {
    Matrix *c = copy();

    c->multiply(mx);

    return c;
}

Matrix *Matrix::operator*(double x) const {
    Matrix *c = copy();

    c->scalar(x);

    return c;
}

Matrix *Matrix::operator/(double x) const {
    Matrix *c = copy();

    c->split(x);

    return c;
}

void Matrix::operator+=(Matrix &mx) const {
    add(mx);
}

void Matrix::operator-=(Matrix &mx) const {
    subtract(mx);
}

void Matrix::operator*=(Matrix &mx) const {
    multiply(mx);
}

void Matrix::operator*=(double x) const {
    scalar(x);
}

void Matrix::operator/=(double x) const {
    split(x);
}

void Matrix::assign_matrix_array(double *array) const {
    unsigned int aux = 0;

    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            setValue(i, j, array[aux++]);
        }
    }
}

double *Matrix::vectorize() const {
    unsigned int n = rows * cols;

    auto *d = static_cast<double *>(malloc(sizeof(double) * n));

    for (unsigned int i = 0, count = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            d[count++] = getValue(i, j);
        }
    }

    return d;
}
