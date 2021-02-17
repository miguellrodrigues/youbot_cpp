//
// Created by miguel on 17/02/2021.
//

#include "YouBot.hpp"

YouBot::YouBot(Controller *controller) {
    this->controller = controller;

    this->arm = new Arm(controller);
    this->base = new Base(controller);
    this->gripper = new Gripper(controller);
}

void YouBot::passiveWait(double seconds) {
    double startTime = controller->getSupervisor()->getTime();

    while (startTime + seconds > controller->getSupervisor()->getTime()) {
        controller->step();
    }
}

double YouBot::getRotationAngle() {
    return controller->getObjectRotation(def)[3];
}

Vector YouBot::getPosition() {
    return Vector(controller->getObjectPosition(def));
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