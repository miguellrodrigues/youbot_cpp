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

    vector<double> sig = data["sig"];

    vector<unsigned int> topology = {2, 16, 32, 16, 1};

    auto network = new Network(topology.data(), topology.size());

    for (unsigned int j = 0; j < errors.size(); j++) {
        network->train({errors[j], sig[j]}, {outputs[j]});

        cout << network->getGlobalError() << endl;
    }

    network->save("trained.json");
}
