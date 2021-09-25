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
    particleRendered = world.makeCube({ 1.0f, 1.0f, 1.0f });
    particleRendered->setPosition({ 0.0f, 0.0f, 0.0f });
    particleRendered->scale(0.05f);

    // Construction de la particule avec les données spécifiées par l'utilisateur dans ImGUI - A CHANGER
    Vector3f position{ 0, 0, 0 };
    particle = new Particle(position, 50, 0.999);
    Vector3f initialVelocity{ 5, 5, 5 };
    particle->setInitialVelocity(initialVelocity);

    // Initialisation des forces s'appliquant sur le projectile
    Vector3f g(0, -9.81, 0);
    float mass = 1.0f / particle->getInverseMass();
    forceList = { mass * g };
}

void Application::update(World & world, float deltaTime)
{

    static float totalTime = 0.0f;
    totalTime += deltaTime;
    lag += deltaTime;

    // Si le temps écoulé est supérieur à TIMESTEP, on entre dans la boucle
    while (lag >= TIMESTEP)
    {
        // On update la position du projectile ainsi que de son affichage graphique
        particleRendered->setPosition({ particle->getPosition().getX(), particle->getPosition().getY(), particle->getPosition().getZ() });
        lag -= TIMESTEP;
        particle->integrate(forceList, TIMESTEP);
    }
}
