//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_ANGLE_HPP
#define YOUBOT_CPP_ANGLE_HPP


#include "../matrix/Matrix.hpp"

class Angle {
public:
    explicit Angle(Matrix *rotation_x);

    ~Angle();

    double calculateAngle(Matrix *rotationMatri);

private:
    Matrix *rotation_x;
};


#endif //YOUBOT_CPP_ANGLE_HPP
