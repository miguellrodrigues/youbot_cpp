//
// Created by miguel on 30/01/2021.
//

#ifndef YOUBOT_CPP_NETWORK_HPP
#define YOUBOT_CPP_NETWORK_HPP

#include <vector>
#include <string>
#include "../layer/Layer.hpp"

using std::vector;
using std::string;

class Network {
public:
    Network(unsigned int *topology, unsigned int topologySize);

    void train(vector<double> input, vector<double> meta);

    double *predict(vector<double> input);

    double globalError = 0.0;

    double getFitness() const { return this->fitness; }

    void setFitness(double fit) { this->fitness = fit; }

    void mutate(double rate);

    void assign(Network &other);

    void crossOver(Network &father, Network &mother);

    void save(const string& path);

    static Network &load(const string& path);

    vector<Matrix *> weightMatrices;

    double bias = 0.005;
private:
    double learningRate = .0005;

    double fitness = .0;

    unsigned int topologySize{};
    vector<unsigned int> topology;

    vector<Layer *> layers;

    vector<double> derivedErrors;
    vector<double> errors;

    void setCurrentInput(Matrix &matrix);

    void feedForward();

    void setErrors(Matrix &meta);

    void backPropagation();

    void setRecurrentInput();

    vector<vector<double>> vectorizeWeightMatrices();
};


#endif
