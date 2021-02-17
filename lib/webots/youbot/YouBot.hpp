//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_YOUBOT_HPP
#define YOUBOT_CPP_YOUBOT_HPP

#include "../controller/Controller.h"
#include "../../util/vector/Vector.hpp"
#include "arm/Arm.hpp"
#include "base/Base.hpp"
#include "gripper/Gripper.hpp"
#include "../../util/angle/Angle.hpp"

using namespace lib;

class YouBot {
public:
    explicit YouBot(Controller *controller);

    void passiveWait(double seconds);

    double getRotationAngle();

    Vector getPosition();

    unsigned int getHeight();

    unsigned int getOrientation();

    void armReset();

    void setArmHeight(unsigned int height);

    void increaseArmHeight();

    void decreaseArmHeight();

    void setArmOrientation(unsigned int orientation);

    void increaseArmOrientation();

    void decreaseArmOrientation();

    void grip();

    void gripRelease();

    void setWheelsSpeed(vector<double> speeds);

    void setGripperOrientation(double orientation);

    void turnLeft();
    void turnRight();

    void strafeLeft();
    void strafeRight();

    void pickupBox(double orientation);

    void throwBox();

private:
    Controller *controller;

    Arm *arm;
    Base *base;
    Gripper *gripper;

    Angle *angle;

    string def = "youBot";
};


#endif //YOUBOT_CPP_YOUBOT_HPP
