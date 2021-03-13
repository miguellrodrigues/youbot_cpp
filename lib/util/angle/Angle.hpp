//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_ANGLE_HPP
#define YOUBOT_CPP_ANGLE_HPP


#include "../matrix/Matrix.hpp"

class Angle {
public:
    Angle();

    static Matrix *calculateMatrix(double *data);

    double calculateAngle(double *data) const;

    const Matrix *rotation_x = new Matrix(3, 3, false);
};


#endif //YOUBOT_CPP_ANGLE_HPP
