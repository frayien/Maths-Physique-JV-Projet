#include "TestApplication.hpp"

void TestApplication::init(World & world)
{
    Camera & cam = world.getCamera();
    LightSource & sun = world.getLightSource();

    cam.setPosition({2.0f, 2.0f, 3.0f});
    cam.setRotation(-3.0f * PI/4.0f, -PI/4.0f);

    //sun.setPosition({10.0f, 6.0f, 14.0f});
    sun.setPosition({2.0f, 0.0f, 6.0f});

    sunShape = world.makeCube({1.0f, 1.0f, 1.0f});
    sunShape->translate(sun.getPosition());
    sunShape->scale(0.1f);

    platform = world.makeShape(
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f , -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f ,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
    },
    {
        0, 1, 2, 2, 3, 0,
    });

    cube = world.makeCube();
    cube->scale(0.5f);
    cube->translate({0.0f, 0.0f, 5.0f});

    auto square = world.makeSquare();
    square->scale(2.0f);
    square->translate({0.0f, 0.0f, -5.0f});

    auto disk = world.makeDisk();
    disk->scale(0.5f);
    disk->translate({3.0f, 0.0f, 0.3f});
}

void TestApplication::update(World & world, float deltaTime)
{
    static float totalTime = 0.0f;
    totalTime += deltaTime;
    const float rotationSpeed = 1.0f / 3.0f * PI; // rad/s

    platform->rotate(deltaTime * rotationSpeed, {0.0f, 0.0f, 1.0f});

    cube->rotate(rotationSpeed * 0.7f * deltaTime, {1.0f,-1.0f, 0.0f});

    // rotate the sun
    float angle = rotationSpeed * totalTime;
    glm::vec3 pos = { 2.0f * glm::cos(angle), 2.0f * glm::sin(angle), 6.0f };

    world.getLightSource().setPosition(pos);
    sunShape->setPosition(pos);

    // update camera mouvement
    updateCamera(world, deltaTime);
}

void TestApplication::updateCamera(World & world, float deltaTime)
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