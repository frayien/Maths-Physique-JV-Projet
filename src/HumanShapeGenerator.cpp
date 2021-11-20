#include "HumanShapeGenerator.hpp"

HumanShapeGenerator::HumanShapeGenerator(RigidBody* rigidbody, color_t color) :
    m_rigidbody{rigidbody},
    m_head{color},
    m_chest{color},
    m_leftArm{color},
    m_rightArm{color},
    m_leftLeg{color},
    m_rightLeg{color}
{

}

HumanShapeGenerator::~HumanShapeGenerator()
{

}

void HumanShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    Matrix34 rotationMat34;
    rotationMat34.setOrientationAndPosition(m_rigidbody->getQuaternion(), Vector3f{0.0f, 0.0f, 0.0f});

    const glm::mat4 rotation = rotationMat34.extractMatrix33();

    // Head
    Vector3f headPosition{0.0f, 0.0f, 0.9f * m_scale[2]};
    headPosition = m_rigidbody->getTranformMatrix() * headPosition;
    m_head.setPosition(headPosition);
    m_head.setScale({m_scale[0], m_scale[2] * 0.1f, m_scale[2] * 0.1f});
    m_head.setRotation(rotation);
    m_head.addShape(shapes);

    // Chest
    Vector3f chestPosition{0.0f, 0.0f, 0.6f * m_scale[2]};
    chestPosition = m_rigidbody->getTranformMatrix() * chestPosition;
    m_chest.setPosition(chestPosition);
    m_chest.setScale({m_scale[0] * 0.5f, m_scale[1] * 0.1f, m_scale[2] * 0.2f});
    m_chest.setRotation(rotation);
    m_chest.addShape(shapes);

    // Left arm
    Vector3f leftArmPosition{0.0f, -0.3f * m_scale[1], 0.775f * m_scale[2]};
    leftArmPosition = m_rigidbody->getTranformMatrix() * leftArmPosition;
    m_leftArm.setPosition(leftArmPosition);
    m_leftArm.setScale({m_scale[0] * 0.5f, m_scale[1] * 0.2f, m_scale[2] * 0.025f});
    m_leftArm.setRotation(rotation);
    m_leftArm.addShape(shapes);

    // Right arm
    Vector3f rightArmPosition{0.0f, 0.3f * m_scale[1], 0.775f * m_scale[2]};
    rightArmPosition = m_rigidbody->getTranformMatrix() * rightArmPosition;
    m_rightArm.setPosition(rightArmPosition);
    m_rightArm.setScale({m_scale[0] * 0.5f, m_scale[1] * 0.2f, m_scale[2] * 0.025f});
    m_rightArm.setRotation(rotation);
    m_rightArm.addShape(shapes);

    // Left leg
    Vector3f leftLegPosition{0.0f, -0.075f * m_scale[1], 0.2f * m_scale[2]};
    leftLegPosition = m_rigidbody->getTranformMatrix() * leftLegPosition;
    m_leftLeg.setPosition(leftLegPosition);
    m_leftLeg.setScale({m_scale[0] * 0.5f, m_scale[1] * 0.025f, m_scale[2] * 0.2f});
    m_leftLeg.setRotation(rotation);
    m_leftLeg.addShape(shapes);

    // Right leg
    Vector3f rightLegPosition{0.0f, 0.075f * m_scale[1], 0.2f * m_scale[2]};
    rightLegPosition = m_rigidbody->getTranformMatrix() * rightLegPosition;
    m_rightLeg.setPosition(rightLegPosition);
    m_rightLeg.setScale({m_scale[0] * 0.5f, m_scale[1] * 0.025f, m_scale[2] * 0.2f});
    m_rightLeg.setRotation(rotation);
    m_rightLeg.addShape(shapes);
}