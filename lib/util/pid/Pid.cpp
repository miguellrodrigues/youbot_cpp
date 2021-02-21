//
// Created by miguellr on 29/11/2020.
//

#include "Pid.hpp"

Pid::Pid(double kp, double ki, double kd, double saturation, double maxError) {
    this->kp = kp;
    this->ki = ki;
    this->kd = kd;

    this->accumulator = 0;
    this->error = 0;
    this->oldError = 0;
    this->out = 0;
    this->saturation = saturation;
    this->maxError = maxError;
}

double Pid::compute(double err, double time) {
    this->oldError = error;
    this->error = err;

    if (-saturation < out < saturation) {
        if (-maxError < err < maxError) {
            accumulator += ((err + oldError) / 2.0) * time;
        }
    }

    double proportional = kp * error;
    double integral = ki * accumulator;
    double derivative = kd * (err - oldError) / time;

    double d = proportional + integral + derivative;

    this->out = d;

    return d;
}