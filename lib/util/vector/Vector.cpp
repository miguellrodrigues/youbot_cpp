//
// Created by miguellr on 03/12/2020.
//

#include "Vector.hpp"
#include <cmath>

namespace lib
{
    Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {
    }

    Vector::Vector(const double *values) : Vector(values[0], values[1], values[2]) {
    }

    double Vector::length() {
        return sqrt(this->lengthSquared());
    }

    double Vector::lengthSquared() const {
        return (pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
    }

    double Vector::distance(Vector other) const {
        return hypot(this->x - other.x, this->z - other.z);
    }

    double Vector::distanceSquared(Vector other) const {
        return pow(distance(other), 2.0);
    }

    double Vector::angle(Vector other) {
        double dot = this->dot(other) / (this->length() * other.length());
        return acos(dot);
    }

    double Vector::differenceAngle(Vector other) const {
        return atan2(other.z - this->z, other.x - this->x);
    }

    double Vector::dot(Vector other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    void Vector::scalar(double value) {
        this->x *= value;
        this->y *= value;
        this->z *= value;
    }

    void Vector::add(Vector other) {
        this->x += other.x;
        this->y += other.y;
    }

    void Vector::subtract(Vector other) {
        this->x -= other.x;
        this->y -= other.y;
    }

    void Vector::multiply(Vector other) {
        this->x *= other.x;
        this->y *= other.y;
    }

    void Vector::update(const double *values) {
        this->x = values[0];
        this->y = values[2];
    }

    Vector *Vector::midPoint(Vector other) {
        auto vector = new Vector(*this);

        vector->x = (this->x + other.x) / 2.0;
        vector->y = (this->y + other.y) / 2.0;
        vector->z = (this->z + other.z) / 2.0;

        return vector;
    }

    Vector Vector::clone() const {
        Vector copy(this->x, this->y, this->z);

        return copy;
    }
}

