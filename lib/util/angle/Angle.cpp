//
// Created by miguel on 17/02/2021.
//

#include "Angle.hpp"
#include <cmath>
#include <iostream>

using namespace std;

Angle::Angle() {
    double phi = -(M_PI / 2.0);

    double data[9] = {
            1, 0, 0,
            0, cos(phi), -sin(phi),
            0, sin(phi), cos(phi)
    };

    rotation_x->assign_matrix_array(reinterpret_cast<double *>(data));

    rotation_x = rotation_x->transpose();
}

Matrix *Angle::calculateMatrix(double *data) {
    auto *R = new Matrix(3, 3, false);

    double cosTheta = cos(data[3]);
    double sinTheta = sin(data[3]);

    double mx[9] = {
            (pow(data[0], 2) * (1 - cosTheta) + cosTheta), ((data[0] * data[1]) * (1 - cosTheta) - (data[2] * sinTheta)), ((data[0] * data[2]) * (1 - cosTheta) + (data[1] * sinTheta)),
            ((data[0] * data[1]) * (1 - cosTheta) + (data[2] * sinTheta)), (pow(data[1], 2) * (1 - cosTheta) + cosTheta), ((data[1] * data[2]) * (1 - cosTheta) - (data[0] * sinTheta)),
            ((data[0] * data[2]) * (1 - cosTheta) - (data[1] * sinTheta)), ((data[1] * data[2]) * (1 - cosTheta) + (data[0] * sinTheta)), (pow(data[2], 2) * (1 - cosTheta) + cosTheta)
    };

    R->assign_matrix_array(mx);

    return R;
}

double Angle::calculateAngle(double *data) const {
    auto x = Matrix::arrayToMatrix(data, 9);

    auto result = rotation_x->multiply(*x);

    double angle = atan2(result->getValue(1, 0), result->getValue(0, 0));

    delete x;
    delete result;

    return angle;
}
