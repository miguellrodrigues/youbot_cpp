//
// Created by miguel on 15/03/2021.
//

#ifndef YOUBOT_CPP_NUMBERS_HPP
#define YOUBOT_CPP_NUMBERS_HPP

#include <cmath>
#include <random>
#include <cstdlib>

using namespace std;

class Numbers {
public:

    static double randomDouble(double min, double max)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(min, max);

        return (dis(gen));
    }

    static int randomInt(int min, int max)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(min, max);

        return (dis(gen));
    }

    static double normalizeAngle(double r)
    {
        return atan2(sin(r), cos(r));
    }

    static double sigmoid(double x)
    {
        return (x / (1 + std::abs(x)));
    }

    static double derivative(double x)
    {
        return (1 / pow((1 + std::abs(x)), 2.0));
    }
};

#endif //YOUBOT_CPP_NUMBERS_HPP
