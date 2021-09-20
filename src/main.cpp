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

int _main()
{
    // required to use ""s and ""ns suffixes
    using namespace std::chrono_literals;

    const std::chrono::nanoseconds TIMESTEP(1s);
    const float DELTA_TIME = TIMESTEP.count() / 1'000'000'000;
    
    using clock = std::chrono::high_resolution_clock;

    std::chrono::nanoseconds lag(0ns);
    auto base_time = clock::now();

    Vector3f acceleration{1, 2, 1};
    Vector3f speed{0, 0, 0};
    Vector3f position{0, 0, 0};

    while(true) {
        auto current_time = clock::now();
        auto frame_time = current_time - base_time;
        base_time = current_time;
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(frame_time);

        while(lag >= TIMESTEP) {
            lag -= TIMESTEP;

            speed += (acceleration * DELTA_TIME);
            position += speed * DELTA_TIME + (DELTA_TIME * DELTA_TIME * acceleration / 2);

            std::cout << "================" << std::endl;
            std::cout << "acceleration = " << acceleration << std::endl;
            std::cout << "speed = " << speed << std::endl;
            std::cout << "position = " << position << std::endl;
            std::cout << "================" << std::endl << std::endl;
        }
    }
}