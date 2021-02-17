//
// Created by miguel on 17/02/2021.
//

#include "Gripper.hpp"
#include <webots/Motor.hpp>

#define LEFT 0
#define RIGHT 1

#define MIN_POS .0
#define MAX_POS .025

Gripper::Gripper(Controller *controller) {
    this->controller = controller;

    fingers.reserve(2);

    fingers.push_back((Motor *) controller->getDeviceByName("finger1"));
    fingers.push_back((Motor *) controller->getDeviceByName("finger2"));

    fingers.at(LEFT)->setVelocity(.06);
    fingers.at(RIGHT)->setVelocity(.06);
}

void Gripper::grip() {
    fingers.at(LEFT)->setPosition(MIN_POS);
    fingers.at(RIGHT)->setPosition(MIN_POS);
}

void Gripper::release() {
    fingers.at(LEFT)->setPosition(MAX_POS);
    fingers.at(RIGHT)->setPosition(MAX_POS);
}
