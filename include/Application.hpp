#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <iomanip>
#include <string>

#include "render/GraphicsEngine.hpp"

#include "render/shape/SphereShapeGenerator.hpp"
#include "render/shape/CubeShapeGenerator.hpp"

#include "physics/Particle.hpp"
#include "physics/forces/ParticleGravity.hpp"
#include "physics/PhysicsEngine.hpp"
#include "physics/forces/ParticleAnchoredSpring.hpp"
#include "physics/forces/ParticleDrag.hpp"
#include "physics/forces/ParticleSpring.hpp"
#include "physics/contacts/ParticleCable.hpp"
#include "GameState.hpp"
#include "physics/contacts/WallContactGenerator.hpp"

class Application : public IImGuiFrameGenerator
{
private:
    static constexpr float PI = glm::pi<float>();

    GraphicsEngine m_graphicsEngine;
    PhysicsEngine m_physicsEngine;
    GameState m_gameState;

    Vector3f m_positionInit = {-8, -3, 0};
    Vector3f m_velocityInit = {0, 4, 7};

    int m_countTimeStepMarks = 0;
    int m_countTimeStepMarksMax = 15;
    std::size_t m_markN = 0;
    bool m_resetMarks = false;

    // ImGui

    char* m_projectiles[4] = { "Ball", "Heavy ball", "Laser", "Fireball" };
    std::vector<std::array<float, 3>> m_projectilesInitialVelocity =
    {
        {0.0f, 4.0f, 7.0f},
        {0.0f, 2.0f, 6.0f},
        {0.0f, 16.0f, 6.0f},
        {0.0f, 4.0f, 4.0f}
    };
    std::vector<float> m_projectilesMass = {4.0f, 12.0f, 0.2f, 2.0f};

    int m_currentIndex = 0;
    char* m_currentProjectile = m_projectiles[m_currentIndex];

    float currentMass = 1.0f;
    std::array<float, 3> currentInitialVelocity = { 0.0f,  4.0f,  7.0f};
    std::array<float, 3> currentInitialPosition = {-8.0f, -3.0f,  0.0f};
    float damping = 0.999f;
    std::array<float, 3> currentAnchorPosition = { -8.0f,  0.0f,  0.0f};
    float currentSpringStiffness = 20.0f;
    float currentSpringRestLength = 2.0f;
    float currentK1DragCoef = 0.0f;
    float currentK2DragCoef = 0.1f;
    
public:
    Application();
    virtual ~Application();

    virtual void createFrame() override;
    void run();

private:
    void init();
    void update(float deltaTime);

    void updateCamera(float deltaTime);

    void createBlob();
    void updateBlob();
    void resetBlob();
    void moveBlob(Vector3f moveVector);

    void createGround();
};

#endif // MPJVP_APPLICATION
