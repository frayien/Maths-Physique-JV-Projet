#include "physics/Vector3f.hpp"

Vector3f::Vector3f() : m_x{0.f}, m_y{0.f}, m_z{0.f}
{
}

Vector3f::Vector3f(float x, float y, float z) : m_x{x}, m_y{y}, m_z{z}
{
}

Vector3f::Vector3f(const std::array<float, 3> & arr) : m_x{arr[0]}, m_y{arr[1]}, m_z{arr[2]}
{
}

Vector3f::~Vector3f()
{
}

float Vector3f::norm() const
{
    return std::sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

Vector3f Vector3f::normalize() const
{
    float n = norm();
    return Vector3f{m_x / n, m_y / n, m_z / n};
}

float Vector3f::dotProduct(const Vector3f & b) const
{
    return m_x*b.m_x + m_y*b.m_y + m_z*b.m_z;
}

Vector3f Vector3f::crossProduct(const Vector3f & b) const
{
    return Vector3f{ m_y*b.m_z - m_z*b.m_y, m_z*b.m_x - m_x*b.m_z, m_x*b.m_y - m_y*b.m_x };
}

Vector3f & Vector3f::operator+=(const Vector3f & b)
{
    m_x += b.m_x;
    m_y += b.m_y;
    m_z += b.m_z;

    return *this;
}

Vector3f & Vector3f::operator-=(const Vector3f & b)
{
    m_x -= b.m_x;
    m_y -= b.m_y;
    m_z -= b.m_z;

    return *this;
}

Vector3f & Vector3f::operator*=(float b)
{
    m_x *= b;
    m_y *= b;
    m_z *= b;

    return *this;
}

Vector3f & Vector3f::operator/=(float b)
{
    m_x /= b;
    m_y /= b;
    m_z /= b;

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
    a.m_x = -a.m_x;
    a.m_y = -a.m_y;
    a.m_z = -a.m_z;

    return a;
}

Vector3f operator*(Vector3f a, float b)
{
    return a *= b;
}

Vector3f operator*(float a, Vector3f b)
{
    return b *= a;
}

Vector3f operator/(Vector3f a, float b)
{
    return a /= b;
}

std::ostream& operator<<(std::ostream & out, const Vector3f & a)
{
    return out << "(" << a.m_x << ", " << a.m_y << ", " << a.m_z << ")";
}
