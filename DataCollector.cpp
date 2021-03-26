//
// Created by miguel on 26/03/2021.
//

#include "include/DataCollector.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "lib/util/Numbers.hpp"
#include "lib/util/pid/Pid.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <iomanip>

using std::vector;
using json = nlohmann::json;

DataCollector::DataCollector() {
    Controller controller(new Supervisor(), 16);
    YouBot youBot = *new YouBot(&controller);

    auto pid = new Pid(50.0, .8, .6, 10.0, 1.57);

    double comps[6] = {0.01, 0.014, 0.012, 0.0078, 0.001, 0.016};

    double angle = .0,
            comp = .01;

    auto initialPosition = new Vector(youBot.getPosition());
    auto pos = new Vector(initialPosition->getX(), .05, initialPosition->getZ());

    vector<double> errors;
    vector<double> outputs;
    vector<double> sig;

    vector<unsigned int> topology = {2, 16, 32, 16, 1};

    auto net = new Network(topology.data(), topology.size());

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

        if (angle > M_PI || angle < -M_PI) {
            comp *= -1;
        }

        double angle_error = Numbers::normalizeAngle(youBotRotationAngle + theta);

        if (time > 20) {
             auto _a = Numbers::map(angle_error, -M_PI, M_PI, .0, 1.0);

             auto out = net->predict({_a, angle_error > 0 ? 1.0 : .0});

             double s = Numbers::map(out[0], .0, 1.0, -10, 10);;

             youBot.setWheelsSpeed({-s, s, -s, s});
        } else {
            double output = pid->compute(angle_error, .16);

            youBot.setWheelsSpeed({-output, output, -output, output});

            auto _a = Numbers::map(angle_error, -M_PI, M_PI, .0, 1.0);
            double _o = Numbers::map(output, -10, 10, .0, 1.0);

            net->train({_a, angle_error > 0 ? 1.0 : .0}, {_o});
        }
    }

    json data;

    data["errors"] = errors;
    data["sig"] = sig;
    data["outputs"] = outputs;

    string name = "data.json";

    std::ofstream o(name);

    o << std::setw(4) << data << endl;

    o.close();

    net->save("ttrained.json");
}
