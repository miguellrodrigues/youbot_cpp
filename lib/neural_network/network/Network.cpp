//
// Created by miguel on 30/01/2021.
//

#include "Network.hpp"
#include <cmath>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "../../util/Numbers.hpp"

using namespace std;
using json = nlohmann::json;

Network::Network(unsigned int *topology, unsigned int topologySize) {
    this->topologySize = topologySize;

    this->topology.reserve(topologySize);

    for (int i = 0; i < topologySize; ++i) {
        this->topology.push_back(topology[i]);
    }

    for (unsigned int i = 0; i < topologySize; ++i) {
        Layer *layer = new Layer(this->topology.at(i));

        this->layers.push_back(layer);
    }

    for (unsigned int i = 0; i < (topologySize - 1); ++i) {
        Matrix *weightMatrix = new Matrix(topology[i + 1], topology[i], true);

        this->weightMatrices.push_back(weightMatrix);
    }

    for (int i = 0; i < this->topology.at(this->topology.size() - 1); i++) {
        errors.push_back(0.00);
        derivedErrors.push_back(0.00);
    }

    this->temp_weights.reserve(this->weightMatrices.size());
}

Network *Network::clone() {
    Network *n = new Network(this->topology.data(), this->topologySize);

    for (unsigned int i = 0; i < this->weightMatrices.size(); ++i) {
        n->weightMatrices.at(i) = new Matrix(*this->weightMatrices.at(i));
    }

    n->setFitness(this->getFitness());
    n->bias = this->bias;
    n->learningRate = this->learningRate;

    return n;
}

void Network::train(vector<double> input, vector<double> meta) {
    Matrix *inputMatrix = Matrix::vectorToMatrix(std::move(input));
    Matrix *metaMatrix = Matrix::vectorToMatrix(std::move(meta));

    setCurrentInput(inputMatrix);

    feedForward();
    setErrors(*metaMatrix);
    backPropagation();

    delete inputMatrix;
    delete metaMatrix;
}

vector<double> Network::predict(vector<double> input) {
    Matrix *inputMatrix = Matrix::vectorToMatrix(std::move(input));

    setCurrentInput(inputMatrix);
    feedForward();

    Matrix *output_layer = this->layers.at(this->topologySize - 1)->convertActivatedValues();

    vector<double> out = output_layer->to_vector();

    delete output_layer;
    delete inputMatrix;

    return out;
}

void Network::mutate(double rate) {
    for (Matrix *weightMatrix : this->weightMatrices) {
        unsigned int count = rate * weightMatrix->getCols();

        unsigned int random_row = Numbers::randomInt(0, (int) weightMatrix->getRows() - 1);

        for (unsigned int i = 0; i < count; ++i) {
            unsigned int random_col = Numbers::randomInt(0, (int) weightMatrix->getCols() - 1);

            double value = weightMatrix->getValue(random_row, random_col);

            weightMatrix->setValue(random_row, random_col, value + Numbers::randomDouble(-1.0, 1.0));
        }
    }
}

void Network::crossOver(Network *net, Network *father, Network *mother) {
    for (int i = 0; i < net->getWeightMatrices().size(); i++) {
        Matrix *fatherWeight = father->weightMatrices.at(i);
        Matrix *motherWeight = mother->weightMatrices.at(i);

        for (int j = 0; j < fatherWeight->getRows(); ++j) {
            for (int k = 0; k < fatherWeight->getCols(); ++k) {
                if (Numbers::randomInt(0, 1) == 0) {
                    net->weightMatrices.at(i)->setValue(j, k, fatherWeight->getValue(j, k));
                } else {
                    net->weightMatrices.at(i)->setValue(j, k, motherWeight->getValue(j, k));
                }
            }
        }
    }
}

void Network::assign(Network &other) {
    for (int i = 0; i < this->weightMatrices.size(); ++i) {
        this->weightMatrices.at(i) = new Matrix(*other.weightMatrices.at(i));
    }

    this->bias = other.getBias();
    this->fitness = other.getFitness();
}

