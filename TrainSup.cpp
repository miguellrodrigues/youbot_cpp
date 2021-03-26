//
// Created by miguel on 26/03/2021.
//

#include "include/TrainSup.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "lib/util/Numbers.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

using std::vector;
using json = nlohmann::json;

TrainSup::TrainSup() {
    std::ifstream ifs("data.json");

    json data;

    ifs >> data;

    ifs.close();

    vector<double> errors = data["errors"];
    vector<double> outputs = data["outputs"];

    vector<unsigned int> topology = {1, 16, 16, 16, 1};

    auto network = new Network(topology.data(), topology.size());

    double error = 10.0;

    while (error > 0.1) {
        for (unsigned int i = 0; i < errors.size(); ++i) {
            network->train({errors[i]}, {outputs[i]});
        }

        error = network->getGlobalError();

        cout << error << endl;
    }

    network->save("trained.json");
}
