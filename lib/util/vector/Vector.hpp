//
// Created by miguellr on 06/12/2020.
//

#ifndef YOUBOT_CPP_VECTOR_HPP
#define YOUBOT_CPP_VECTOR_HPP

#include <vector>
#include <cstdlib>

using std::vector;

namespace lib {
    class Vector {
    public:
        Vector(double x, double y, double z);

        explicit Vector(const double *values);

        double length() const;

        double lengthSquared() const;

        double distance(const Vector& other) const;

        double distanceSquared(const Vector& other) const;

        double angle(const Vector& other);

        double differenceAngle(const Vector& other) const;

        double dot(const Vector& other) const;

        void scalar(double value);

        void add(const Vector& other);

        void subtract(const Vector& other);

        void multiply(Vector other);

        void update(const double *d);

        double getX() { return this->x; }

        double getY() { return this->y; }

        double getZ() { return this->z; }

        Vector *midPoint(const Vector& other);

        Vector clone() const;

        double* getValues() {
            this->data[0] = this->x;
            this->data[1] = this->y;
            this->data[2] = this->z;

            return this->data;
        }

        void setX(double d);

    private:
        double *data;

        double x;
        double y;
        double z;
    };
}


#endif //CINEMATICCONTROLLER_VECTOR_H
