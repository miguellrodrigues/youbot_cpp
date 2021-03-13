//
// Created by miguel on 17/02/2021.
//

#include "YouBot.hpp"

#include <utility>

YouBot::YouBot(Controller *controller) {
    this->controller = controller;
    this->arm = new Arm(controller);
    this->base = new Base(controller);
    this->gripper = new Gripper(controller);

    this->arm->reset();
    this->arm->setOrientation(Arm::ARM_FRONT);

    this->position = new Vector(controller->getObjectPosition(def));

    double phi = -(M_PI / 2.0);

    double data[9] = {
            1, 0, 0,
            0, cos(phi), -sin(phi),
            0, sin(phi), cos(phi)
    };

    auto rotation_x = new Matrix(3, 3, false);

    rotation_x->assign_matrix_array(data);

    this->angle = new Angle(rotation_x);
}

void YouBot::passiveWait(double seconds) {
    double startTime = controller->getSupervisor()->getTime();

    while (startTime + seconds > controller->getSupervisor()->getTime()) {
        controller->step();
    }
}

double YouBot::getRotationAngle() {
    return this->angle->calculateAngle(controller->getObjectRotation(def));//this->angle.calculateAngle(controller->getObjectOrientation(def));
}

Vector YouBot::getPosition() {
    this->position->update(controller->getObjectPosition(def));
    return *this->position;
}

unsigned int YouBot::getHeight() {
    return arm->getCurrentHeight();
}

unsigned int YouBot::getOrientation() {
    return arm->getCurrentOrientation();
}

void YouBot::armReset() {
    arm->reset();
}

void YouBot::setArmHeight(unsigned int height) {
    arm->setHeight(height);
}

void YouBot::increaseArmHeight() {
    arm->increaseHeight();
}

void YouBot::decreaseArmHeight() {
    arm->decreaseHeight();
}

void YouBot::setArmOrientation(unsigned int orientation) {
    arm->setOrientation(orientation);
}

void YouBot::increaseArmOrientation() {
    arm->increaseOrientation();
}

void YouBot::decreaseArmOrientation() {
    arm->decreaseOrientation();
}

void YouBot::grip() {
    gripper->grip();
}

void YouBot::gripRelease() {
    gripper->release();
}

void YouBot::setWheelsSpeed(vector<double> speeds) {
    base->setWheelsSpeed(speeds);
}

void YouBot::turnLeft() {
    base->turnLeft();
}

void YouBot::turnRight() {
    base->turnRight();
}

void YouBot::strafeLeft() {
    base->strafeLeft();
}

void YouBot::strafeRight() {
    base->strafeRight();
}

void YouBot::setGripperOrientation(double orientation) {
    arm->setSubRotation(Arm::ARM5, orientation);
}

void YouBot::pickupBox(double orientation) {
    gripRelease();
    passiveWait(0.5);

    setArmHeight(Arm::ARM_FRONT_TABLE_BOX);
    setGripperOrientation(orientation);
    passiveWait(1.2);
    grip();
    passiveWait(.5);
}

void YouBot::throwBox() {
    setArmHeight(Arm::ARM_PREPARE_LAUNCH);
    passiveWait(1.0);

    setArmHeight(Arm::ARM_LAUNCH);
    passiveWait(.55);
    gripRelease();
    passiveWait(.16);

    armReset();
    grip();
    passiveWait(2.0);
}

void YouBot::setArmsPosition(vector<Arm::Arms> arms, vector<double> positions) {
    this->arm->setArmsPosition(std::move(arms), std::move(positions));
}
