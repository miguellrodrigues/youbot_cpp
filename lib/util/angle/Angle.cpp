//
// Created by miguel on 17/02/2021.
//

#include "Angle.hpp"
#include <cmath>
#include <iostream>

using namespace std;

Angle::Angle(Matrix *rotation_x) {
    this->rotation_x = rotation_x->transpose();
}

Matrix *Angle::calculateMatrix(double *d) {
    auto *R = new Matrix(3, 3, false);

    double cosTheta = cos(d[3]);
    double sinTheta = sin(d[3]);

    double mx[9] = {
            (pow(d[0], 2) * (1 - cosTheta) + cosTheta), ((d[0] * d[1]) * (1 - cosTheta) - (d[2] * sinTheta)), ((d[0] * d[2]) * (1 - cosTheta) + (d[1] * sinTheta)),
            ((d[0] * d[1]) * (1 - cosTheta) + (d[2] * sinTheta)), (pow(d[1], 2) * (1 - cosTheta) + cosTheta), ((d[1] * d[2]) * (1 - cosTheta) - (d[0] * sinTheta)),
            ((d[0] * d[2]) * (1 - cosTheta) - (d[1] * sinTheta)), ((d[1] * d[2]) * (1 - cosTheta) + (d[0] * sinTheta)), (pow(d[2], 2) * (1 - cosTheta) + cosTheta)
    };

    R->assign_matrix_array(mx);

    return R;
}

double Angle::calculateAngle(double *d) {
    auto x = calculateMatrix(d);

    auto result = this->rotation_x->multiply(*x);

    double angle = atan2(result->getValue(1, 0), result->getValue(0, 0));

    x->d();
    result->d();

    return angle;
}
