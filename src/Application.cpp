#include "Application.hpp"

Application::~Application()
{
    for (Particle * particle : m_particles)
    {
        delete(particle);
    }
}

void Application::init(World & world)
{
    Camera & cam = world.getCamera();
    LightSource & sun = world.getLightSource();

    cam.setPosition({0.0f, 0.0f, 0.0f});
    // rotate cam to look at X- (this way Y+ and Z+ looks more natural for a 2D environment, aka. Y+ is on the right)
    cam.rotateYaw(PI);

    sun.setPosition({0.0f, 0.0f, 0.0f});

    // Creation of the background
    auto cube = world.makeCube({0.9f, 0.9f, 0.9f});
    cube->setPosition({-10.0f, 0.0f, 0.0f});
    cube->scale({0.1f, 15.0f, 10.0f});

    // We mark the center
    cube = world.makeCube({0.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 0.0f});
    cube->scale(0.05f);

    // Y+ axis
    cube = world.makeCube({1.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 1.0f, 0.0f});
    cube->scale(0.05f);

    // Z+ axis
    cube = world.makeCube({0.0f, 1.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 1.0f});
    cube->scale(0.05f);

    // Initialisation of the graphic rendering of the particle
    auto particuleShape = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particuleShape->scale(0.2f);
    m_particleShapes.push_back(particuleShape);

    particuleShape = world.makeSphere({ 0.05f, 0.05f, 0.05f });
    particuleShape->scale(0.2f);
    m_particleShapes.push_back(particuleShape);

    particuleShape = world.makeCube({ 0.1f, 0.5f, 0.5f });
    particuleShape->scale(0.2f);
    m_particleShapes.push_back(particuleShape);

    // Initialisation of the particle based on the data specified by the user in ImGui
    Particle * particle = new Particle(m_positionInit, 1.0f, 0.999f);
    particle->setVelocity(m_velocityInit);
    m_particles.push_back(particle);

    m_particleAnchoredSpring.setK(20.0f);
    m_particleAnchoredSpring.setRestLength(2.0f);
    m_particleAnchoredSpring.setAnchor({-8.0f, 0.0f, 0.0f});

    m_particleDrag.setK1(0.0f);
    m_particleDrag.setK2(0.1f);

    m_physicsEngine.getParticleRegistry().addForce(particle, &m_particleGravity, 0.0);
    m_physicsEngine.getParticleRegistry().addForce(particle, &m_particleAnchoredSpring, 0.0f);
    m_physicsEngine.getParticleRegistry().addForce(particle, &m_particleDrag, 0.0f);

}

void Application::update(World & world, float deltaTime)
{
    static bool canPressPause = true;
    static bool pause = false;
    static constexpr float TIMESTEP = 1.0f / 200.0f;
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    // If we want to reset the world and the pause isn't active
    if (m_resetMarks && !pause)
    {
        while (!m_marks.empty())
        {
            world.removeShape(m_marks.back());
            m_marks.pop_back();
        }

        m_resetMarks = false;
    }

    // If the elapsed time is greater than TIMESTEP, we enter the loop
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;
        
        if (!pause)
        {
            // Creation of multiple little spheres to symbolize the path taken by the particle
            m_countTimeStepMarks++;

            if (m_countTimeStepMarks >= m_countTimeStepMarksMax)
            {
                m_countTimeStepMarks = 0;
                std::shared_ptr<BufferedShape> tmpSphere = world.makeSphere({1.0f, 0.0f, 0.0f});
                tmpSphere->setPosition(m_particles[0]->getPosition());
                tmpSphere->scale(0.03f);

                m_marks.push_back(tmpSphere);
            }

            // Update physics engine
            m_physicsEngine.update(TIMESTEP, m_particles);
        }
    }

    // update graphics
    Vector3f particulePos = m_particles[0]->getPosition();
    Vector3f anchorPos = m_particleAnchoredSpring.getAnchor();
    Vector3f springPos = (particulePos + anchorPos)/2.0f;

    const Vector3f springDefaultDir {0.f, 0.f, 1.f};

    Vector3f springDir = (particulePos - anchorPos).normalize();
    float springAngle = -glm::acos(springDir.dotProduct(springDefaultDir));
    float springLen = (particulePos - anchorPos).norm();

    m_particleShapes[0]->setPosition(particulePos);
    m_particleShapes[1]->setPosition(anchorPos);
    m_particleShapes[2]->setPosition(springPos);
    m_particleShapes[2]->resetRotation();
    m_particleShapes[2]->rotate(springAngle, springDir.crossProduct(springDefaultDir));
    m_particleShapes[2]->setScale({0.05f, 0.05f, springLen / 2.f});


    // If we click on the reset button
    if(world.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        m_particles[0]->setPosition(m_positionInit);
        m_particles[0]->setVelocity(m_velocityInit);

        m_resetMarks = true;
    }

    // If we click on the pause button
    if(world.getWindow().isKeyPressed(GLFW_KEY_P) && canPressPause)
    {
        canPressPause = false;
        pause = !pause;
    }
    else if(!world.getWindow().isKeyPressed(GLFW_KEY_P))
    {
        canPressPause = true;
    }

    updateCamera(world, deltaTime);
}

