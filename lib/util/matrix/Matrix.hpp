//
// Created by miguel on 30/01/2021.
//

#ifndef YOUBOT_CPP_MATRIX_HPP
#define YOUBOT_CPP_MATRIX_HPP

#include <string>
#include <vector>

using std::vector;

class Matrix {

public:
    Matrix();

    ~Matrix();

    Matrix(unsigned int rows, unsigned int cols, bool isRandom);

    void setValue(unsigned int row, unsigned int col, double value);

    double getValue(unsigned int row, unsigned int col);

    Matrix *copy() ;

    Matrix *transpose() ;

    Matrix *hadamard(Matrix &mx) ;

    Matrix *multiply(Matrix &mx) ;

    void add(Matrix &mx);

    void subtract(Matrix &mx);

    void scalar(double x);

    void split(double x);

    vector<double> to_vector();

    static Matrix *vectorToMatrix(std::vector<double> vector);

    void assign_matrix_array(double *array);

    unsigned int size() const;

    const vector<vector<double>> &getData()
    {
        return this->data;
    }

    unsigned int getRows() const
    {
        return this->rows;
    }

    unsigned int getCols() const
    {
        return this->cols;
    }

    void printToConsole() ;

private:
    vector<vector<double>> data;

    unsigned int rows{};
    unsigned int cols{};
};

#endif //REDENEURAL_MATRIX_HPP
