//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_GRIPPER_HPP
#define YOUBOT_CPP_GRIPPER_HPP


#include "../../controller/Controller.hpp"

class Gripper {

public:
    explicit Gripper(Controller *controller);

    void grip();

    void release();

private:
    const unsigned int FINGERS_SIZE = 2;

    Controller *controller;

    vector<Motor *> fingers;
};


#endif //YOUBOT_CPP_GRIPPER_HPP
