#ifndef MPJVP_MATRIX34
#define MPJVP_MATRIX34

#include "physics/Vector3f.hpp"
#include "physics/Quaternion.hpp"
#include "physics/Matrix33.hpp"

#include <array>

class Matrix34
{

public:
    Matrix34();
    Matrix34(std::array<float, 12> values);
	~Matrix34();

    Matrix34 & operator*=(const Matrix34 & b);
    Matrix34 & operator*=(float b);
    Matrix34 & operator/=(float b);
    
    // Implicit conversion to glm::mat4
    operator glm::mat4();
    operator const glm::mat4() const;

    Matrix34 inverse();
    void setOrientationAndPosition(const Quaternion & q, const Vector3f & p); // Set the matrix base on a quaternion and a position
    Vector3f transformPosition(const Vector3f & vector);
    Vector3f transformDirection(const Vector3f & vector); // Transform a direction by ignoring the translation
    Matrix33 extractMatrix33() const;

    inline std::array<float, 12> & getValues() { return m_values; }

    friend Matrix34 operator-(Matrix34 m);
    friend Matrix34 operator*(Matrix34 a, const Matrix34 & b); // Combination of affine transformations
    friend Vector3f operator*(const Matrix34 & a, Vector3f b); // Transform of a vector
    friend Matrix34 operator*(Matrix34 a, float b);
    friend Matrix34 operator*(float a, Matrix34 b);
    friend Matrix34 operator/(Matrix34 a, float b);

    friend std::ostream& operator<<(std::ostream & out, const Matrix34 & m);

private:
    std::array<float, 12> m_values;
};

#endif // MPJVP_MATRIX34