void Network::save(const string &path) {
    json data;

    vector<vector<double>> a;

    vector<double> b = {1, 2, 3, 4, 5, 6};

    a.push_back(b);

    data["topology"] = this->topology;
    data["weight_matrices"] = this->vectorizeWeightMatrices();

    std::ofstream o(path);

    o << std::setw(4) << data << endl;

    o.close();
}

Network &Network::load(const string &path) {
    std::ifstream ifs(path);

    json data;

    ifs >> data;

    ifs.close();

    vector<unsigned int> topology = data["topology"];
    vector<vector<double>> weightMatrices = data["weight_matrices"];

    Network *network = new Network(topology.data(), topology.size());

    for (unsigned int i = 0; i < network->weightMatrices.size(); i++) {
        Matrix *matrix = network->weightMatrices.at(i);
        vector<double> json_matrix = weightMatrices.at(i);

        unsigned int count = 0;
        for (unsigned int j = 0; j < matrix->getRows(); ++j) {
            for (unsigned int k = 0; k < matrix->getCols(); ++k) {
                matrix->setValue(j, k, json_matrix.at(count++));
            }
        }
    }

    return *network;
}

vector<vector<double>> Network::vectorizeWeightMatrices() {
    vector<vector<double>> data;

    for (Matrix *matrix : this->weightMatrices) {
        vector<double> x;

        for (unsigned int i = 0; i < matrix->getRows(); i++) {
            for (unsigned int j = 0; j < matrix->getCols(); j++) {
                x.push_back(matrix->getValue(i, j));
            }
        }

        data.push_back(x);
    }

    return data;
}

void Network::setCurrentInput(Matrix *matrix) {
    for (int i = 0; i < matrix->getRows(); ++i) {
        this->layers.at(0)->setNeuronValue(i, matrix->getValue(i, 0));
    }
}

void Network::feedForward() {
    Matrix *left;
    Matrix *right;
    Matrix *r;

    for (int i = 0; i < (this->topologySize - 1); ++i) {
        if (i != 0) {
            left = this->layers.at(i)->convertActivatedValues();
        } else {
            left = this->layers.at(i)->convertValues();
        }

        right = new Matrix(*this->weightMatrices.at(i));

        r = right->multiply(*left);

        for (int j = 0; j < r->getRows(); ++j) {
            this->layers.at(i + 1)->setNeuronValue(j, r->getValue(j, 0) + (this->bias));
        }

        delete r;
        delete left;
        delete right;
    }
}

void Network::setErrors(Matrix &meta) {
    if (meta.getRows() == 0) {
        std::cout << "invalid meta matrix" << std::endl;
        return;
    }

    unsigned int outputLayerIndex = this->layers.size() - 1;

    if (meta.getRows() != this->layers.at(outputLayerIndex)->getNeurons().size()) {
        std::cout << "invalid meta matrix" << std::endl;
        return;
    }

    this->globalError = 0.0;

    vector<Neuron *> outputNeurons = this->layers.at(outputLayerIndex)->getNeurons();

    for (int i = 0; i < meta.getRows(); ++i) {
        double t = meta.getValue(i, 0);
        double y = outputNeurons.at(i)->getDerivedValue();

        errors.at(i) = 0.5 * pow((t - y), 2.0);
        derivedErrors.at(i) = (y - t);

        this->globalError += errors.at(i);
    }
}

