//
// Created by miguel on 11/03/2021.
//

#ifndef YOUBOT_CPP_MATUTIL_CUH
#define YOUBOT_CPP_MATUTIL_CUH


class MatUtil {
public:
    static void matrixMultiply(double *a, double *b, double *c, int m, int n, int k);

    static void matrixTranspose(double *a, double *b, unsigned int rows, unsigned int cols);
};


#endif //YOUBOT_CPP_MATUTIL_CUH
