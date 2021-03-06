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

#include "ParticleShapeGenerator.hpp"
#include "LinkShapeGenerator.hpp"
#include "RigidCubeShapeGenerator.hpp"
#include "RigidBodyLinkShapeGenerator.hpp"
#include "HumanShapeGenerator.hpp"
#include "ContactShapeGenerator.hpp"
#include "RigidBodySphereShapeGenerator.hpp"

#include "physics/Particle.hpp"
#include "physics/forces/Particle/ParticleGravity.hpp"
#include "physics/forces/RigidBody/RigidBodyGravity.hpp"
#include "physics/PhysicsEngine.hpp"
#include "physics/forces/Particle/ParticleAnchoredSpring.hpp"
#include "physics/forces/Particle/ParticleDrag.hpp"
#include "physics/forces/Particle/ParticleSpring.hpp"
#include "physics/contacts/Particle/ParticleCable.hpp"
#include "physics/contacts/Particle/ParticleRod.hpp"
#include "physics/forces/RigidBody/RigidBodyAnchoredSpring.hpp"
#include "physics/forces/RigidBody/RigidBodySpring.hpp"
#include "GameState.hpp"
#include "physics/contacts/Particle/WallContactGenerator.hpp"
#include "physics/contacts/Particle/NaiveParticleContactGenerator.hpp"

class Application : public IImGuiFrameGenerator
{
private:
    static constexpr float PI = glm::pi<float>();

    GraphicsEngine m_graphicsEngine;
    PhysicsEngine m_physicsEngine;
    GameState m_gameState;

    // Blob data
    Vector3f m_blobCenterInitPos = {-2.0f, -3.0f, 0.0f};
    float m_blobParticleRadius = 0.2f;
    float m_blobEdgeLength = sqrt(5.0f) - 1.0f; // dodecahedron edge length
    float m_blobCenterToVerticesLength = sqrt(3.0f); // dodecahedron circumradius
    float m_blobK = 200.0f;
    float m_blobCableRestitution = 0.3f;

    // Ground data
    float m_groundLength = 25.0f;
    float m_groundWidth = 25.0f;
    float m_groundThickness = 0.5f;
    float m_groundRestitution = 0.4f;
    Vector3f m_groundCenterPosition{0.0f, 0.0f, -6.0f};
    Vector3f m_groundDirectionWidth{1.0f, 0.0f, 0.0f};  // X+ axis
    Vector3f m_groundDirectionLength{0.0f, 1.0f, 0.0f}; // Y+ axis
    float m_wallsLength = 10.0f;
    float m_wallsWidth = 7.0f;
    float m_wallsThickness = 0.4f;
    float m_wallsRestitution = 0.0f;

    // ImGui data
    std::array<float, 3> currentBlobInitialPosition = {-2.0f, -3.0f,  0.0f};
    std::array<float, 3> currentGroundCenterPosition = {0.0f, 0.0f, -6.0f};

    std::array<std::string, 7> m_modes = {"Blob", "Exemples Phase 2", "Demo 1 Phase 3", "Demo 2 Phase 3 (car collision)", "Demo phase 4 (collision detection)", "Demo sphere/sphere collision", "Demo sphere/plane collision"};
    int m_selected_mode = 0;

    bool m_carCollision = false;

    bool m_pauseAtContact = true;
    bool m_displayContactText = true;
    bool m_displayContactShape = true;
    bool m_resetContactShape = true;

public:
    Application();
    virtual ~Application();

    virtual void createFrame() override;
    void run();

private:
    void init();
    void initParticleForceGenerators();
    void initRigidbodyForceGenerators();
    void update(float deltaTime);

    void updateCamera(float deltaTime);

    void createBlobDemo();
    void moveBlob(Vector3f moveDirection, float deltaTime);
    void jumpBlob();
    void changeBlobSettings();

    void createExamplePhase2Demo();

    void createGround();
    void changeGroundSettings();

    void createRigidCubeDemo();
    void checkRigidCubeDemo();

    void createCarCollisionDemo();
    void checkCarCollision();

    void createDemoPhase4();
    void createSphereAndSphereCollisionDemo();
    void createSphereAndPlaneCollisionDemo();
};

#endif // MPJVP_APPLICATION
