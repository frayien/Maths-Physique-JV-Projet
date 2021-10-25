#ifndef MPJVP_QUATERNION
#define MPJVP_QUATERNION

#include <iostream>
#include <cmath>
#include <array>

class Quaternion
{
public:
    Quaternion();
    Quaternion(float w);
    Quaternion(float w, float x, float y, float z);
    Quaternion(const std::array<float, 4> & array);
    ~Quaternion();

    float norm() const;
    Quaternion normalize() const;

    Quaternion & operator+=(const Quaternion & b);
    Quaternion & operator-=(const Quaternion & b);
    Quaternion & operator*=(const Quaternion & b);
    Quaternion & operator*=(float b);
    Quaternion & operator/=(float b);

    friend Quaternion operator+(Quaternion a, const Quaternion & b);
    friend Quaternion operator-(Quaternion a, const Quaternion & b);
    friend Quaternion operator-(Quaternion a);
    friend Quaternion operator*(Quaternion a, const Quaternion & b);
    friend Quaternion operator*(Quaternion a, float b);
    friend Quaternion operator*(float a, Quaternion b);
    friend Quaternion operator/(Quaternion a, float b);

    friend std::ostream& operator<<(std::ostream & out, const Quaternion & a);

private:
    // i^2 = j^2 = k^2 = ijk = -1
    // w + ix + jy + kz
    float m_w;
    float m_x;
    float m_y;
    float m_z;

};


#endif // MPJVP_QUATERNION
