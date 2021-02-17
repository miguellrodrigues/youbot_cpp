//
// Created by miguel on 17/02/2021.
//

#ifndef YOUBOT_CPP_ARM_HPP
#define YOUBOT_CPP_ARM_HPP


#include <cmath>
#include "../../controller/Controller.h"

class Arm {
public:
    enum Height {
        ARM_FRONT_FLOOR,
        ARM_FRONT_PLATE,
        ARM_HANOI_PREPARE,
        ARM_FRONT_CARDBOARD_BOX,
        ARM_RESET ,
        ARM_BACK_PLATE_HIGH,
        ARM_BACK_PLATE_LOW ,
        ARM_FRONT_TABLE_BOX,
        ARM_PREPARE_LAUNCH,
        ARM_LAUNCH,
        ARM_ALIGNING,
        ARM_MAX_HEIGHT,
    };

    enum Orientation {
        ARM_BACK_LEFT,
        ARM_LEFT,
        ARM_FRONT_LEFT ,
        ARM_FRONT,
        ARM_FRONT_RIGHT,
        ARM_RIGHT,
        ARM_BACK_RIGHT,
        ARM_MAX_SIDE,
    };

    enum Arms {
        ARM1,
        ARM2,
        ARM3,
        ARM4,
        ARM5
    };

    Arm(Controller *controller);

    void setArmsPosition(vector<Arms> arms, vector<double> positions);

    void reset();

    void setHeight(unsigned int height);

    void increaseHeight();

    void decreaseHeight();

    void setOrientation(unsigned int orientation);

    void increaseOrientation();

    void decreaseOrientation();

    void setSubRotation(unsigned int arm, double radian);

    static double radians(double degrees)
    {
        return degrees * (M_PI / 180.0);
    }

private:
    const unsigned int ARMS_SIZE = 5;

    unsigned int currentHeight = 0;
    unsigned int currentOrientation = 0;

    Controller *controller;

    vector<Motor *> elements;

    void change(vector<double> positions);
};


#endif //YOUBOT_CPP_ARM_HPP
