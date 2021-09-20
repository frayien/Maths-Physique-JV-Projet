#include <iostream>

#include "render/VulkanApplication.hpp"
#include "render/IApplication.hpp"

class Application : public IApplication
{
    virtual void init(World & world) override
    {
        world.getCamera().setPosition(glm::vec3{2.0f, 2.0f, 2.0f});
        world.getCamera().setRotation(-3.0f * glm::pi<float>()/4.0f, -glm::pi<float>()/4.0f);

        world.addEntity(
        std::vector<Vertex>({
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f , -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.5f ,  0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
            {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        }),
        std::vector<uint32_t>({
            0, 1, 2, 2, 3, 0,
        }));
        world.addEntity(
        std::vector<Vertex>({
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f , -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}},
            {{0.5f ,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        }),
        std::vector<uint32_t>({
            0, 1, 2, 2, 3, 0,
        }));
    }

    virtual void update(World & world, float deltaTime) override
    {
        const float rotationSpeed = 1.0f * glm::pi<float>(); // rad/s
        const float moveSpeed     = 0.1f; // unit/s

        if(world.getWindow().isKeyPressed(GLFW_KEY_A))
        {
            world.getCamera().rotateYaw(deltaTime * rotationSpeed);
        }
        else if(world.getWindow().isKeyPressed(GLFW_KEY_D))
        {
            world.getCamera().rotateYaw(-deltaTime * rotationSpeed);
        }

        if(world.getWindow().isKeyPressed(GLFW_KEY_W))
        {
            world.getCamera().rotatePitch(deltaTime * rotationSpeed);
        }
        else if(world.getWindow().isKeyPressed(GLFW_KEY_S))
        {
            world.getCamera().rotatePitch(-deltaTime * rotationSpeed);
        }

        if(world.getWindow().isKeyPressed(GLFW_KEY_LEFT))
        {
            world.getCamera().move(moveSpeed * glm::vec3{0.0f, 1.0f, 0.0f});
        }
        else if(world.getWindow().isKeyPressed(GLFW_KEY_RIGHT))
        {
            world.getCamera().move(moveSpeed * glm::vec3{0.0f, -1.0f, 0.0f});
        }

        if(world.getWindow().isKeyPressed(GLFW_KEY_UP))
        {
            world.getCamera().move(moveSpeed * glm::vec3{1.0f, 0.0f, 0.0f});
        }
        else if(world.getWindow().isKeyPressed(GLFW_KEY_DOWN))
        {
            world.getCamera().move(moveSpeed * glm::vec3{-1.0f, 0.0f, 0.0f});
        }

        if(world.getWindow().isKeyPressed(GLFW_KEY_O))
        {
            world.getCamera().move(moveSpeed * glm::vec3{0.0f, 0.0f, 1.0f});
        }
        else if(world.getWindow().isKeyPressed(GLFW_KEY_L))
        {
            world.getCamera().move(moveSpeed * glm::vec3{0.0f, 0.0f, -1.0f});
        }
    }
};

int main() 
{
    VulkanApplication app{std::make_shared<Application>()};

    try 
    {
        app.run();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


#include "Vector3f.hpp"
#include "Particle.hpp"


int _main()
{
    Vector3f position{ 0, 0, 0 };
    Particle particle{ position, 50, 0.95 };
    Vector3f initialVelocity{ 5, 5, 5 };
    particle.setInitialVelocity(initialVelocity);
    particle.integrate();
}