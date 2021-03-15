//
// Created by miguellr on 03/12/2020.
//

#include "Vector.hpp"
#include <cmath>

namespace lib
{
    Vector::Vector(double x, double y, double z): x(x), y(y), z(z) {
        this->data = vector<double>();

        this->data.push_back(x);
        this->data.push_back(y);
        this->data.push_back(z);
    }

    Vector::Vector(const double *values) : Vector(values[0], values[1], values[2]) {
    }

    double Vector::length() const {
        return sqrt(this->lengthSquared());
    }

    double Vector::lengthSquared() const {
        return (pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
    }

    double Vector::distance(const Vector& other) const {
        return hypot(this->x - other.x, this->z - other.z);
    }

    double Vector::distanceSquared(const Vector& other) const {
        return pow(distance(other), 2.0);
    }

    double Vector::angle(const Vector& other) const {
        double dot = this->dot(other) / (this->length() * other.length());
        return acos(dot);
    }

    double Vector::differenceAngle(const Vector& other) const {
        return atan2(other.z - this->z, other.x - this->x);
    }

    double Vector::dot(const Vector& other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    void Vector::scalar(double value) {
        this->x *= value;
        this->y *= value;
        this->z *= value;
    }

    void Vector::add(const Vector& other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
    }

    void Vector::subtract(const Vector& other) {
        this->x -= other.x;
        this->y -= other.y;
        this->z -= other.z;
    }

    void Vector::multiply(Vector other) {
        this->x *= other.x;
        this->y *= other.y;
        this->z *= other.z;
    }

    void Vector::update(const double *d) {
        this->x = d[0];
        this->y = d[1];
        this->z = d[2];
    }

    Vector *Vector::midPoint(const Vector& other) {
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

    Vector::~Vector() {
        this->data.clear();
    }
}

