#ifndef FNES_VECTOR3F
#define FNES_VECTOR3F

#include <iostream>
#include <cmath>

class Vector3f
{
public:
    Vector3f();
    Vector3f(float _x, float _y, float _z);
    ~Vector3f();

    float norm() const;
    Vector3f normalize() const;

    Vector3f & operator+=(const Vector3f & b);
    Vector3f & operator-=(const Vector3f & b);
    Vector3f & operator*=(const float & b);
    Vector3f & operator/=(const float & b);

    friend Vector3f operator+(Vector3f a, const Vector3f & b);
    friend Vector3f operator-(Vector3f a, const Vector3f & b);
    friend Vector3f operator-(Vector3f a);
    friend Vector3f operator*(Vector3f a, const float & b);
    friend Vector3f operator*(const float & a, Vector3f b);
    friend Vector3f operator/(Vector3f a, const float & b);

    friend std::ostream& operator<<(std::ostream & out, const Vector3f & a);

public:
    float x;
    float y;
    float z;
};

#endif // FNES_VECTOR3F
