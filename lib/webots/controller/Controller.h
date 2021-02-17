//
// Created by miguellr on 06/12/2020.
//

#ifndef CINEMATICCONTROLLER_CONTROLLER_H
#define CINEMATICCONTROLLER_CONTROLLER_H

#include "webots/Supervisor.hpp"
#include "vector"

using namespace std;
using namespace webots;

class Controller {
public:
    explicit Controller(Supervisor *supervisor, unsigned int samplingRate, bool setupSensors);

    void step();

    Supervisor *getSupervisor();

    vector<Device *> getAllDevices();

    vector<Device *> getDeviceByType(unsigned int nodeType);

    vector<PositionSensor *> getPositionSensors();

    vector<DistanceSensor *> getDistanceSensors();

    vector<LightSensor *> getLightSensors();

    vector<TouchSensor *> getTouchSensors();

    vector<Motor *> getMotors();

    Device *getDeviceByName(string name);

    double *getObjectPosition(string nodeDef);

    double *getObjectRotation(string nodeDef);

    void setObjectPosition(string nodeDef, double *position);

    void setObjectRotation(string nodeDef, double *rotation);

    double *getObjectVelocity(string nodeDef);

    void setupMotors(double position);

    void setMotorVelocity(unsigned int index, double velocity);

private:
    Supervisor *supervisor;

    unsigned int samplingRate;
};


#endif //CINEMATICCONTROLLER_CONTROLLER_H
