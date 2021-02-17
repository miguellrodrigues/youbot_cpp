//
// Created by miguel on 31/01/2021.
//

#ifndef REDENEURAL_NEURON_HPP
#define REDENEURAL_NEURON_HPP

class Neuron {
public:
    explicit Neuron(double val);

    void setValue(double val);

    void activate();

    void derive();

    double getValue() const;

    double getActivatedValue() const;

    double getDerivedValue() const;

private:
    double value{};

    double activatedValue{};
    double derivedValue{};
};


#endif //REDENEURAL_NEURON_HPP
