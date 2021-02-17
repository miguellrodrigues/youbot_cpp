//
// Created by miguellr on 06/12/2020.
//

#ifndef CINEMATICCONTROLLER_PID_H
#define CINEMATICCONTROLLER_PID_H


class Pid {
public:
    Pid(double kp, double ki, double kd, double saturation, double maxError);

    double update(double err, double time);

private:
    double kp, ki, kd;

    double error, oldError;
    double accumulator;
    double out;
    double saturation;
    double maxError;
};

#endif //CINEMATICCONTROLLER_PID_H
