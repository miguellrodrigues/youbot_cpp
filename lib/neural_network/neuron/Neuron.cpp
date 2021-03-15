//
// Created by miguel on 31/01/2021.
//

#include "Neuron.hpp"
#include "../../util/Numbers.hpp"

Neuron::Neuron(double val) {
    this->value = static_cast<double *>(malloc(sizeof(double)));
    this->activatedValue = static_cast<double *>(malloc(sizeof(double)));
    this->derivedValue = static_cast<double *>(malloc(sizeof(double)));

    setValue(val);
}

void Neuron::activate() {
    *this->activatedValue = Numbers::sigmoid(*this->value);
}

void Neuron::derive() {
    *this->derivedValue = Numbers::derivative(*this->activatedValue);
}

double Neuron::getValue() const {
    return *this->value;
}

double Neuron::getActivatedValue() const {
    return *this->activatedValue;
}

double Neuron::getDerivedValue() const {
    return *this->derivedValue;
}

void Neuron::setValue(double val) {
    *this->value = val;

    this->activate();
    this->derive();
}

Neuron::~Neuron() {
    delete this->value;
    delete this->activatedValue;
    delete this->derivedValue;
}
