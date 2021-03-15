//
// Created by miguel on 31/01/2021.
//

#ifndef YOUBOT_CPP_LAYER_HPP
#define YOUBOT_CPP_LAYER_HPP

#include "../neuron/Neuron.hpp"
#include "../../util/matrix/Matrix.hpp"

#include <vector>

using std::vector;

class Layer {
public:
    explicit Layer(unsigned int size);

    ~Layer();

    void setNeuronValue(unsigned int index, double value);

    Matrix *convertValues();

    Matrix *convertActivatedValues();

    Matrix *convertDerivedValues();

    vector<Neuron *> getNeurons() { return this->neurons; }

private:
    vector<Neuron *> neurons;
};


#endif //REDENEURAL_LAYER_HPP
