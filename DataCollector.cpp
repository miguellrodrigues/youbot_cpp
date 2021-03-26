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

    while (controller.step() != -1) {
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

        double output = pid->compute(angle_error, .16);

        youBot.setWheelsSpeed({-output, output, -output, output});

        errors.push_back(angle_error);
        outputs.push_back(output);
    }

    json data;

    data["errors"] = errors;
    data["outputs"] = outputs;

    string name = "data.json";

    std::ofstream o(name);

    o << std::setw(4) << data << endl;

    o.close();
}
