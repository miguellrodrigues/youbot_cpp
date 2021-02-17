//
// Created by miguel on 17/02/2021.
//

#include "Arm.hpp"
#include <string>
#include <utility>
#include <cmath>
#include <iostream>
#include <webots/Motor.hpp>

using namespace webots;
using std::cout;
using std::endl;
using std::string;

Arm::Arm(Controller *controller) {
    this->controller = controller;

    elements.reserve(5);

    elements.push_back((Motor *) controller->getDeviceByName("arm1"));
    elements.push_back((Motor *) controller->getDeviceByName("arm2"));
    elements.push_back((Motor *) controller->getDeviceByName("arm3"));
    elements.push_back((Motor *) controller->getDeviceByName("arm4"));
    elements.push_back((Motor *) controller->getDeviceByName("arm5"));

    controller->setMotorVelocity(ARM2, 3.1415);
}

void Arm::setArmsPosition(vector<Arms> arms, vector<double> positions) {
    for (unsigned int i = 0; i < arms.size(); ++i) {
        elements.at(arms.at(i))->setPosition(positions.at(i));
    }
}

void Arm::change(vector<double> positions) {
    setArmsPosition({ARM2, ARM3, ARM4, ARM5}, std::move(positions));
}

void Arm::reset() {
    setArmsPosition({ARM1, ARM2, ARM3, ARM4, ARM5}, {.0, 1.57, -2.635, 1.78, .0});
}

void Arm::setHeight(Arm::Height height) {
    switch (height) {
        case ARM_FRONT_FLOOR:
            change({-.97, -1.55, -.61, .0});
            break;
        case ARM_FRONT_PLATE:
            change({-.62, -.98, -1.53, .0});
            break;
        case ARM_HANOI_PREPARE:
            change({-.4, -1.2, -(M_PI / 2.0), (M_PI / 2.0)});
            break;
        case ARM_FRONT_CARDBOARD_BOX:
            change({.0, -.77, -1.21, .0});
            break;
        case ARM_RESET:
            change({1.57, -2.635, 1.78, .0});
            break;
        case ARM_BACK_PLATE_HIGH:
            change({.678, .682, 1.74, .0});
            break;
        case ARM_BACK_PLATE_LOW:
            change({.92, .42, 1.78, .0});
            break;
        case ARM_FRONT_TABLE_BOX:
            change({-.7, -.38, -1.365, 0.0});
            break;
        case ARM_PREPARE_LAUNCH:
            change({1.0, .72, .3, .0});
            break;
        case ARM_LAUNCH:
            change({-.5, -.5, -.3, .0});
            break;
        case ARM_ALIGNING:
            change({-.278, -.682, radians(-75), .0});
            break;
        default:
            cout << "SetHeight Bad argument";
            break;
    }

    curretHeight = height;
}


