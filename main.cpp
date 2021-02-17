#include "lib/webots/controller/Controller.h"
#include "lib/webots/youbot/arm/Arm.hpp"
#include "lib/webots/youbot/base/Base.hpp"

int main() {
    Controller cont(new Supervisor(), 14);
    Arm arm(&cont);
    Base base(&cont);

    while (cont.step() != -1) {
        arm.setHeight(Arm::ARM_BACK_PLATE_HIGH);

        base.backwards();
    }
}
