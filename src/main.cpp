#include <iostream>

#include "render/GraphicsEngine.hpp"
#include "Application.hpp"

#include "render/shape/SphereShapeGenerator.hpp"

int main()
{
    Application application{};
    GraphicsEngine graphics{&application};

    Camera & cam = graphics.getCamera();
    LightSource & sun = graphics.getLightSource();

    cam.setPosition({5.0f, 0.0f, 0.0f});
    // rotate cam to look at X- (this way Y+ and Z+ looks more natural for a 2D environment, aka. Y+ is on the right)
    cam.rotateYaw(glm::pi<float>());

    sun.setPosition({5.0f, 0.0f, 0.0f});

    try 
    {
        while (!graphics.windowShouldClose()) 
        {
            static auto previousTime = std::chrono::high_resolution_clock::now();

            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - previousTime).count();
            previousTime = currentTime;


            graphics.windowPollEvents();

            graphics.clear();

            SphereShapeGenerator sphereA{{0.f, 0.f, 1.f}};
            sphereA.setScale({0.5f,0.5f,0.5f});
            sphereA.setPosition({0.0f, 0.0f, 1.0f});

            graphics.draw(sphereA);
            graphics.draw(SphereShapeGenerator{{0.f, 1.f, 0.f}});

            graphics.display();
        }
    } 
    catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