void Network::backPropagation() {
    Matrix *gradients;
    Matrix *_gradients;
    Matrix *derivedOutputValues;
    Matrix *lastHiddenLayerActivated;
    Matrix *deltaWeightsLastHiddenToOutput;
    Matrix *tempWeights;
    Matrix *transposeWeights;
    Matrix *derivedValues;
    Matrix *layerGradients;
    Matrix *layerValues;
    Matrix *deltaWeights;
    Matrix *_tempWeights;

    unsigned int indexOutputLayer = this->topologySize - 1;

    gradients = new Matrix(this->topology.at(indexOutputLayer), 1, false);

    derivedOutputValues = this->layers.at(indexOutputLayer)->convertDerivedValues();

    for (int i = 0; i < this->topology.at(indexOutputLayer); ++i) {
        double error = this->derivedErrors.at(i);
        double output = derivedOutputValues->getValue(i, 0);

        double gradient = error * output;

        gradients->setValue(i, 0, gradient);
    }

    lastHiddenLayerActivated = this->layers.at(indexOutputLayer - 1)->convertActivatedValues();

    deltaWeightsLastHiddenToOutput = gradients->multiply(*lastHiddenLayerActivated->transpose());

    tempWeights = new Matrix(
            this->topology.at(indexOutputLayer),
            this->topology.at(indexOutputLayer - 1),
            false);

    for (int i = 0; i < tempWeights->getRows(); ++i) {
        for (int j = 0; j < tempWeights->getCols(); ++j) {
            double originalValue = this->weightMatrices.at(indexOutputLayer - 1)->getValue(i, j);
            double deltaValue = deltaWeightsLastHiddenToOutput->getValue(i, j);

            deltaValue = this->learningRate * deltaValue;

            tempWeights->setValue(i, j, (originalValue - deltaValue));
        }
    }

    unsigned int x = 0;

    this->temp_weights.push_back(tempWeights->copy());

    delete derivedOutputValues;
    delete lastHiddenLayerActivated;
    delete deltaWeightsLastHiddenToOutput;
    delete tempWeights;

    //last hidden to input

    for (int i = ((int) indexOutputLayer - 1); i > 0; --i) {
        _gradients = gradients->copy();

        transposeWeights = this->weightMatrices.at(i)->transpose();

        gradients = transposeWeights->multiply(*_gradients);

        derivedValues = this->layers.at(i)->convertDerivedValues();

        layerGradients = derivedValues->hadamard(*gradients);

        for (int j = 0; j < layerGradients->getRows(); ++j) {
            for (int k = 0; k < layerGradients->getCols(); ++k) {
                gradients->setValue(j, k, layerGradients->getValue(j, k));
            }
        }

        layerValues =
                i == 1 ? this->layers.at(0)->convertValues() : this->layers.at(i - 1)->convertActivatedValues();

        deltaWeights = (gradients->multiply(*layerValues->transpose()));

        _tempWeights = new Matrix(this->weightMatrices.at(i - 1)->getRows(),
                                        this->weightMatrices.at(i - 1)->getCols(),
                                        false);

        for (int j = 0; j < _tempWeights->getRows(); ++j) {
            for (int k = 0; k < _tempWeights->getCols(); ++k) {
                double originalValue = this->weightMatrices.at(i - 1)->getValue(j, k);
                double deltaValue = deltaWeights->getValue(j, k);

                deltaValue = this->learningRate * deltaValue;

                _tempWeights->setValue(j, k, (originalValue - deltaValue));
            }
        }

        this->temp_weights.push_back(_tempWeights->copy());

        delete _gradients;
        delete transposeWeights;
        delete derivedValues;
        delete layerGradients;
        delete layerValues;
        delete deltaWeights;
        delete _tempWeights;
    }

    delete gradients;

    for (Matrix *matrix : this->weightMatrices) {
        delete matrix;
    }

    this->weightMatrices.clear();

    reverse(this->temp_weights.begin(), this->temp_weights.end());

    for (Matrix *weight : this->temp_weights) {
        this->weightMatrices.push_back(weight->copy());
        delete weight;
    }

    this->temp_weights.clear();
}

void Network::setRecurrentInput() {
    Matrix *output = layers.at(topologySize - 1)->convertActivatedValues();

    for (unsigned int i = topology.at(0) - topology[topologySize - 1]; i < topology.at(0); ++i) {
        this->layers.at(0)->setNeuronValue(i, output->getValue(i - topology[topologySize - 1], 0));
    }
}

Network::~Network() {
    for (Matrix *wm : this->weightMatrices) {
        delete wm;
    }

    for (Layer *ly : this->layers) {
        delete ly;
    }

    this->layers.clear();
    this->weightMatrices.clear();

    this->errors.clear();
    this->derivedErrors.clear();
}