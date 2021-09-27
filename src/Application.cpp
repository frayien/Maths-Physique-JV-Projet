#include "Application.hpp"

void Application::init(World & world)
{
    Camera & cam = world.getCamera();
    LightSource & sun = world.getLightSource();

    cam.setPosition({0.0f, 0.0f, 0.0f});
    // rotate cam to look at X- (this way Y+ and Z+ looks more natural for a 2D environment, aka. Y+ is on the right)
    cam.rotateYaw(PI);

    sun.setPosition({0.0f, 0.0f, 0.0f});

    // background
    auto cube = world.makeCube({0.9f, 0.9f, 0.9f});
    cube->setPosition({-10.0f, 0.0f, 0.0f});
    cube->scale({0.1f, 10.0f, 10.0f});

    // center
    cube = world.makeCube({0.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 0.0f});
    cube->scale(0.05f);

    // axe Y+
    cube = world.makeCube({1.0f, 0.0f, 0.0f});
    cube->setPosition({-9.0f, 1.0f, 0.0f});
    cube->scale(0.05f);

    // axe Z+
    cube = world.makeCube({0.0f, 1.0f, 0.0f});
    cube->setPosition({-9.0f, 0.0f, 1.0f});
    cube->scale(0.05f);

    // Construction du rendu graphique de la particule
    particleRendered = world.makeSphere({ 0.2f, 0.2f, 0.2f });
    particleRendered->scale(0.2f);

    // Construction de la particule avec les données spécifiées par l'utilisateur dans ImGUI - A CHANGER
    particle.setMass(1.0f);
    particle.setPosition(positionInit);
    particle.setVelocity(velocityInit);
}

void Application::update(World & world, float deltaTime)
{
    static bool canPressPause = true;
    static bool pause = false;
    static constexpr float TIMESTEP = 1.0f / 60.0f;
    static float elapsedTime = 0.0f;
    elapsedTime += deltaTime;

    if (resetMarks && !pause)
    {
        while (!marks.empty())
        {
            world.removeShape(marks.back());
            marks.pop_back();
        }

        resetMarks = false;
    }

    // Si le temps écoulé est supérieur à TIMESTEP, on entre dans la boucle
    while (elapsedTime >= TIMESTEP)
    {
        elapsedTime -= TIMESTEP;
        // On update la position du projectile ainsi que de son affichage graphique
        if (!pause)
        {
            countTimeStepMarks++;

            if (countTimeStepMarks >= countTimeStepMarksMax)
            {
                countTimeStepMarks = 0;
                std::shared_ptr<BufferedShape> tmpSphere = world.makeSphere({1.0f, 0.0f, 0.0f});
                tmpSphere->setPosition(particle.getPosition());
                tmpSphere->scale(0.03f);

                marks.push_back(tmpSphere);
            }

            // Initialisation des forces s'appliquant sur le projectile
            // gravité
            Vector3f g(0, 0, -9.81);
            float mass = 1.0f / particle.getInverseMass();

            std::vector<Vector3f> forceList = { mass * g };

            particle.integrate(forceList, TIMESTEP);
            particleRendered->setPosition(particle.getPosition());
        }
    }

    // Reset
    if(world.getWindow().isKeyPressed(GLFW_KEY_R))
    {
        particle.setPosition(positionInit);
        particle.setVelocity(velocityInit);
        resetMarks = true;
    }

    // Pause
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

void Application::setPositionInit(Vector3f positionInit)
{
    this->positionInit = positionInit;
}

void Application::setVelocityInit(Vector3f velocityInit)
{
    this->velocityInit = velocityInit;
}

void Application::setResetMarks(bool resetMarks)
{
    this->resetMarks = resetMarks;
}