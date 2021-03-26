//
// Created by miguel on 11/03/2021.
//

#ifndef YOUBOT_CPP_TRAINGENETIC_HPP
#define YOUBOT_CPP_TRAINGENETIC_HPP


#include <vector>

using std::vector;

class TrainGenetic {

public:
    explicit TrainGenetic(vector<unsigned int> topology, unsigned int max_per_generation, unsigned int max_generations, unsigned int time_interval);
};


#endif //YOUBOT_CPP_TRAINGENETIC_HPP
