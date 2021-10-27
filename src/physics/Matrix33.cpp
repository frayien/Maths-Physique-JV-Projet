#include "physics/Matrix33.hpp"

Matrix33::Matrix33()
{
    for (int i = 0; i < 9; i++)
    {
        m_values[i] = 0.0f;
    }
}

Matrix33::Matrix33(std::array<float, 9> values)
{
    for (int i = 0; i < 9; i++)
    {
        m_values[i] = values[i];
    }
}

Matrix33::~Matrix33()
{

}

Matrix33 & Matrix33::operator+=(const Matrix33 & b)
{
    for (int i = 0 ; i < 9; i++)
    {
        m_values[i] += b.m_values[i];
    }

    return *this;
}

Matrix33 & Matrix33::operator-=(const Matrix33 & b)
{
    for (int i = 0 ; i < 9; i++)
    {
        m_values[i] -= b.m_values[i];
    }

    return *this;
}

Matrix33 & Matrix33::operator*=(const Matrix33 & b)
{
    std::array<float, 9> values{m_values};

    for (int i = 0; i < 9; i++)
    {
        m_values[i] = 0.0f;
        for (int j = 0; j < 3; j++)
        {
            int column = (i % 3);
            int row = (i / 3);
            m_values[i] += values[row * 3 + j] * b.m_values[j * 3 + column];
        }
    }

    return *this;
}

Matrix33 & Matrix33::operator*=(float b)
{
    for (int i = 0 ; i < 9; i++)
    {
        m_values[i] *= b;
    }

    return *this;
}

Matrix33 & Matrix33::operator/=(float b)
{
    for (int i = 0 ; i < 9; i++)
    {
        m_values[i] /= b;
    }

    return *this;
}

Matrix33 Matrix33::inverse()
{
    // In our case it's a transformation matrix 
    // so the inverse is equal to the transpose
    return transpose();
}

Matrix33 Matrix33::transpose()
{
    Matrix33 res;

    for (int i = 0; i < 9; i++)
    {
        int column = (i % 3);
        int row = (i / 3);
        res.m_values[i] = m_values[column * 3 + row];
    }

    return res;
}

void Matrix33::setOrientation(const Quaternion & q)
{
    float w = q.getW();
    float x = q.getX();
    float y = q.getY();
    float z = q.getZ();

    // Precalculation of terms
    float _2x2 = 2 * x * x;
    float _2y2 = 2 * y * y;
    float _2z2 = 2 * z * z;

    float _2xy = 2 * x * y;
    float _2xz = 2 * x * z;
    float _2xw = 2 * x * w;

    float _2yz = 2 * y * z;
    float _2yw = 2 * y * w;

    float _2zw = 2 * z * w;

    m_values =
    {
        1 - (_2y2 + _2z2),  _2xy + _2zw,        _2xz - _2yw,
        _2xy - _2zw,        1 - (_2x2 + _2z2),  _2yz + _2xw,
        _2xz + _2yw,        _2yz - _2xw,        1 - (_2x2 + _2y2)
    };
}

// //////////// friends //////////// //

Matrix33 operator+(Matrix33 a, const Matrix33 & b)
{
    return a += b;
}

Matrix33 operator-(Matrix33 a, const Matrix33 & b)
{
    return a -= b;
}

Matrix33 operator-(Matrix33 m)
{
    for (int i = 0; i < 9; i++)
    {
        m.m_values[i] = -m.m_values[i];
    }

    return m;
}

Matrix33 operator*(Matrix33 a, const Matrix33 & b)
{
    return a *= b;
}

Vector3f operator*(const Matrix33 & a, Vector3f b)
{
    float bX = b.getX();
    float bY = b.getY();
    float bZ = b.getZ();

    b.setX(a.m_values[0] * bX + a.m_values[1] * bY + a.m_values[2] * bZ);
    b.setY(a.m_values[3] * bX + a.m_values[4] * bY + a.m_values[5] * bZ);
    b.setZ(a.m_values[6] * bX + a.m_values[7] * bY + a.m_values[8] * bZ);

    return b;
}

Matrix33 operator*(Matrix33 a, float b)
{
    return a *= b;
}

Matrix33 operator*(float a, Matrix33 b)
{
    return b *= a;
}

Matrix33 operator/(Matrix33 a, float b)
{
    return a /= b;
}

std::ostream& operator<<(std::ostream & out, const Matrix33 & m)
{
    for (int i = 0; i < 9; i++)
    {
        if (i != 0 && i % 3 == 0)
        {
            out << std::endl;
        }
        out << m.m_values[i] << " ";
    }

    return out;
}