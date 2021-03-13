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

using namespace std;
using json = nlohmann::json;

double mutateFunction(double x) {
    return x + Matrix::randomDouble(-1.0, 1.0);
}

Network::Network(unsigned int *topology, unsigned int topologySize) {
    this->topologySize = topologySize;

    this->topology.reserve(topologySize);

    for (int i = 0; i < topologySize; ++i) {
        this->topology.push_back(topology[i]);
    }

    for (unsigned int i = 0; i < topologySize; ++i) {
        auto *layer = new Layer(this->topology.at(i));

        this->layers.push_back(layer);
    }

    for (unsigned int i = 0; i < (topologySize - 1); ++i) {
        auto *weightMatrix = new Matrix(topology[i + 1], topology[i], true);

        this->weightMatrices.push_back(weightMatrix);
    }

    for (int i = 0; i < this->topology.at(this->topology.size() - 1); i++) {
        errors.push_back(0.00);
        derivedErrors.push_back(0.00);
    }
}

void Network::setCurrentInput(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; ++i) {
        this->layers.at(0)->setNeuronValue(i, matrix->getValue(i, 0));
    }
}

void Network::setRecurrentInput() {
    auto output = layers.at(topologySize - 1)->convertActivatedValues();

    for (unsigned int i = topology.at(0) - topology[topologySize - 1]; i < topology.at(0); ++i) {
        this->layers.at(0)->setNeuronValue(i, output->getValue(i - topology[topologySize - 1], 0));
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

        for (int j = 0; j < r->rows; ++j) {
            this->layers.at(i + 1)->setNeuronValue(j, r->getValue(j, 0) + (this->bias));
        }

        delete r;
        delete left;
        delete right;
    }
}


void Network::setErrors(Matrix &meta) {
    if (meta.rows == 0) {
        std::cout << "invalid meta matrix" << std::endl;
        return;
    }

    unsigned int outputLayerIndex = this->layers.size() - 1;

    if (meta.rows != this->layers.at(outputLayerIndex)->getNeuronsSize()) {
        std::cout << "invalid meta matrix" << std::endl;
        return;
    }

    this->globalError = 0.0;

    vector<Neuron *> outputNeurons = this->layers.at(outputLayerIndex)->getNeurons();

    for (int i = 0; i < meta.rows; ++i) {
        double t = meta.getValue(i, 0);
        double y = outputNeurons.at(i)->getDerivedValue();

        errors.at(i) = 0.5 * pow((t - y), 2.0);
        derivedErrors.at(i) = (y - t);

        this->globalError += errors.at(i);
    }
}

void Network::backPropagation() {
    vector<Matrix *> weights;

    unsigned int indexOutputLayer = this->topologySize - 1;

    auto *gradients = new Matrix(this->topology.at(indexOutputLayer), 1, false);

    Matrix *derivedOutputValues = this->layers.at(indexOutputLayer)->convertDerivedValues();

    for (int i = 0; i < this->topology.at(indexOutputLayer); ++i) {
        double error = this->derivedErrors.at(i);
        double output = derivedOutputValues->getValue(i, 0);

        double gradient = error * output;

        gradients->setValue(i, 0, gradient);
    }

    delete derivedOutputValues;

    Matrix *lastHiddenLayerActivated = this->layers.at(indexOutputLayer - 1)->convertActivatedValues();

    Matrix *deltaWeightsLastHiddenToOutput = Matrix::multiply(*gradients, *lastHiddenLayerActivated->transpose());

    auto *tempWeights = new Matrix(
            this->topology.at(indexOutputLayer),
            this->topology.at(indexOutputLayer - 1),
            false);

    for (int i = 0; i < tempWeights->rows; ++i) {
        for (int j = 0; j < tempWeights->cols; ++j) {
            double originalValue = this->weightMatrices.at(indexOutputLayer - 1)->getValue(i, j);
            double deltaValue = deltaWeightsLastHiddenToOutput->getValue(i, j);

            deltaValue = this->learningRate * deltaValue;

            tempWeights->setValue(i, j, (originalValue - deltaValue));
        }
    }

    weights.push_back(tempWeights);

    delete lastHiddenLayerActivated;
    delete deltaWeightsLastHiddenToOutput;

    //last hidden to input

    for (int i = ((int) indexOutputLayer - 1); i > 0; --i) {
        auto *_gradients = new Matrix(*gradients);

        Matrix *transposeWeights = this->weightMatrices.at(i)->transpose();

        gradients = new Matrix(*transposeWeights->multiply(*_gradients));

        delete transposeWeights;
        delete _gradients;

        Matrix *derivedValues = this->layers.at(i)->convertDerivedValues();

        Matrix *layerGradients = Matrix::hadamard(*derivedValues, *gradients);

        delete derivedValues;

        for (int j = 0; j < layerGradients->rows; ++j) {
            for (int k = 0; k < layerGradients->cols; ++k) {
                gradients->setValue(j, k, layerGradients->getValue(j, k));
            }
        }

        delete layerGradients;

        Matrix *layerValues =
                i == 1 ? this->layers.at(0)->convertValues() : this->layers.at(i - 1)->convertActivatedValues();

        Matrix *deltaWeights = Matrix::multiply(*gradients, *layerValues->transpose());

        delete layerValues;

        auto *_tempWeights = new Matrix(this->weightMatrices.at(i - 1)->rows,
                                        this->weightMatrices.at(i - 1)->cols,
                                        false);

        for (int j = 0; j < _tempWeights->rows; ++j) {
            for (int k = 0; k < _tempWeights->cols; ++k) {
                double originalValue = this->weightMatrices.at(i - 1)->getValue(j, k);
                double deltaValue = deltaWeights->getValue(j, k);

                deltaValue = this->learningRate * deltaValue;

                _tempWeights->setValue(j, k, (originalValue - deltaValue));
            }
        }

        weights.push_back(_tempWeights);

        delete deltaWeights;
    }

    for (auto &weightMatrix : this->weightMatrices) {
        delete weightMatrix;
    }

    this->weightMatrices.clear();

    reverse(weights.begin(), weights.end());

    for (auto &weight : weights) {
        this->weightMatrices.push_back(new Matrix(*weight));
        delete weight;
    }

    //setRecurrentInput();
}

