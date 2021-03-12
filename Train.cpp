//
// Created by miguel on 11/03/2021.
//

#include "include/Train.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "nlohmann/json.hpp"
#include "lib/util/pid/Pid.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <vector>

using std::vector;

double normalize(double d) {
    return atan2(sin(d), cos(d));
}

using json = nlohmann::json;


Train::Train(vector<unsigned int> topology, unsigned int max_per_generation, unsigned int max_generations, unsigned int time_interval) {
    Controller controller(new Supervisor(), 16);
    YouBot youBot(&controller);

    auto center = youBot.getPosition();
    auto initialPosition = center;

    double  angle          = .0,
            inc           = .4,
            max_velocity   = 4.0,
            last_time      = .0,
            target_fitness = .0;

    double x_min = -1.0859, x_max = -0.558922;

    vector<Network *> networks;
    vector<double> generationsFitness;
    vector<double> errors;

    unsigned int count              = 0,
                 current            = 0;

    for (unsigned int i = 0; i < max_per_generation; ++i) {
        networks.push_back(new Network(topology.data(), topology.size()));
    }

    auto network = networks.at(0);

    vector<string> logs;

    cout << "Geracao " << count << " De " << max_generations << endl;

    double initialRotation[4] = {-0.999891, -0.0093612, -0.0113853, 1.57093};

    auto pid = new Pid(4.0, .001, 1.0, 5.0, .01);

    while (controller.step() != -1) {
        double time = controller.getSupervisor()->getTime();

        auto youBotPosition = youBot.getPosition();

        auto boxPosition = Vector(controller.getObjectPosition("box"));

        /*if (time > last_time + 1 && (int) time % time_interval == 0) {
            last_time = time;

            if (count < max_generations) {
                double fitness = abs(abs(youBotPosition.distance(boxPosition)) - .6);
                double fitness_error = 0.5 * pow(target_fitness - fitness, 2.0);

                errors.clear();

                if (fitness_error <= target_fitness)
                    break;

                network->setFitness(fitness);

                controller.setObjectPosition("youBot", initialPosition.getValues());
                controller.setObjectRotation("youBot", initialRotation);

                logs.push_back("Individuo " + to_string(current) + " Fitness " + to_string(fitness) +  " Fit Err " + to_string(fitness_error));

                cout << logs.at(logs.size() - 1) << endl;

                current += 1;

                if (current == networks.size()) {
                    count += 1;

                    sort(networks.begin(), networks.end(), [](Network *one, Network *two) {
                        return one->getFitness() < two->getFitness();
                    });

                    double best_fitness = networks[0]->getFitness();

                    generationsFitness.push_back(best_fitness);

                    auto father = networks.at(0);
                    auto mother = networks.at(1);

                    networks.clear();

                    for (unsigned int i = 0; i < max_per_generation; ++i) {
                        auto net = new Network(topology.data(), topology.size());

                        net->crossOver(*father, *mother);

                        net->mutate(.3);

                        networks.push_back(net);
                    }

                    logs.push_back("Best Fitness: " + to_string(best_fitness));

                    cout << logs.at(logs.size() - 1) << endl;

                    logs.push_back("Geracao " + to_string(count) + " De " + to_string(max_generations));

                    cout << logs.at(logs.size() - 1) << endl;

                    current = 0;
                }

                network = networks.at(current);
            }
        }*/

        /*auto output = network->predict({abs(youBotPosition.distance(boxPosition)) - .6});

        if (output[0] > 0) {
            youBot.setWheelsSpeed({max_velocity, max_velocity, max_velocity, max_velocity});
        }

        if (output[1] > 0) {
            youBot.setWheelsSpeed({-max_velocity, -max_velocity, -max_velocity, -max_velocity});
        }

        if (output[2] > 0) {
            youBot.setWheelsSpeed({.0, .0, .0, .0});
        }*/

        double out = pid->compute(youBotPosition.distance(boxPosition) - .6, 0.14);

        youBot.setWheelsSpeed({out, out, out, out});

        network->train({youBotPosition.distance(boxPosition) - .6}, {out});
    }

    network->save("");

    std::time_t timeStamp = std::time(nullptr);

    json training_data;

    training_data["topology"] = topology;
    training_data["bias"] = network->bias;
    training_data["time_stamp"] = timeStamp;
    training_data["generations_fitness"] = generationsFitness;
    training_data["logs"] = logs;

    string name = "training_";
    name.append(to_string(timeStamp));
    name.append(".json");

    std::ofstream o(name);

    o << std::setw(4) << training_data << endl;

    o.close();
}
