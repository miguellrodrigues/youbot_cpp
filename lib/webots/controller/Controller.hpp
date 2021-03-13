//
// Created by miguellr on 06/12/2020.
//

#ifndef YOUBOT_CPP_CONTROLLER_H
#define YOUBOT_CPP_CONTROLLER_H

#include "webots/Supervisor.hpp"
#include "vector"

using namespace std;
using namespace webots;

class Controller {
public:
    explicit Controller(Supervisor *supervisor, unsigned int samplingRate);

    ~Controller();

    double step();

    Supervisor *getSupervisor();

    vector<Device *> getAllDevices();

    vector<Device *> getDeviceByType(unsigned int nodeType);

    vector<PositionSensor *> getPositionSensors();

    vector<DistanceSensor *> getDistanceSensors();

    vector<LightSensor *> getLightSensors();

    vector<TouchSensor *> getTouchSensors();

    vector<Motor *> getMotors();

    Device *getDeviceByName(string name);

    double *getObjectPosition(const string& nodeDef);

    double *getObjectRotation(const string& nodeDef);

    void setObjectPosition(const string& nodeDef, double *position);

    void setObjectRotation(const string& nodeDef, double *rotation);

    double *getObjectVelocity(const string& nodeDef);

    void setupMotors(double position);

    void setMotorVelocity(unsigned int index, double velocity);

    double *getObjectOrientation(const string& nodeDef);

private:
    Supervisor *supervisor;

    unsigned int samplingRate;
};


#endif //CINEMATICCONTROLLER_CONTROLLER_H
