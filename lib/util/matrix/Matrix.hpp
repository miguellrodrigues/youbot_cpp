//
// Created by miguel on 30/01/2021.
//

#ifndef YOUBOT_CPP_MATRIX_HPP
#define YOUBOT_CPP_MATRIX_HPP


#include <string>
#include <vector>

class Matrix {

public:
    static double randomDouble(double min, double max);

    Matrix();

    Matrix(unsigned int rows, unsigned int cols, bool isRandom);

    void setValue(unsigned int row, unsigned int col, double value) const;

    double getValue(unsigned int row, unsigned int col) const;

    Matrix *copy() const;

    Matrix *transpose() const;

    Matrix *hadamard(Matrix &mx) const;

    Matrix *multiply(Matrix &mx) const;

    void add(Matrix &mx) const;

    void subtract(Matrix &mx) const;

    void scalar(double x) const;

    void split(double x) const;

    void map(double (*function)(double)) const;

    double *matrixToArray() const;

    static Matrix *arrayToMatrix(double *array, unsigned int size);
    static Matrix *vectorToMatrix(std::vector<double> vector);

    static Matrix *multiply(Matrix &mx, Matrix &my);
    static Matrix *hadamard(Matrix &mx, Matrix &my);

    void assign_matrix_array(double *array) const;

    void printToConsole();

    std::string matrixToString() const;

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
};


#endif //REDENEURAL_MATRIX_HPP
