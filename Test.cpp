#include "include/Test.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"

double nm(double d) {
    return atan2(sin(d), cos(d));
}

Test::Test() {
    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    double max_velocity   = 10.0;

    auto network = Network::load();

    while (controller.step() != -1) {
        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        auto boxPosition = new Vector(controller.getObjectPosition("box"));

        double theta = youBotPosition.differenceAngle(*boxPosition);

        double angle_error = nm(youBotRotationAngle + theta);

        auto output = network.predict({angle_error});

        cout << angle_error << endl;

        if (output[0] > 0) {
            youBot.setWheelsSpeed({-max_velocity, max_velocity, -max_velocity, max_velocity});
        }

        if (output[1] > 0) {
            youBot.setWheelsSpeed({0, 0, 0, 0});
        }

        if (output[2] > 0) {
            youBot.setWheelsSpeed({max_velocity, -max_velocity, max_velocity, -max_velocity});
        }
    }
}
