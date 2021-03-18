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

void Network::mutate(double rate) {
    for (auto &weightMatrix : this->weightMatrices) {
        unsigned int count = rate * weightMatrix->getCols();

        unsigned int random_row = Numbers::randomInt(0, (int) weightMatrix->getRows() - 1);

        for (unsigned int i = 0; i < count; ++i) {
            unsigned int random_col = Numbers::randomInt(0, (int) weightMatrix->getCols() - 1);

            double value = weightMatrix->getValue(random_row, random_col);

            weightMatrix->setValue(random_row, random_col, value + Numbers::randomDouble(-1.0, 1.0));
        }
    }
}

vector<Network *> Network::crossOver(Network &father, Network &mother) {
    vector<Network *> child;

    child.push_back(new Network(father.topology.data(), father.topology.size()));
    child.push_back(new Network(mother.topology.data(), mother.topology.size()));

    for (int n = 0; n < child.size(); ++n) {
        auto net = child.at(n);

        for (int i = 0; i < net->getWeightMatrices().size(); i++) {
            vector<unsigned int> mask;

            Matrix *fatherWeight = father.weightMatrices.at(i);
            Matrix *motherWeight = mother.weightMatrices.at(i);

            for (int j = 0; j < fatherWeight->getRows(); ++j) {
                for (int k = 0; k < fatherWeight->getCols(); ++k) {
                    if (Numbers::randomInt(0, 1) == 0) {
                        if (n == 0) {
                            net->weightMatrices.at(i)->setValue(j, k, fatherWeight->getValue(j, k));
                        } else {
                            net->weightMatrices.at(i)->setValue(j, k, motherWeight->getValue(j, k));
                        }
                    } else {
                        if (n == 0) {
                            net->weightMatrices.at(i)->setValue(j, k, motherWeight->getValue(j, k));
                        } else {
                            net->weightMatrices.at(i)->setValue(j, k, fatherWeight->getValue(j, k));
                        }
                    }
                }
            }
        }
    }

    return child;
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

    auto network = new Network(topology.data(), topology.size());

    for (unsigned int i = 0; i < network->weightMatrices.size(); i++) {
        auto matrix = network->weightMatrices.at(i);
        auto json_matrix = weightMatrices.at(i);

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

    for (auto &matrix : this->weightMatrices) {
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

vector<vector<vector<double>>> Network::slice(const vector<Matrix *>& matrices) {
    vector<vector<vector<double>>> data;

    for (auto matrix : matrices) {
        vector<vector<double>> slices;

        unsigned int size = matrix->size();

        auto matrix_vector = matrix->to_vector();

        unsigned int point_1 = Numbers::randomInt(1, (int)size - 1);
        unsigned int point_2 = Numbers::randomInt((int)point_1, (int)size - 1);

        if (point_1 == point_2)
            point_2 += 1;

        vector<double> slice_x;
        vector<double> slice_y;
        vector<double> slice_z;

        for (unsigned int i = 0; i < point_1; ++i) {
            slice_x.push_back(matrix_vector.at(i));
        }

        for (unsigned int i = point_1; i < point_2; ++i) {
            slice_y.push_back(matrix_vector.at(i));
        }

        for (unsigned int i = point_2; i < size; ++i) {
            slice_z.push_back(matrix_vector.at(i));
        }

        slices.push_back(slice_x);
        slices.push_back(slice_y);
        slices.push_back(slice_z);

        data.push_back(slices);
    }

    return data;
}

vector<Matrix *> Network::combine(const vector<vector<vector<double>>>& data, unsigned int rows, unsigned int cols)
{
    auto father_slices = data.at(0);
    auto mother_slices = data.at(1);

    vector<vector<Matrix *>> d;

    vector<double> c1;
    vector<double> c2;

    auto *m1 = new Matrix(rows, cols, false);
    auto *m2 = new Matrix(rows, cols, false);

    for (double & i : father_slices.at(0)) {
        c1.push_back(i);
    }

    for (double & i : mother_slices.at(1)) {
        c1.push_back(i);
    }

    for (double & i : father_slices.at(2)) {
        c1.push_back(i);
    }

    for (double & i : mother_slices.at(0)) {
        c2.push_back(i);
    }

    for (double & i : father_slices.at(1)) {
        c2.push_back(i);
    }

    for (double & i : mother_slices.at(2)) {
        c2.push_back(i);
    }

    for (int i = 0, c = 0; i < rows; i++) {
        for (int j = 0; j < cols; ++j) {
            m1->setValue(i, j, c1.at(c++));
        }
    }

    return {m1, m2};
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

    Matrix *deltaWeightsLastHiddenToOutput = gradients->multiply(*lastHiddenLayerActivated->transpose());

    auto *tempWeights = new Matrix(
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

        Matrix *layerGradients = derivedValues->hadamard(*gradients);

        delete derivedValues;

        for (int j = 0; j < layerGradients->getRows(); ++j) {
            for (int k = 0; k < layerGradients->getCols(); ++k) {
                gradients->setValue(j, k, layerGradients->getValue(j, k));
            }
        }

        delete layerGradients;

        Matrix *layerValues =
                i == 1 ? this->layers.at(0)->convertValues() : this->layers.at(i - 1)->convertActivatedValues();

        Matrix *deltaWeights = (gradients->multiply(*layerValues->transpose()));

        delete layerValues;

        auto *_tempWeights = new Matrix(this->weightMatrices.at(i - 1)->getRows(),
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

void Network::setRecurrentInput() {
    auto output = layers.at(topologySize - 1)->convertActivatedValues();

    for (unsigned int i = topology.at(0) - topology[topologySize - 1]; i < topology.at(0); ++i) {
        this->layers.at(0)->setNeuronValue(i, output->getValue(i - topology[topologySize - 1], 0));
    }
}

Network::~Network() {
    for (auto wm : this->weightMatrices) {
        delete wm;
    }

    for (auto ly : this->layers) {
        delete ly;
    }

    this->layers.clear();
    this->weightMatrices.clear();

    this->errors.clear();
    this->derivedErrors.clear();
}