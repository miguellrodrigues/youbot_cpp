//
// Created by miguellr on 29/11/2020.
//

#include "Pid.h"

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

double Pid::update(double err, double time) {
    this->oldError = error;
    this->error = err;

    if (out < saturation && out > -saturation) {
        if (err < maxError && err > -maxError) {
            accumulator += ((err + oldError) / 2.0) * time;
        }
    }

    double proportional = kp * error;
    double integral = ki * accumulator;
    double derivative = kd * (err - oldError) / time;

    double d = proportional + integral + derivative;

    d = d > saturation ? saturation : d < -saturation ? -saturation : d;

    this->out = d;

    return d;
}