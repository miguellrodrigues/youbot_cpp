//
// Created by miguellr on 06/12/2020.
//

#ifndef YOUBOT_CPP_PID_HPP
#define YOUBOT_CPP_PID_HPP


class Pid {
public:
    Pid(double kp, double ki, double kd, double saturation, double maxError);

    double compute(double err, double time);

private:
    double kp, ki, kd;

    double error, oldError;
    double accumulator;
    double out;
    double saturation;
    double maxError;
};

#endif //CINEMATICCONTROLLER_PID_H
