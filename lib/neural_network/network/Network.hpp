//
// Created by miguel on 30/01/2021.
//

#ifndef REDENEURAL_NETWORK_HPP
#define REDENEURAL_NETWORK_HPP

#include <vector>
#include "../matrix/Matrix.hpp"
#include "../layer/Layer.hpp"

using std::vector;

class Network {
public:
    Network(unsigned int *topology, unsigned int topologySize);

    void train(Matrix &input, Matrix &meta);

    double *predict(Matrix &input);

    double globalError = 0.0;

    double getFitness() const { return this->fitness; }

    void setFitness(double fit) { this->fitness = fit; }

    void mutate(double rate);

    void assign(Network &other);

    void crossOver(Network &father, Network &mother);

    void save();

    static Network *load();

    vector<Matrix *> weightMatrices;

private:
    double learningRate = 0.05;
    double bias = 0.1;

    double fitness{};

    unsigned int topologySize{};
    vector<unsigned int> topology;

    vector<Layer *> layers;

    vector<double> derivedErrors;
    vector<double> errors;

    void setCurrentInput(Matrix matrix);

    void feedForward();

    void setErrors(Matrix &meta);

    void backPropagation();
};


#endif //REDENEURAL_NETWORK_HPP
