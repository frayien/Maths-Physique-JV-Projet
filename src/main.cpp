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
