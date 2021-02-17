//
// Created by miguel on 17/02/2021.
//

#include "Base.hpp"
#include <webots/Motor.hpp>

Base::Base(Controller *controller) {
    this->controller = controller;

    wheels.reserve(WHEELS_SIZE);

    wheels.push_back((Motor *) controller->getDeviceByName("wheel1"));
    wheels.push_back((Motor *) controller->getDeviceByName("wheel2"));
    wheels.push_back((Motor *) controller->getDeviceByName("wheel3"));
    wheels.push_back((Motor *) controller->getDeviceByName("wheel4"));

    for (auto &wheel : wheels) {
        wheel->setPosition(INFINITY);
        wheel->setVelocity(.0);
    }
}

void Base::setWheelSpeed(unsigned int index, double speed) {
    wheels.at(index)->setVelocity(speed);
}

void Base::setWheelsSpeed(vector<double> speeds) {
    for (unsigned int i = 0; i < wheels.size(); ++i) {
        wheels.at(i)->setVelocity(speeds.at(i));
    }
}

void Base::reset() {
    setWheelsSpeed({.0, .0, .0, .0});
}

void Base::forwards() {
    setWheelsSpeed({SPEED, SPEED, SPEED, SPEED});
}

void Base::backwards() {
    setWheelsSpeed({-SPEED, -SPEED, -SPEED, -SPEED});
}

void Base::turnLeft() {
    setWheelsSpeed({-SPEED, SPEED, -SPEED, SPEED});
}

void Base::turnRight() {
    setWheelsSpeed({SPEED, -SPEED, SPEED, -SPEED});
}

void Base::strafeLeft() {
    setWheelsSpeed({SPEED, -SPEED, -SPEED, SPEED});
}

void Base::strafeRight() {
    setWheelsSpeed({-SPEED, SPEED, SPEED, -SPEED});
}
