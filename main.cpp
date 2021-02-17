#include "lib/webots/controller/Controller.h"
#include "lib/webots/youbot/arm/Arm.hpp"
#include "lib/webots/youbot/base/Base.hpp"
#include "lib/webots/youbot/YouBot.hpp"

int main() {
    Controller cont(new Supervisor(), 14);
    YouBot yb(&cont);

    while (cont.step() != -1) {

    }
}
