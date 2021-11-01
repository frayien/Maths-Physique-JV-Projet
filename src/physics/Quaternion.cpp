#include "physics/Quaternion.hpp"

Quaternion::Quaternion() :
    m_w{0.f},
    m_x{0.f},
    m_y{0.f},
    m_z{0.f}
{
}
Quaternion::Quaternion(float w) :
    m_w{w},
    m_x{0.f},
    m_y{0.f},
    m_z{0.f}
{
}

Quaternion::Quaternion(float w, float x, float y, float z) :
    m_w{w},
    m_x{x},
    m_y{y},
    m_z{z}
{
}

Quaternion::Quaternion(const std::array<float, 4> & array) :
    m_w{array[0]},
    m_x{array[1]},
    m_y{array[2]},
    m_z{array[3]}
{
}

Quaternion::~Quaternion()
{
}

float Quaternion::norm() const
{
    return std::sqrt(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);
}

Quaternion Quaternion::normalize() const
{
    float n = norm();
    if(n == 0) return Quaternion{0.f};
    return {m_w / n, m_x / n, m_y / n, m_z / n};
}

Quaternion Quaternion::rotate(const Vector3f & vector) const
{
    Quaternion l_angles{0.f, vector.getX(), vector.getY(), vector.getZ()};
    return (*this) * l_angles;
}

Quaternion Quaternion::update(const Vector3f & angularVelocity, float deltaTime) const
{
    Quaternion l_angularVelocity{0.f, angularVelocity.getX(), angularVelocity.getY(), angularVelocity.getZ()};
    return (*this) + (deltaTime / 2.f) * l_angularVelocity * (*this); 
}

Quaternion & Quaternion::operator+=(const Quaternion & b)
{
    m_w += b.m_w;
    m_x += b.m_x;
    m_y += b.m_y;
    m_z += b.m_z;

    return *this;
}

Quaternion & Quaternion::operator-=(const Quaternion & b)
{
    m_w -= b.m_w;
    m_x -= b.m_x;
    m_y -= b.m_y;
    m_z -= b.m_z;

    return *this;
}

Quaternion & Quaternion::operator*=(const Quaternion & b)
{
    float t_w = m_w;
    float t_x = m_x;
    float t_y = m_y;
    float t_z = m_z;

    m_w = t_w*b.m_w - t_x*b.m_x - t_y*b.m_y - t_z*b.m_z;
    m_x = t_w*b.m_x + t_x*b.m_w + t_y*b.m_z - t_z*b.m_y;
    m_y = t_w*b.m_y + t_y*b.m_w + t_z*b.m_x - t_x*b.m_z;
    m_z = t_w*b.m_z + t_z*b.m_w + t_x*b.m_y - t_y*b.m_x;

    return *this;
}

Quaternion & Quaternion::operator*=(float b)
{
    m_w *= b;
    m_x *= b;
    m_y *= b;
    m_z *= b;

    return *this;
}

Quaternion & Quaternion::operator/=(float b)
{
    m_w /= b;
    m_x /= b;
    m_y /= b;
    m_z /= b;

    return *this;
}

// //////////// friends //////////// //

Quaternion operator+(Quaternion a, const Quaternion & b)
{
    return a += b;
}

Quaternion operator-(Quaternion a, const Quaternion & b)
{
    return a -= b;
}

Quaternion operator-(Quaternion a)
{
    a.m_w = -a.m_w;
    a.m_x = -a.m_x;
    a.m_y = -a.m_y;
    a.m_z = -a.m_z;

    return a;
}

Quaternion operator*(Quaternion a, const Quaternion & b)
{
    return a *= b;
}

Quaternion operator*(Quaternion a, float b)
{
    return a *= b;
}

Quaternion operator*(float a, Quaternion b)
{
    return b *= a;
}

Quaternion operator/(Quaternion a, float b)
{
    return a /= b;
}

std::ostream& operator<<(std::ostream & out, const Quaternion & a)
{
    return out << "(" << a.m_w << " + " << a.m_x << "i + " << a.m_y << "j + " << a.m_z << "k)";
}