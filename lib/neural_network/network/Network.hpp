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

    ~Network();

    Network *clone();

    void train(vector<double> input, vector<double> meta);

    vector<double> predict(vector<double> input);

    void mutate(double rate);

    static void crossOver(Network *net, Network *father, Network *mother);

    void assign(Network &other);

    void save(const string& path);

    static Network &load(const string& path);

    double getFitness() const { return this->fitness; }

    void setFitness(double fit) { this->fitness = fit; }

    double getGlobalError() const
    {
        return this->globalError;
    }

    double getBias() const
    {
        return this->bias;
    }

    const vector<Matrix *> &getWeightMatrices() const
    {
        return this->weightMatrices;
    }

private:
    double learningRate = .00001;

    double fitness = .0;

    double globalError = 0.0;

    double bias = .01;

    unsigned int topologySize{};

    vector<unsigned int> topology;

    vector<Layer *> layers;
    vector<Matrix *> weightMatrices;
    vector<Matrix *> temp_weights;

    vector<double> derivedErrors;
    vector<double> errors;

    vector<vector<double>> vectorizeWeightMatrices();

    void setCurrentInput(Matrix *matrix);

    void feedForward();

    void setErrors(Matrix &meta);

    void backPropagation();

    void setRecurrentInput();
};


#endif
