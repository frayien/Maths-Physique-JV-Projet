#include "Vector3f.hpp"

Vector3f::Vector3f() : x{0.f}, y{0.f}, z{0.f}
{
}

Vector3f::Vector3f(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z}
{
}

Vector3f::~Vector3f()
{
}

float Vector3f::norm() const
{
    return std::sqrt(x*x + y*y + z*z);
}

Vector3f Vector3f::normalize() const
{
    float n = norm();
    return Vector3f{x / n, y / n, z / n};
}

Vector3f & Vector3f::operator+=(const Vector3f & b)
{
    x += b.x;
    y += b.y;
    z += b.z;

    return *this;
}

Vector3f & Vector3f::operator-=(const Vector3f & b)
{
    x -= b.x;
    y -= b.y;
    z -= b.z;

    return *this;
}

Vector3f & Vector3f::operator*=(const float & b)
{
    x *= b;
    y *= b;
    z *= b;

    return *this;
}

Vector3f & Vector3f::operator/=(const float & b)
{
    x /= b;
    y /= b;
    z /= b;

    return *this;
}

// //////////// friends //////////// //

Vector3f operator+(Vector3f a, const Vector3f & b)
{
    return a += b;
}

Vector3f operator-(Vector3f a, const Vector3f & b)
{
    return a -= b;
}

Vector3f operator-(Vector3f a)
{
    a.x = -a.x;
    a.y = -a.y;
    a.z = -a.z;
    
    return a;
}

Vector3f operator*(Vector3f a, const float & b)
{
    return a *= b;
}

Vector3f operator*(float const& a, Vector3f b)
{
    return b *= a;
}

Vector3f operator/(Vector3f a, const float & b)
{
    return a /= b;
}

std::ostream& operator<<(std::ostream & out, const Vector3f & a)
{
    return out << "(" << a.x << ", " << a.y << ", " << a.z << ")";
}