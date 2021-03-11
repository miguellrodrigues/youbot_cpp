//
// Created by miguel on 11/03/2021.
//

#ifndef YOUBOT_CPP_TRAIN_HPP
#define YOUBOT_CPP_TRAIN_HPP


#include <vector>

using std::vector;

class Train {

public:
    explicit Train(vector<unsigned int> topology, unsigned int max_per_generation, unsigned int max_generations, unsigned int time_interval);
};


#endif //YOUBOT_CPP_TRAIN_HPP
