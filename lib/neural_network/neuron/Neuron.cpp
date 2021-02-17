//
// Created by miguel on 31/01/2021.
//

#include "Neuron.hpp"
#include <cmath>

double sigmoid(double x) {
    return (x / (1 + std::abs(x)));
}

double derivative(double x) {
    return (1 / pow((1 + std::abs(x)), 2.0));
}

Neuron::Neuron(double val) : value(val) {
    setValue(val);
}

void Neuron::activate() {
    this->activatedValue = sigmoid(this->value);
}

void Neuron::derive() {
    this->derivedValue = derivative(this->activatedValue);
}

double Neuron::getValue() const {
    return this->value;
}

double Neuron::getActivatedValue() const {
    return this->activatedValue;
}

double Neuron::getDerivedValue() const {
    return this->derivedValue;
}

void Neuron::setValue(double val) {
    this->value = val;

    this->activate();
    this->derive();
}
