//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_ANGLE_HPP
#define YOUBOT_CPP_ANGLE_HPP


#include "../matrix/Matrix.hpp"

class Angle {
public:
    Angle();

    static Matrix &calculateMatrix(double *data);

    double calculateAngle(double *data);

private:
    Matrix *rotation_x;
};


#endif //YOUBOT_CPP_ANGLE_HPP
