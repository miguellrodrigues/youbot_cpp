//
// Created by miguel on 30/01/2021.
//

#ifndef REDENEURAL_MATRIX_HPP
#define REDENEURAL_MATRIX_HPP


#include <string>

class Matrix {

public:
    Matrix(unsigned int rows, unsigned int cols, bool isRandom);

    Matrix();

    Matrix *copy() const;

    void map(double (*function)(double)) const;

    void hadamard(Matrix &mx) const;

    void add(Matrix &mx) const;

    void subtract(Matrix &mx) const;

    void scalar(double x) const;

    void split(double x) const;

    void setValue(unsigned int row, unsigned int col, double value) const;

    double getValue(unsigned int row, unsigned int col) const;

    double *matrixToArray() const;

    static Matrix *arrayToMatrix(double *array, unsigned int size);

    static double randomDouble(double min, double max);

    Matrix *transpose() const;

    Matrix *multiply(Matrix &mx) const;

    void printToConsole();

    std::string arrayToString() const;

    Matrix *operator+(Matrix &mx) const;

    Matrix *operator-(Matrix &mx) const;

    Matrix *operator*(Matrix &mx) const;

    Matrix *operator*(double x) const;

    Matrix *operator/(double x) const;

    void operator+=(Matrix &mx) const;

    void operator-=(Matrix &mx) const;

    void operator*=(Matrix &mx) const;

    void operator*=(double x) const;

    void operator/=(double x) const;

    double **data{};

    unsigned int rows{};
    unsigned int cols{};


private:
};


#endif //REDENEURAL_MATRIX_HPP
