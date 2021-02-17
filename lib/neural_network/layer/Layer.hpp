//
// Created by miguel on 31/01/2021.
//

#ifndef REDENEURAL_LAYER_HPP
#define REDENEURAL_LAYER_HPP

#include "../neuron/Neuron.hpp"
#include "../../util/matrix/Matrix.hpp"

#include <vector>

using std::vector;

class Layer {
public:
    explicit Layer(unsigned int size);

    void setNeuronValue(unsigned int index, double value);

    Matrix *convertValues();

    Matrix *convertActivatedValues();

    Matrix *convertDerivedValues();

    unsigned int getNeuronsSize() const { return this->neuronSize; }

    vector<Neuron *> getNeurons() { return this->neurons; }

private:
    unsigned int neuronSize;
    vector<Neuron *> neurons;
};


#endif //REDENEURAL_LAYER_HPP
