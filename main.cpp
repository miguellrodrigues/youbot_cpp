#include "include/Train.hpp"
#include "include/Test.hpp"
#include "lib/util/matrix/Matrix.hpp"
#include <iostream>

using namespace std;

int main() {

    new Train({2, 16, 16, 16, 3}, 5, 200, 60);
    //new Test();

    /*auto a = new Matrix(3, 2, false);
    auto b = new Matrix(2, 3, false);

    int aa[6] = {1, 2, 3, 4, 5, 6};
    int bb[6] = {6, 5, 4, 3, 2, 1};

    int count = 0;

    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 2; ++j) {
            a->setValue(i, j, aa[count++]);
        }
    }

    count = 0;

    for (unsigned int i = 0; i < 2; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            b->setValue(i, j, bb[count++]);
        }
    }

    b->transpose()->printToConsole();*/

    return 0;
}


