#include "include/Test.hpp"

#include "lib/webots/youbot/YouBot.hpp"
#include "lib/neural_network/network/Network.hpp"
#include "lib/util/Numbers.hpp"

Test::Test() {
    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    double M   = 3.0;

    auto network_a = Network::load("align.json");

    while (controller.step() != -1) {
        auto youBotPosition = youBot.getPosition();
        auto youBotRotationAngle = youBot.getRotationAngle();

        auto boxPosition = new Vector(controller.getObjectPosition("box"));

        double angle_error = Numbers::normalizeAngle(youBotRotationAngle + youBotPosition.differenceAngle(*boxPosition));

        /*auto output = network_a.predict({abs(angle_error), angle_error > 0 ? 1.0 : .0});

        if (output.at(0) > 0) {
            youBot.setWheelsSpeed({-s, s, -s, s});
        }

        if (output.at(1) > 0) {
            youBot.setWheelsSpeed({s, -s, s, -s});
        }

        output.clear()*/

        auto output = network_a.predict({abs(angle_error) / M_PI, angle_error > 0 ? 1.0 : .0});

        double s = output.at(0) * 2 * M_PI;
        cout << s << endl;

        youBot.setWheelsSpeed({-s, s, -s, s});

        output.clear();
    }
}
