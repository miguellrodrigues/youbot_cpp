//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_BASE_HPP
#define YOUBOT_CPP_BASE_HPP


#include "../../controller/Controller.hpp"

class Base {
public:
    Base(Controller *controller);

    void setWheelSpeed(unsigned int index, double speed);

    void setWheelsSpeed(vector<double> speeds);

    void reset();

    void forwards();

    void backwards();

    void turnLeft();

    void turnRight();

    void strafeLeft();

    void strafeRight();

private:
    const unsigned int WHEELS_SIZE = 4;
    const double SPEED = 4;

    Controller *controller;

    vector<Motor *> wheels;
};


#endif //YOUBOT_CPP_BASE_HPP
