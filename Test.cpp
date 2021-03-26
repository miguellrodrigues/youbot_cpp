#include "include/Test.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "lib/util/Numbers.hpp"

Test::Test() {
    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    double M   = 3.0;

    auto network_a = Network::load("ttrained.json");

    while (controller.step() != -1) {
        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        auto boxPosition = new Vector(controller.getObjectPosition("box"));

        double angle_error = Numbers::normalizeAngle(youBotRotationAngle + youBotPosition.differenceAngle(*boxPosition));

        auto _a = Numbers::map(angle_error, -M_PI, M_PI, .0, 1.0);

        auto out = network_a.predict({_a, angle_error > 0 ? 1.0 : .0});

        double s = Numbers::map(out[0], .0, 1.0, -5, 5);;

        youBot.setWheelsSpeed({-s, s, -s, s});
    }
}