void Network::train(vector<double> input, vector<double> meta) {
    auto inputMatrix = Matrix::vectorToMatrix(std::move(input));
    auto metaMatrix = Matrix::vectorToMatrix(std::move(meta));

    setCurrentInput(inputMatrix);

    feedForward();
    setErrors(*metaMatrix);
    backPropagation();
}

vector<double> Network::predict(vector<double> input) {
    auto inputMatrix = Matrix::vectorToMatrix(std::move(input));

    setCurrentInput(inputMatrix);
    feedForward();

    auto *output_layer = this->layers.at(this->topologySize - 1)->convertActivatedValues();

    auto out = output_layer->to_vector();

    delete output_layer;
    delete inputMatrix;

    return out;
}

void Network::assign(Network &other) {
    for (int i = 0; i < this->weightMatrices.size(); ++i) {
        this->weightMatrices.at(i) = new Matrix(*other.weightMatrices.at(i));
    }
}

void Network::mutate(double rate) {
    for (auto &weightMatrix : this->weightMatrices) {
        unsigned int count = rate * weightMatrix->cols;

        unsigned int random_row = Matrix::randomInt(0, (int)weightMatrix->rows - 1);

        for (unsigned int i = 0; i < count; ++i) {
            unsigned int random_col = Matrix::randomInt(0, (int)weightMatrix->cols - 1);

            double value = weightMatrix->getValue(random_row, random_col);

            weightMatrix->setValue(random_row, random_col, value + Matrix::randomDouble(-1.0, 1.0));
        }

        //weightMatrix->map(mutateFunction);
    }
}

void Network::crossOver(Network &n, Network &father, Network &mother) {
    for (int i = 0; i < n.weightMatrices.size(); ++i) {
        Matrix *fatherWeight = father.weightMatrices.at(i);
        Matrix *motherWeight = mother.weightMatrices.at(i);

        for (int j = 0; j < fatherWeight->rows; ++j) {
            for (int k = 0; k < fatherWeight->cols; ++k) {
                if (Matrix::randomDouble(0.0, 1.0) < 0.5) {
                    n.weightMatrices.at(i)->setValue(j, k, fatherWeight->getValue(j, k));
                } else {
                    n.weightMatrices.at(i)->setValue(j, k, motherWeight->getValue(j, k));
                }
            }
        }
    }
}

void Network::save(const string& path) {
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

Network &Network::load(const string& path) {
    std::ifstream ifs(path);

    json data;

    ifs >> data;

    ifs.close();

    vector<unsigned int> topology = data["topology"];
    vector<vector<double>> weightMatrices = data["weight_matrices"];

    auto network = new Network(topology.data(), topology.size());

    for (unsigned int i = 0; i < network->weightMatrices.size(); i++) {
        auto matrix = network->weightMatrices.at(i);
        auto json_matrix = weightMatrices.at(i);

        unsigned int count = 0;
        for (unsigned int j = 0; j < matrix->rows; ++j) {
            for (unsigned int k = 0; k < matrix->cols; ++k) {
                matrix->setValue(j, k, json_matrix.at(count++));
            }
        }
    }

    return *network;
}

vector<vector<double>> Network::vectorizeWeightMatrices() {
    vector<vector<double>> data;

    for (auto &matrix : this->weightMatrices) {
        vector<double> x;

        for (unsigned int i = 0; i < matrix->rows; i++) {
            for (unsigned int j = 0; j < matrix->cols; j++) {
                x.push_back(matrix->getValue(i, j));
            }
        }

        data.push_back(x);
    }

    return data;
}

Network::~Network() {
    for (auto wm : this->weightMatrices) {
        free(wm);
    }

    for (auto ly : this->layers) {
        free(ly);
    }
}

Network *Network::clone() {
    auto n = new Network(this->topology.data(), this->topologySize);

    for (unsigned int i = 0; i < this->weightMatrices.size(); ++i) {
        n->weightMatrices.at(i) = new Matrix(*this->weightMatrices.at(i));
    }

    n->setFitness(this->getFitness());
    n->bias = this->bias;
    n->learningRate = this->learningRate;

    return n;
}

