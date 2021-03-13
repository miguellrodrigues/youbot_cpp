//
// Created by miguellr on 06/12/2020.
//

#include <webots/Device.hpp>
#include <webots/Motor.hpp>

#include <string>
#include "Controller.hpp"

using namespace std;

Controller::Controller(Supervisor *supervisor, unsigned int samplingRate) {
    this->supervisor = supervisor;
    this->samplingRate = samplingRate;
}

double Controller::step() {
    return this->getSupervisor()->step(samplingRate);
}

Supervisor *Controller::getSupervisor() {
    return this->supervisor;
}

vector<Device *> Controller::getAllDevices() {
    unsigned int devicesCount = getSupervisor()->getNumberOfDevices();

    vector<Device *> devices;

    devices.reserve(devicesCount);

    for (unsigned int i = 0; i < devicesCount; ++i) {
        devices.push_back(getSupervisor()->getDeviceByIndex(i));
    }

    return devices;
}

vector<Device *> Controller::getDeviceByType(unsigned int nodeType) {
    vector<Device *> allDevices = getAllDevices();

    vector<Device *> filtered;

    for (auto &device : allDevices) {
        if (device->getNodeType() == nodeType) {
            filtered.push_back(device);
        }
    }

    return filtered;
}

vector<PositionSensor *> Controller::getPositionSensors() {
    vector<Device *> devices = getDeviceByType(Node::POSITION_SENSOR);

    vector<PositionSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((PositionSensor *) device); }

    return sensors;
}

vector<DistanceSensor *> Controller::getDistanceSensors() {
    vector<Device *> devices = getDeviceByType(Node::DISTANCE_SENSOR);

    vector<DistanceSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((DistanceSensor *) device); }

    return sensors;
}

vector<LightSensor *> Controller::getLightSensors() {
    vector<Device *> devices = getDeviceByType(Node::LIGHT_SENSOR);

    vector<LightSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((LightSensor *) device); }

    return sensors;
}

vector<TouchSensor *> Controller::getTouchSensors() {
    vector<Device *> devices = getDeviceByType(Node::TOUCH_SENSOR);

    vector<TouchSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((TouchSensor *) device); }

    return sensors;
}

vector<Motor *> Controller::getMotors() {
    vector<Device *> devices = getDeviceByType(Node::ROTATIONAL_MOTOR);

    vector<Motor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((Motor *) device); }

    return sensors;
}

Device *Controller::getDeviceByName(string name) {
    vector<Device *> allDevices = getAllDevices();

    for (auto &device : allDevices) {
        if (device->getName() == name) {
            return device;
        }
    }

    return nullptr;
}

double *Controller::getObjectPosition(const string& nodeDef) {
    auto robotNode = supervisor->getFromDef(nodeDef);

    auto translationField = robotNode->getField("translation");

    return const_cast<double *>(translationField->getSFVec3f());
}

double *Controller::getObjectRotation(const string& nodeDef) {
    auto robotNode = supervisor->getFromDef(nodeDef);

    auto rotationField = robotNode->getField("rotation");

    return const_cast<double *>(rotationField->getSFRotation());
}

double *Controller::getObjectOrientation(const string& nodeDef) {
    auto node = supervisor->getFromDef(nodeDef);

    return const_cast<double *>(node->getOrientation());
}

void Controller::setObjectPosition(const string& nodeDef, double *position) {
    auto node = supervisor->getFromDef(nodeDef);

    auto translationField = node->getField("translation");

    translationField->setSFVec3f(position);
}

void Controller::setObjectRotation(const string& nodeDef, double *rotation) {
    auto robotNode = supervisor->getFromDef(nodeDef);

    auto rotationField = robotNode->getField("rotation");

    rotationField->setSFRotation(rotation);
}

double *Controller::getObjectVelocity(const string& nodeDef) {
    return const_cast<double *>(getSupervisor()->getFromDef(nodeDef)->getVelocity());
}

void Controller::setupMotors(double position) {
    vector<Motor *> motors = getMotors();

    for (auto &motor : motors) {
        motor->setPosition(position);
    }
}

void Controller::setMotorVelocity(unsigned int index, double velocity) {
    vector<Motor *> motors = getMotors();

    motors.at(index)->setVelocity(velocity);
}

Controller::~Controller() {
    delete this->supervisor;
}