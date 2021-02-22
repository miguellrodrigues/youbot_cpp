#include "lib/neural_network/network/Network.hpp"
#include "lib/webots/controller/Controller.hpp"
#include "lib/webots/youbot/YouBot.hpp"
#include "lib/util/pid/Pid.hpp"

using namespace std;

double normalize(double r) {
    return atan2(sin(r), cos(r));
}

int main() {
    unsigned int topology[5] = { 1, 16, 32, 64, 1 };

    //Network network(topology, 5);
    Network network = Network::load();

    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    Pid anglePid(8.0, .001, 2.6, 5.0, 0.1);

    while (controller.step() != -1) {
        double time = controller.getSupervisor()->getTime();

        double youBotAngle = youBot.getRotationAngle();

        Vector bp = Vector(controller.getObjectPosition("box"));

        double angleError = normalize(youBotAngle + youBot.getPosition().differenceAngle(bp));

        double out = network.predict({angleError})[0];

        cout << out << endl;

        out *= 6;

        youBot.setWheelsSpeed({-out, out, -out, out});

        /*if (time > 0 && time < 15) {
            double out = anglePid.compute(angleError, .05);

            youBot.setWheelsSpeed({-out, out, -out, out});

            network.train({angleError}, {out});
        } else {
            double out = network.predict({angleError})[0];

            cout << out << endl;

            out *= 6;

            youBot.setWheelsSpeed({-out, out, -out, out});
        }*/
    }

    //network.save();
}
