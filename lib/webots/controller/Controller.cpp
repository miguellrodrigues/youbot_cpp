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

    devices.clear();

    return sensors;
}

vector<DistanceSensor *> Controller::getDistanceSensors() {
    vector<Device *> devices = getDeviceByType(Node::DISTANCE_SENSOR);

    vector<DistanceSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((DistanceSensor *) device); }

    devices.clear();

    return sensors;
}

vector<LightSensor *> Controller::getLightSensors() {
    vector<Device *> devices = getDeviceByType(Node::LIGHT_SENSOR);

    vector<LightSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((LightSensor *) device); }

    devices.clear();

    return sensors;
}

vector<TouchSensor *> Controller::getTouchSensors() {
    vector<Device *> devices = getDeviceByType(Node::TOUCH_SENSOR);

    vector<TouchSensor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((TouchSensor *) device); }

    devices.clear();

    return sensors;
}

vector<Motor *> Controller::getMotors() {
    vector<Device *> devices = getDeviceByType(Node::ROTATIONAL_MOTOR);

    vector<Motor *> sensors;

    sensors.reserve(devices.size());

    for (auto &device : devices) { sensors.push_back((Motor *) device); }

    devices.clear();

    return sensors;
}

Device *Controller::getDeviceByName(string name) {
    vector<Device *> allDevices = getAllDevices();

    for (auto &device : allDevices) {
        if (device->getName() == name) {
            return device;
        }
    }

    allDevices.clear();

    return nullptr;
}

double *Controller::getObjectPosition(const string& nodeDef) {
    return const_cast<double *>(supervisor->getFromDef(nodeDef)->getPosition());
}

double *Controller::getObjectRotation(const string& nodeDef) {
    return const_cast<double *>(supervisor->getFromDef(nodeDef)->getField("rotation")->getSFRotation());
}

double *Controller::getObjectOrientation(const string& nodeDef) {
    return const_cast<double *>(supervisor->getFromDef(nodeDef)->getOrientation());
}

void Controller::setObjectPosition(const string& nodeDef, double *position) {
    supervisor->getFromDef(nodeDef)->getField("translation")->setSFVec3f(position);
}

void Controller::setObjectRotation(const string& nodeDef, double *rotation) {
    supervisor->getFromDef(nodeDef)->getField("rotation")->setSFRotation(rotation);
}

double *Controller::getObjectVelocity(const string& nodeDef) {
    return const_cast<double *>(getSupervisor()->getFromDef(nodeDef)->getVelocity());
}

void Controller::setupMotors(double position) {
    vector<Motor *> motors = getMotors();

    for (auto &motor : motors) {
        motor->setVelocity(.0);
        motor->setPosition(position);
    }

    motors.clear();
}

void Controller::setMotorVelocity(unsigned int index, double velocity) {
    vector<Motor *> motors = getMotors();

    motors.at(index)->setVelocity(velocity);

    motors.clear();
}

Controller::~Controller() {
    delete this->supervisor;
}