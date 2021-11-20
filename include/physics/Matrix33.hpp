#ifndef MPJVP_MATRIX33
#define MPJVP_MATRIX33

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include "physics/Vector3f.hpp"
#include "physics/Quaternion.hpp"

#include <array>

class Matrix33
{

public:
	Matrix33();
    Matrix33(std::array<float, 9> values);
	~Matrix33();

    Matrix33 & operator+=(const Matrix33 & b);
    Matrix33 & operator-=(const Matrix33 & b);
    Matrix33 & operator*=(const Matrix33 & b);
    Matrix33 & operator*=(float b);
    Matrix33 & operator/=(float b);

    // Implicit conversion to glm::mat4
    operator glm::mat4();
    operator const glm::mat4() const;

    Matrix33 inverse();
    Matrix33 transpose();
    void setOrientation(const Quaternion & q);

    inline std::array<float, 9> & getValues() { return m_values; }

    friend Matrix33 operator+(Matrix33 a, const Matrix33 & b);
    friend Matrix33 operator-(Matrix33 a, const Matrix33 & b);
    friend Matrix33 operator-(Matrix33 m);
    friend Matrix33 operator*(Matrix33 a, const Matrix33 & b); // Combination of linear transformations
    friend Vector3f operator*(const Matrix33 & a, Vector3f b); // Transform of a vector
    friend Matrix33 operator*(Matrix33 a, float b);
    friend Matrix33 operator*(float a, Matrix33 b);
    friend Matrix33 operator/(Matrix33 a, float b);

    friend std::ostream& operator<<(std::ostream & out, const Matrix33 & m);

private:
    std::array<float, 9> m_values;
};

#endif // MPJVP_MATRIX33
