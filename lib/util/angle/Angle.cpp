//
// Created by miguel on 17/02/2021.
//

#include "Angle.hpp"
#include <cmath>

Angle::Angle(Matrix *rotation_x) {
    this->rotation_x = rotation_x->transpose();
}

double Angle::calculateAngle(Matrix *rotationMatrix) {
    auto result = this->rotation_x->multiply(*rotationMatrix);

    double angle = atan2(result->getValue(1, 0), result->getValue(0, 0));

    delete result;

    return angle;
}

Angle::~Angle() {
    delete this->rotation_x;
}
