//
// Created by miguel on 17/02/2021.
//

#include "Angle.hpp"
#include <cmath>

Angle::Angle() {
    rotation_x = new Matrix(3, 3, false);

    double phi = -1.5708;

    double data[9] = {
            1, 0, 0,
            0, cos(phi), -sin(phi),
            0, sin(phi), cos(phi)
    };

    rotation_x->assign_matrix_array(reinterpret_cast<double *>(data));
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

double Angle::calculateAngle(double *data) {
    auto R = calculateMatrix(data);

    auto result = rotation_x->transpose()->multiply(R);

    return atan2(result->getValue(1, 0), result->getValue(0, 0));
}
