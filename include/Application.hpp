#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include <map>
#include <functional>
#include <iomanip>

#include "render/GraphicsEngine.hpp"

#include "render/shape/SphereShapeGenerator.hpp"
#include "render/shape/CubeShapeGenerator.hpp"

#include "physics/Particle.hpp"
#include "physics/ParticleGravity.hpp"
#include "physics/PhysicsEngine.hpp"
#include "physics/ParticleAnchoredSpring.hpp"
#include "physics/ParticleDrag.hpp"
#include "physics/ParticleSpring.hpp"
#include "GameState.hpp"

class Application : public IImGuiFrameGenerator
{
private:
    static constexpr float PI = glm::pi<float>();
    
    std::vector<std::unique_ptr<IShapeGenerator>> m_particleShapes;

    PhysicsEngine m_physicsEngine;
    GameState m_gameState;

    Vector3f m_positionInit = {-8, -3, 0};
    Vector3f m_velocityInit = {0, 4, 7};

    int m_countTimeStepMarks = 0;
    int m_countTimeStepMarksMax = 15;
    std::vector<std::unique_ptr<IShapeGenerator>> m_marks{};
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
    ~Application();

    void init(GraphicsEngine & graphics);
    void update(GraphicsEngine & graphics, float deltaTime);

    virtual void createFrame() override;

    void updateCamera(GraphicsEngine & graphics, float deltaTime);

    void applyParticleAnchoredSpringSettings(Vector3f anchorPosition, float springStiffness, float springRestLength);
    void applyParticleDragSettings(float k1, float k2);

    void createBlob(GraphicsEngine & graphics);
    void updateBlob();
    void resetBlob();

    inline GameState & getGameState() { return m_gameState; }
};

#endif // MPJVP_APPLICATION
