#include "iostream"
#include "lib/neural_network/network/Network.hpp"
#include "lib/webots/controller/Controller.h"
#include "lib/webots/youbot/YouBot.hpp"
#include "lib/util/angle/Angle.hpp"

using namespace std;

double normalize(double r) {
    return atan2(sin(r), cos(r));
}

int main() {
    /*unsigned int topology[4] = { 2, 20, 30, 1 };

    Network network(topology, 4);

    double datasetInput[4][2] = { {0, 0}, {0, 1}, {1, 0}, {1, 1}};

    double datasetOutput[4][1] = { {1}, {0}, {1}, {1} };

    for (int i = 0; i < 10000; ++i) {
        for (int j = 0; j < 4; ++j) {
            network.train(Matrix::arrayToMatrix(datasetInput[j], 2), Matrix::arrayToMatrix(datasetOutput[j], 1));

            cout << " Global Error: " << network.globalError << endl;
        }
    }

    for (int i = 0; i < 4; i++) {
        cout << "Target " << datasetOutput[i][0] << " Output " << network.predict(Matrix::arrayToMatrix(datasetInput[i], 2))[0] << endl;
    }

    return 0;*/

    Controller controller(new Supervisor(), 14);
    YouBot youBot(&controller);

    while (controller.step() != -1) {
        double youBotAngle = youBot.getRotationAngle();
        double boxOrientation = controller.getObjectRotation("box")[3];

        double angleError = normalize((boxOrientation - youBotAngle));

        if (angleError > 0) {
            youBot.turnLeft();
        } else {
            youBot.turnRight();
        }
    }
}
