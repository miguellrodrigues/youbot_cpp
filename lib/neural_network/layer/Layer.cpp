//
// Created by miguel on 31/01/2021.
//

#include "Layer.hpp"

Layer::Layer(unsigned int neurons) : neuronSize(neurons) {
    this->neuronSize = neurons;
    this->neurons.reserve(neuronSize);

    for (int i = 0; i < neurons; ++i) {
        this->neurons.push_back(new Neuron(.000));
    }
}

void Layer::setNeuronValue(unsigned int index, double value) {
    this->neurons.at(index)->setValue(value);
}

Matrix *Layer::convertValues() {
    auto *matrix = new Matrix(this->neuronSize, 1, false);

    for (unsigned int i = 0; i < this->neuronSize; ++i) {
        matrix->setValue(i, 0, this->neurons.at(i)->getValue());
    }

    return matrix;
}

Matrix *Layer::convertActivatedValues() {
    auto *matrix = new Matrix(this->neuronSize, 1, false);

    for (unsigned int i = 0; i < this->neuronSize; ++i) {
        matrix->setValue(i, 0, this->neurons.at(i)->getActivatedValue());
    }

    return matrix;
}

Matrix *Layer::convertDerivedValues() {
    auto *matrix = new Matrix(this->neuronSize, 1, false);

    for (unsigned int i = 0; i < this->neuronSize; ++i) {
        matrix->setValue(i, 0, this->neurons.at(i)->getDerivedValue());
    }

    return matrix;
}