//
// Created by miguellr on 06/12/2020.
//

#ifndef YOUBOT_CPP_VECTOR_HPP
#define YOUBOT_CPP_VECTOR_HPP

namespace lib {
    class Vector {
    public:
        Vector(double x, double y, double z);

        explicit Vector(const double *values);

        double length();

        double lengthSquared() const;

        double distance(Vector other) const;

        double distanceSquared(Vector other) const;

        double angle(Vector other);

        double differenceAngle(Vector other) const;

        double dot(Vector other);

        void scalar(double value);

        void add(Vector other);

        void subtract(Vector other);

        void multiply(Vector other);

        void update(const double *values);

        double getX() { return this->x; }

        double getY() { return this->y; }

        double getZ() { return this->z; }

        Vector *midPoint(Vector other);

        Vector clone() const;

    private:
        double x;
        double y;
        double z;
    };
}


#endif //CINEMATICCONTROLLER_VECTOR_H
