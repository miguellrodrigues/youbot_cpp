//
// Created by miguel on 11/03/2021.
//

#include "include/TrainGenetic.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "lib/util/Numbers.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using std::vector;
using json = nlohmann::json;

TrainGenetic::TrainGenetic(vector<unsigned int> topology, unsigned int max_per_generation, unsigned int max_generations,
                           unsigned int time_interval) {
    assert(max_per_generation % 2 == 0);

    Controller controller(new Supervisor(), 64);
    YouBot youBot = *new YouBot(&controller);

    auto initialPosition = new Vector(youBot.getPosition());
    auto pos = new Vector(initialPosition->getX(), .05, initialPosition->getZ());

    double comps[6] = {0.01, 0.014, 0.012, 0.0078, 0.001, 0.016};

    double angle = .0,
            comp = comps[0],
            max_velocity = 8.0,
            last_time = .0,
            target_fitness = .0001;

    vector<Network *> networks;
    vector<Network *> temp;

    vector<double> generationsFitness;
    vector<double> errors;

    unsigned int count = 0,
            current = 0;

    for (unsigned int i = 0; i < max_per_generation; ++i) {
        networks.push_back(new Network(topology.data(), topology.size()));
    }

    auto network = networks.at(0);

    vector<string> logs;

    cout << "Geracao " << count << " De " << max_generations << endl;

    while (controller.step() != -1) {
        double time = controller.getSupervisor()->getTime();

        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        angle += comp;

        double x = 0.8 * cos(angle);
        double z = 0.8 * sin(angle);

        pos->add(x, .0, z);

        controller.setObjectPosition("box", pos->getValues());

        double theta = youBotPosition.differenceAngle(*pos);

        pos->subtract(x, .0, z);

        double angle_error = Numbers::normalizeAngle(youBotRotationAngle + theta);

        errors.push_back(angle_error);

        if (angle > M_PI || angle < -M_PI) {
            comp *= -1;
        }

        if (time > last_time + 1 && (int) time % time_interval == 0) {
            last_time = time;

            comp = comps[Numbers::randomInt(0, 5)];

            if (count < max_generations) {
                double sum_errors = .0;

                for (auto &err : errors) {
                    sum_errors += abs(err);
                }

                double fitness = (sum_errors / errors.size()) + initialPosition->distance(youBotPosition);
                double fitness_error = 0.5 * pow(target_fitness - fitness, 2.0);

                errors.clear();

                /*if (fitness_error <= target_fitness)
                    break;*/

                network->setFitness(fitness);

                controller.setObjectPosition("youBot", initialPosition->getValues());

                logs.push_back("Individuo " + to_string(current) + " Fitness " + to_string(fitness) + " Fit Err " +
                               to_string(fitness_error));

                cout << "Individuo " + to_string(current) + " Fitness " + to_string(fitness) + " Fit Err " +
                        to_string(fitness_error) << endl;

                current += 1;

                if (current == networks.size()) {
                    count += 1;

                    sort(networks.begin(), networks.end(), [](Network *one, Network *two) {
                        return one->getFitness() < two->getFitness();
                    });

                    double best_fitness = networks.at(0)->getFitness();

                    generationsFitness.push_back(best_fitness);

                    auto father = networks.at(0)->clone();

                    for (auto net : networks) {
                        temp.push_back(net->clone());

                        delete net;
                    }

                    for (unsigned int i = 0; i < max_per_generation; ++i) {
                        auto net = new Network(topology.data(), topology.size());

                        Network::crossOver(net, father, temp.at(Numbers::randomInt(0, temp.size() - 1)));

                        net->mutate(.2);

                        networks.at(i) = net;
                    }

                    for (auto net : temp) {
                        delete net;
                    }

                    temp.clear();

                    delete father;

                    logs.push_back("Best Fitness: " + to_string(best_fitness));

                    cout << "Best Fitness: " << best_fitness << endl;

                    logs.push_back("Geracao " + to_string(count) + " De " + to_string(max_generations));

                    cout << "Geracao " << count << " De " << max_generations << endl;

                    current = 0;
                }

                network = networks.at(current);
            }
        }

        auto output = network->predict({abs(angle_error) / M_PI, angle_error > 0 ? 1.0 : .0});

        double s = output.at(0) * 2 * M_PI;

        youBot.setWheelsSpeed({-s, s, -s, s});

        output.clear();
    }

    network->save("align.json");

    std::time_t timeStamp = std::time(nullptr);

    json training_data;

    training_data["topology"] = topology;
    training_data["bias"] = network->getBias();
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
