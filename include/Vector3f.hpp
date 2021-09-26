#ifndef MPJVP_VECTOR3F
#define MPJVP_VECTOR3F

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

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

    inline operator glm::vec3() { return {m_x, m_y, m_z}; }

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

#endif // MPJVP_VECTOR3F
