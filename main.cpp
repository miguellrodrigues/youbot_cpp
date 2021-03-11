#include <algorithm>
#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"

using namespace std;

double normalize(double d) {
    return atan2(sin(d), cos(d));
}

int main() {
    vector<unsigned int> topology = {1, 8, 16, 16, 8, 1};

    Controller controller(new Supervisor(), 50);
    YouBot youBot(&controller);

    auto center = youBot.getPosition();
    auto initialPosition = center;

    double angle          = .0,
            comp           = .005,
            last_time      = .0,
            target_fitness = .001;

    vector<Network *> networks;
    vector<double> errors;

    unsigned int max_per_generation = 10,
            max_generations    = 1000,
            count              = 0,
            current            = 0,
            time_interval      = 10;

    for (unsigned int i = 0; i < max_per_generation; ++i) {
        networks.push_back(new Network(topology.data(), topology.size()));
    }

    auto network = networks.at(0);

    cout << "Geracao " << count << " De " << max_generations << endl;

    Network::load();

    while (controller.step() != -1) {
        double time = controller.getSupervisor()->getTime();

        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        angle += comp;

        double x = 0.8 * cos(angle);
        double z = 0.8 * sin(angle);

        center.add(Vector(x, .0, z));

        controller.setObjectPosition("box", center.getValues());

        double theta = youBotPosition.differenceAngle(center);

        center.subtract(Vector(x, .0, z));

        double angle_error = normalize(youBotRotationAngle + theta);

        errors.push_back(angle_error);

        if (time > last_time + 1 && (int) time % time_interval == 0) {
            last_time = time;

            if (count < max_generations) {
                double sum_errors = .0;

                for (auto &err : errors) {
                    sum_errors += abs(err);
                }

                double fitness = (sum_errors / errors.size()) + initialPosition.distance(youBotPosition);
                double fitness_error = 0.5 * pow(target_fitness - fitness, 2.0);

                errors.clear();

                if (fitness_error <= target_fitness)
                    break;

                network->setFitness(fitness);

                controller.setObjectPosition("youBot", initialPosition.getValues());

                cout << "Individuo " << current << " Fitness " << network->getFitness() << " Fit Err " << fitness_error << endl;

                current += 1;

                if (current == networks.size()) {
                    count += 1;

                    sort(networks.begin(), networks.end(), [](Network *one, Network *two) {
                        return one->getFitness() < two->getFitness();
                    });

                    double best_fitness = networks[0]->getFitness();

                    auto father = networks.at(0);
                    auto mother = networks.at(1);

                    networks.clear();

                    for (unsigned int i = 0; i < max_per_generation; ++i) {
                        auto net = new Network(topology.data(), topology.size());

                        net->crossOver(*father, *mother);

                        net->mutate(.3);

                        networks.push_back(net);
                    }

                    cout << "Best Fitness: " << best_fitness << endl;

                    cout << "Geracao " << count << " De " << max_generations << endl;

                    current = 0;
                }

                network = networks.at(current);
            }
        }

        auto output = network->predict({angle_error});

        double speed = output[0] * 12;

        youBot.setWheelsSpeed({-speed, speed, -speed, speed});
    }

    return 0;
}


