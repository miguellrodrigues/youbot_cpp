#include "include/Test.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"

double nm(double d) {
    return atan2(sin(d), cos(d));
}

Test::Test() {
    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    double max_velocity   = 8.0;

    auto network_d = Network::load("network-dd.json");
    auto network_a = Network::load("network.json");

    while (controller.step() != -1) {
        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        auto boxPosition = new Vector(controller.getObjectPosition("box"));

        double angle_error = nm(youBotRotationAngle + youBotPosition.differenceAngle(*boxPosition));

        auto output_d = network_d.predict({youBotPosition.distance(*boxPosition) - 0.6});
        auto output_a = network_a.predict({angle_error});

        double od = max_velocity * output_d[0];

        if (output_a[0] > 0) {
            youBot.setWheelsSpeed({-max_velocity, max_velocity, -max_velocity, max_velocity});
        }

        if (output_a[1] > 0) {
            youBot.setWheelsSpeed({max_velocity, -max_velocity, max_velocity, -max_velocity});
        }

        if (output_a[2] > 0) {
            youBot.setWheelsSpeed({od, od, od, od});
        }
    }
}