// Function used to translate and rotate the camera
void Application::updateCamera(World & world, float deltaTime)
{
    const Window & win = world.getWindow();
          Camera & cam = world.getCamera();

    const float rotationSpeed = 1.0f * PI; // rad/s
    const float moveSpeed     = 0.1f; // unit/s

    static std::vector<std::tuple<int, std::function<void ()>>> keymap = 
    {
        { GLFW_KEY_A, [&](){ cam.rotateYaw(   deltaTime * rotationSpeed); } },
        { GLFW_KEY_D, [&](){ cam.rotateYaw(  -deltaTime * rotationSpeed); } },
        { GLFW_KEY_W, [&](){ cam.rotatePitch( deltaTime * rotationSpeed); } },
        { GLFW_KEY_S, [&](){ cam.rotatePitch(-deltaTime * rotationSpeed); } },

        { GLFW_KEY_LEFT , [&](){ cam.move( moveSpeed * glm::vec3{0.0f, 1.0f, 0.0f}); } },
        { GLFW_KEY_RIGHT, [&](){ cam.move(-moveSpeed * glm::vec3{0.0f, 1.0f, 0.0f}); } },
        { GLFW_KEY_UP   , [&](){ cam.move( moveSpeed * glm::vec3{1.0f, 0.0f, 0.0f}); } },
        { GLFW_KEY_DOWN , [&](){ cam.move(-moveSpeed * glm::vec3{1.0f, 0.0f, 0.0f}); } },
        { GLFW_KEY_O    , [&](){ cam.move( moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
        { GLFW_KEY_L    , [&](){ cam.move(-moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f}); } },
    };

    for(const auto & [key, fun] : keymap)
    {
        if(win.isKeyPressed(key)) fun();
    }
}

// Function used to set the initial position of the particle as defined in ImGui
void Application::setPositionInit(Vector3f positionInit)
{
    m_positionInit = positionInit;
}

// Function used to set the initial velocity of the particle as defined in ImGui
void Application::setVelocityInit(Vector3f velocityInit)
{
    m_velocityInit = velocityInit;
}

void Application::setResetMarks(bool resetMarks)
{
    m_resetMarks = resetMarks;
}

void Application::applyParticleAnchoredSpringSettings(Vector3f anchorPosition, float springStiffness, float springRestLength)
{
    m_particleAnchoredSpring.setAnchor(anchorPosition);
    m_particleAnchoredSpring.setK(springStiffness);
    m_particleAnchoredSpring.setRestLength(springRestLength);
}

void Application::applyParticleDragSettings(float k1, float k2)
{
    m_particleDrag.setK1(k1);
    m_particleDrag.setK2(k2);
}