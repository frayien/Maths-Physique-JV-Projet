#include <iostream>

#include "render/VulkanApplication.hpp"
#include "TestApplication.hpp"
#include "Application.hpp"

int main() 
{
    //VulkanApplication app{std::make_shared<TestApplication>()};
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
    Particle particle{ position, 50, 1 };
    Vector3f initialVelocity{ 5, 5, 5 };
    particle.setInitialVelocity(initialVelocity);
    particle.integrate();


    return 0;
}