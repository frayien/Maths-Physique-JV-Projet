#include "Application.hpp"

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
    cube->scale({0.1f, 10.0f, 10.0f});

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
    particleRendered = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particleRendered->scale(0.2f);

    particle = new Particle();

    // Initialisation of the particle based on the data specified by the user in ImGui
    particle->setMass(1.0f);
    particle->setPosition(positionInit);
    particle->setVelocity(velocityInit);

    ParticleGravity* particleGravity = new ParticleGravity();
    particleRegistry.AddForce(particle, particleGravity, 0.0);
}

void Application::update(World & world, float deltaTime)
{
    static bool canPressPause = true;
    static bool pause = false;
    static constexpr float TIMESTEP = 1.0f / 60.0f;
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    // If we want to reset the world and the pause isn't active
    if (resetMarks && !pause)
    {
        while (!marks.empty())
        {
            world.removeShape(marks.back());
            marks.pop_back();
        }

        resetMarks = false;
    }

    // If the elapsed time is greater than TIMESTEP, we enter the loop
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;
        
        if (!pause)
        {
            // Creation of multiple little spheres to symbolize the path taken by the particle
            countTimeStepMarks++;

            if (countTimeStepMarks >= countTimeStepMarksMax)
            {
                countTimeStepMarks = 0;
                std::shared_ptr<BufferedShape> tmpSphere = world.makeSphere({1.0f, 0.0f, 0.0f});
                tmpSphere->setPosition(particle->getPosition());
                tmpSphere->scale(0.03f);

                marks.push_back(tmpSphere);
            }

            for (std::vector<ParticleForceRegistry::Entry>::iterator it = particleRegistry.getRegistry().begin(); it != particleRegistry.getRegistry().end(); ++it) {
                it->particleForceGenerator->UpdateForce(it->particle, 0.0);
            }

            // We update the position of the particle and its graphical rendering
            particle->integrate(TIMESTEP);
            particleRendered->setPosition(particle->getPosition());
        }
    }

    // If we click on the reset button
    if(world.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        particle->setPosition(positionInit);
        particle->setVelocity(velocityInit);
        resetMarks = true;
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
    this->positionInit = positionInit;
}

// Function used to set the initial velocity of the particle as defined in ImGui
void Application::setVelocityInit(Vector3f velocityInit)
{
    this->velocityInit = velocityInit;
}

void Application::setResetMarks(bool resetMarks)
{
    this->resetMarks = resetMarks;
}