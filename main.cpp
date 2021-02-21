#include "lib/neural_network/network/Network.hpp"
#include "lib/webots/controller/Controller.hpp"
#include "lib/webots/youbot/YouBot.hpp"
#include "lib/util/pid/Pid.hpp"

using namespace std;

double normalize(double r) {
    return atan2(sin(r), cos(r));
}

int main() {
    unsigned int topology[5] = { 2, 16, 32, 64, 2 };

    Network network(topology, 5);

    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    Pid anglePid(8.0, .001, 2.6, 5.0, 0.1);
    Pid distancePid(4.0, .01, 1.6, 5.0, 0.1);

    double starttime, endtime = .0;

    while (controller.step() != -1) {
        double time = controller.getSupervisor()->getTime();

        double youBotAngle = youBot.getRotationAngle();

        Vector bp = Vector(controller.getObjectPosition("box"));

        double angleError = normalize(youBotAngle + youBot.getPosition().differenceAngle(bp));
        double distanceError = youBot.getPosition().distance(bp);

        if (time > 0 && time < 15) {
            double out = anglePid.compute(angleError, .05);
            double dOut = distancePid.compute(distanceError, .05);

            double x = (out - dOut);
            double y = (out + dOut);

            youBot.setWheelsSpeed({-x, y, -x, y});

            network.train({angleError, distanceError}, {out, dOut});
        } else {
            double *out = network.predict({angleError, distanceError});

            double a = out[0] * 6;
            double b = out[1] * 6;

            double x = (a - b);
            double y = (a + b);

            youBot.setWheelsSpeed({-x, y, -x, y});
        }
    }

    network.save();
}
