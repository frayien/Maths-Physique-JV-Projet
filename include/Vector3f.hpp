#ifndef FNES_VECTOR3F
#define FNES_VECTOR3F

#include <iostream>
#include <cmath>

class Vector3f
{
public:
    Vector3f();
    Vector3f(float x, float y, float z);
    ~Vector3f();

    float norm() const;
    Vector3f normalize() const;

    float dotProduct(const Vector3f & b) const;
    Vector3f crossProduct(const Vector3f & b) const;

    Vector3f & operator+=(const Vector3f & b);
    Vector3f & operator-=(const Vector3f & b);
    Vector3f & operator*=(float b);
    Vector3f & operator/=(float b);

    friend Vector3f operator+(Vector3f a, const Vector3f & b);
    friend Vector3f operator-(Vector3f a, const Vector3f & b);
    friend Vector3f operator-(Vector3f a);
    friend Vector3f operator*(Vector3f a, float b);
    friend Vector3f operator*(float a, Vector3f b);
    friend Vector3f operator/(Vector3f a, float b);

    friend std::ostream& operator<<(std::ostream & out, const Vector3f & a);

    inline float getX() const { return m_x; }
    inline float getY() const { return m_y; }
    inline float getZ() const { return m_z; }

    inline void setX(float x) { m_x = x; }
    inline void setY(float y) { m_y = y; }
    inline void setZ(float z) { m_z = z; }

private:
    float m_x;
    float m_y;
    float m_z;
};

#endif // FNES_VECTOR3F
