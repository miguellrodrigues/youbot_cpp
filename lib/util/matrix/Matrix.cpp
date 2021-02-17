//
// Created by miguel on 30/01/2021.
//

#include "Matrix.hpp"
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
                this->data[i][j] = randomDouble(-.0001, .0001);
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

void Matrix::map(double (*function)(double)) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double value = this->data[i][j];

            setValue(i, j, function(value));
        }
    }
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

void Matrix::hadamard(Matrix &mx) const {
    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < this->cols; ++j) {
            setValue(i, j, getValue(i, j) * mx.getValue(i, j));
        }
    }
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

Matrix *Matrix::multiply(Matrix &mx) const {
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

Matrix *Matrix::arrayToMatrix(double *array, unsigned int size) {
    auto *matrix = new Matrix(size, 1, false);

    for (int i = 0; i < size; ++i) {
        matrix->setValue(i, 0, array[i]);
    }

    return matrix;
}

double *Matrix::matrixToArray() const {
    auto *arr = static_cast<double *>(malloc(this->rows * sizeof(double)));

    for (int i = 0; i < this->rows; ++i) {
        arr[i] = this->data[i][0];
    }

    return arr;
}

string Matrix::arrayToString() const {
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

void Matrix::printToConsole() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << getValue(i, j) << " \n"[j == cols - 1];
        }
    }
}
