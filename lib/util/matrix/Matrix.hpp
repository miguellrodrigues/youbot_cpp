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
    static double randomDouble(double min, double max);

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

    void map(double (*function)(double));

    double *matrixToArray() ;

    double *vectorize() ;

    static Matrix *arrayToMatrix(double *array, unsigned int size);
    static Matrix *vectorToMatrix(std::vector<double> vector);

    static Matrix *multiply(Matrix &mx, Matrix &my);
    static Matrix *hadamard(Matrix &mx, Matrix &my);

    void assign_matrix_array(double *array);

    void printToConsole() ;

    std::string matrixToString() ;

    Matrix *operator+(Matrix &mx) ;

    Matrix *operator-(Matrix &mx) ;

    Matrix *operator*(Matrix &mx) ;

    Matrix *operator*(double x) ;

    Matrix *operator/(double x) ;

    void operator+=(Matrix &mx);

    void operator-=(Matrix &mx);

    void operator*=(Matrix &mx);

    void operator*=(double x);

    void operator/=(double x);

    //double **data{};

    vector<vector<double>> data;

    unsigned int rows{};
    unsigned int cols{};

    unsigned int size() ;

    static int randomInt(int min, int max);

    vector<double> to_vector();
};


#endif //REDENEURAL_MATRIX_HPP
