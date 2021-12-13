#include "physics/Matrix34.hpp"
#include "physics/Matrix33.hpp"

Matrix34::Matrix34()
{
    for (int i = 0; i < 12; i++)
    {
        m_values[i] = 0.0f;
    }
}

Matrix34::Matrix34(std::array<float, 12> values)
{
    for (int i = 0; i < 12; i++)
    {
        m_values[i] = values[i];
    }
}

Matrix34::~Matrix34()
{

}

Matrix34 & Matrix34::operator*=(const Matrix34 & b)
{
    std::array<float, 12> values{m_values};

    for (int i = 0; i < 12; i++)
    {
        int column = (i % 4);
        int row = (i / 4);

        int jMax = column == 3 ? 4 : 3;

        m_values[i] = 0.0f;

        for (int j = 0; j < jMax; j++)
        {
            if (j == 3)
            {
                // 4th column
                m_values[i] += values[row * 4 + j] * 1;
            }
            else
            {
                m_values[i] += values[row * 4 + j] * b.m_values[j * 4 + column];
            }
        }
    }
    return *this;
}

Matrix34 & Matrix34::operator*=(float b)
{
    for (int i = 0; i < 12; i++)
    {
        m_values[i] *= b;
    }

    return *this;
}

Matrix34 & Matrix34::operator/=(float b)
{
    for (int i = 0; i < 12; i++)
    {
        m_values[i] /= b;
    }

    return *this;
}

Matrix34::operator glm::mat4()
{
    // glm::mat4 is column defined
    return
    {
        m_values[0], m_values[4], m_values[8] , 0.0f,
        m_values[1], m_values[5], m_values[9] , 0.0f,
        m_values[2], m_values[6], m_values[10], 0.0f,
        m_values[3], m_values[7], m_values[11], 1.0f,
    };
}

Matrix34::operator const glm::mat4() const
{
    // glm::mat4 is column defined
    return
    {
        m_values[0], m_values[4], m_values[8] , 0.0f,
        m_values[1], m_values[5], m_values[9] , 0.0f,
        m_values[2], m_values[6], m_values[10], 0.0f,
        m_values[3], m_values[7], m_values[11], 1.0f,
    };
}

Matrix34 Matrix34::inverse()
{
    // LTM : Linear Transform Matrix
    Matrix33 ltm
    {
        {
            m_values[0], m_values[1], m_values[2],
            m_values[4], m_values[5], m_values[6],
            m_values[8], m_values[9], m_values[10],
        }
    };

    Vector3f translation{m_values[3], m_values[7], m_values[11]};

    Matrix33 ltmInverse = ltm.inverse();
    std::array<float, 9> ltmInverseValues = ltmInverse.getValues();

    Vector3f newTranslation = - ltmInverse * translation;

    return Matrix34
    {
        {
            ltmInverseValues[0], ltmInverseValues[1], ltmInverseValues[2], newTranslation.getX(),
            ltmInverseValues[3], ltmInverseValues[4], ltmInverseValues[5], newTranslation.getY(),
            ltmInverseValues[6], ltmInverseValues[7], ltmInverseValues[8], newTranslation.getZ()
        }
    };
}

void Matrix34::setOrientationAndPosition(const Quaternion & q, const Vector3f & p)
{
    // LTM : Linear Transform Matrix
    Matrix33 ltm;
    ltm.setOrientation(q);
    std::array<float, 9> ltmValues = ltm.getValues();

    m_values =
    {
        ltmValues[0], ltmValues[1], ltmValues[2], p.getX(),
        ltmValues[3], ltmValues[4], ltmValues[5], p.getY(),
        ltmValues[6], ltmValues[7], ltmValues[8], p.getZ()
    };
}

Matrix33 Matrix34::extractMatrix33() const
{
    Matrix33 ltm
    {
        {
            m_values[0], m_values[1], m_values[2],
            m_values[4], m_values[5], m_values[6],
            m_values[8], m_values[9], m_values[10],
        }
    };
    return ltm;
}

Vector3f Matrix34::transformPosition(const Vector3f & vector)
{
    return *this * vector;
}

Vector3f Matrix34::transformDirection(const Vector3f & vector)
{
    // LTM : Linear Transform Matrix
    Matrix33 ltm
    {
        {
            m_values[0], m_values[1], m_values[2],
            m_values[4], m_values[5], m_values[6],
            m_values[8], m_values[9], m_values[10],
        }
    };

    return ltm * vector;
}

// //////////// friends //////////// //

Matrix34 operator-(Matrix34 m)
{
    for (int i = 0; i < 12; i++)
    {
        m.m_values[i] = -m.m_values[i];
    }

    return m;
}

Matrix34 operator*(Matrix34 a, const Matrix34 & b)
{
    return a *= b;
}

Vector3f operator*(const Matrix34 & a, Vector3f b)
{
    float bX = b.getX();
    float bY = b.getY();
    float bZ = b.getZ();

    b.setX(a.m_values[0] * bX + a.m_values[1] * bY + a.m_values[2]  * bZ + a.m_values[3]);
    b.setY(a.m_values[4] * bX + a.m_values[5] * bY + a.m_values[6]  * bZ + a.m_values[7]);
    b.setZ(a.m_values[8] * bX + a.m_values[9] * bY + a.m_values[10] * bZ + a.m_values[11]);

    return b;
}

Matrix34 operator*(Matrix34 a, float b)
{
    return a *= b;
}

Matrix34 operator*(float a, Matrix34 b)
{
    return b *= a;
}

Matrix34 operator/(Matrix34 a, float b)
{
    return a /= b;
}

std::ostream& operator<<(std::ostream & out, const Matrix34 & m)
{
    for (int i = 0; i < 12; i++)
    {
        if (i != 0 && i % 4 == 0)
        {
            out << std::endl;
        }
        out << m.m_values[i] << " ";
    }

    return out;
}